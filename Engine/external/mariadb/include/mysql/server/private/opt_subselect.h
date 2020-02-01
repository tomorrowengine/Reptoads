



#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

int check_and_do_in_subquery_rewrites(JOIN *join);
bool convert_join_subqueries_to_semijoins(JOIN *join);
int pull_out_semijoin_tables(JOIN *join);
bool optimize_semijoin_nests(JOIN *join, table_map all_table_map);
bool setup_jtbm_semi_joins(JOIN *join, List<TABLE_LIST> *join_list,  
                           Item **join_where);
void cleanup_empty_jtbm_semi_joins(JOIN *join, List<TABLE_LIST> *join_list);


ulonglong get_bound_sj_equalities(TABLE_LIST *sj_nest, 
                                  table_map remaining_tables);



class Loose_scan_opt
{
  
  bool try_loosescan;

  
  ulonglong bound_sj_equalities;
 
  
  ulonglong handled_sj_equalities;
  key_part_map loose_scan_keyparts;
  uint max_loose_keypart;
  bool part1_conds_met;

  
  uint quick_uses_applicable_index;
  uint quick_max_loose_keypart;
  
  
  uint   best_loose_scan_key;
  double best_loose_scan_cost;
  double best_loose_scan_records;
  KEYUSE *best_loose_scan_start_key;

  uint best_max_loose_keypart;

public:
  Loose_scan_opt():
    try_loosescan(FALSE),
    bound_sj_equalities(0),
    quick_uses_applicable_index(FALSE)
  {
    
    LINT_INIT(quick_max_loose_keypart);
    
    LINT_INIT(best_loose_scan_key);
    LINT_INIT(best_loose_scan_records);
    LINT_INIT(best_max_loose_keypart);
    LINT_INIT(best_loose_scan_start_key);
  }
  
  void init(JOIN *join, JOIN_TAB *s, table_map remaining_tables)
  {
    
    best_loose_scan_cost= DBL_MAX;
    if (!join->emb_sjm_nest && s->emb_sj_nest &&                        
        s->emb_sj_nest->sj_in_exprs < 64 && 
        ((remaining_tables & s->emb_sj_nest->sj_inner_tables) ==        
         s->emb_sj_nest->sj_inner_tables) &&                            
        join->cur_sj_inner_tables == 0 &&                               
        !(remaining_tables & 
          s->emb_sj_nest->nested_join->sj_corr_tables) &&               
        remaining_tables & s->emb_sj_nest->nested_join->sj_depends_on &&
        optimizer_flag(join->thd, OPTIMIZER_SWITCH_LOOSE_SCAN))
    {
      
      bound_sj_equalities= get_bound_sj_equalities(s->emb_sj_nest, 
                                                   remaining_tables);
      try_loosescan= TRUE;
      DBUG_PRINT("info", ("Will try LooseScan scan, bound_map=%llx",
                          (longlong)bound_sj_equalities));
    }
  }

  void next_ref_key()
  {
    handled_sj_equalities=0;
    loose_scan_keyparts= 0;
    max_loose_keypart= 0;
    part1_conds_met= FALSE;
  }
  
  void add_keyuse(table_map remaining_tables, KEYUSE *keyuse)
  {
    if (try_loosescan && keyuse->sj_pred_no != UINT_MAX &&
        (keyuse->table->file->index_flags(keyuse->key, 0, 1 ) & HA_READ_ORDER))

    {
      if (!(remaining_tables & keyuse->used_tables))
      {
        
        bound_sj_equalities |= 1ULL << keyuse->sj_pred_no;
      }
      else
      {
        handled_sj_equalities |= 1ULL << keyuse->sj_pred_no;
        loose_scan_keyparts |= ((key_part_map)1) << keyuse->keypart;
        set_if_bigger(max_loose_keypart, keyuse->keypart);
      }
    }
  }

  bool have_a_case() { return MY_TEST(handled_sj_equalities); }

  void check_ref_access_part1(JOIN_TAB *s, uint key, KEYUSE *start_key, 
                              table_map found_part)
  {
    
    if (try_loosescan &&                                       
        (handled_sj_equalities | bound_sj_equalities) ==       
        PREV_BITS(ulonglong, s->emb_sj_nest->sj_in_exprs) &&   
        (PREV_BITS(key_part_map, max_loose_keypart+1) &        
         (found_part | loose_scan_keyparts)) ==                
        PREV_BITS(key_part_map, max_loose_keypart+1) &&        
        !key_uses_partial_cols(s->table->s, key))
    {
      if (s->quick && s->quick->index == key && 
          s->quick->get_type() == QUICK_SELECT_I::QS_TYPE_RANGE)
      {
        quick_uses_applicable_index= TRUE;
        quick_max_loose_keypart= max_loose_keypart;
      }
      DBUG_PRINT("info", ("Can use LooseScan scan"));

      if (found_part & 1)
      {
        
        part1_conds_met= TRUE;
      }

      
      if (!(found_part & 1 ) && 
          s->table->covering_keys.is_set(key))
      {
        part1_conds_met= TRUE;
        DBUG_PRINT("info", ("Can use full index scan for LooseScan"));
        
        
        double records= rows2double(s->table->file->stats.records);

        
        double read_time= s->table->file->keyread_time(key, 1,
                                                       (ha_rows) records);

        
        ulong rpc;
        if ((rpc= s->table->key_info[key].rec_per_key[max_loose_keypart]))
          records= records / rpc;

        
        if (read_time < best_loose_scan_cost)
        {
          best_loose_scan_key= key;
          best_loose_scan_cost= read_time;
          best_loose_scan_records= records;
          best_max_loose_keypart= max_loose_keypart;
          best_loose_scan_start_key= start_key;
        }
      }
    }
  }
  
  void check_ref_access_part2(uint key, KEYUSE *start_key, double records, 
                              double read_time)
  {
    if (part1_conds_met && read_time < best_loose_scan_cost)
    {
      
      best_loose_scan_key= key;
      best_loose_scan_cost= read_time;
      best_loose_scan_records= records;
      best_max_loose_keypart= max_loose_keypart;
      best_loose_scan_start_key= start_key;
    }
  }

  void check_range_access(JOIN *join, uint idx, QUICK_SELECT_I *quick)
  {
    
    if (quick_uses_applicable_index && idx == join->const_tables && 
        quick->read_time < best_loose_scan_cost)
    {
      best_loose_scan_key= quick->index;
      best_loose_scan_cost= quick->read_time;
      
      best_loose_scan_records= rows2double(quick->records);
      best_max_loose_keypart= quick_max_loose_keypart;
      best_loose_scan_start_key= NULL;
    }
  }

  void save_to_position(JOIN_TAB *tab, POSITION *pos)
  {
    pos->read_time=       best_loose_scan_cost;
    if (best_loose_scan_cost != DBL_MAX)
    {
      pos->records_read=    best_loose_scan_records;
      pos->key=             best_loose_scan_start_key;
      pos->cond_selectivity= 1.0;
      pos->loosescan_picker.loosescan_key=   best_loose_scan_key;
      pos->loosescan_picker.loosescan_parts= best_max_loose_keypart + 1;
      pos->use_join_buffer= FALSE;
      pos->table=           tab;
      
      DBUG_PRINT("info", ("Produced a LooseScan plan, key %s, %s",
                          tab->table->key_info[best_loose_scan_key].name.str,
                          best_loose_scan_start_key? "(ref access)":
                                                     "(range/index access)"));
    }
  }
};


void advance_sj_state(JOIN *join, table_map remaining_tables, uint idx,
                      double *current_record_count, double *current_read_time,
                      POSITION *loose_scan_pos);
void restore_prev_sj_state(const table_map remaining_tables, 
                                  const JOIN_TAB *tab, uint idx);

void fix_semijoin_strategies_for_picked_join_order(JOIN *join);

bool setup_sj_materialization_part1(JOIN_TAB *sjm_tab);
bool setup_sj_materialization_part2(JOIN_TAB *sjm_tab);




class SJ_TMP_TABLE : public Sql_alloc
{
public:
  
  class TAB
  {
  public:
    JOIN_TAB *join_tab;
    uint rowid_offset;
    ushort null_byte;
    uchar null_bit;
  };
  TAB *tabs;
  TAB *tabs_end;
  
  
  bool is_degenerate;

  
  bool have_degenerate_row;
  
  
  uint null_bits;
  uint null_bytes;
  uint rowid_len;

  
  TABLE *tmp_table;
  
  
  TMP_ENGINE_COLUMNDEF *start_recinfo;
  TMP_ENGINE_COLUMNDEF *recinfo;

  SJ_TMP_TABLE *next_flush_table; 

  int sj_weedout_delete_rows();
  int sj_weedout_check_row(THD *thd);
  bool create_sj_weedout_tmp_table(THD *thd);
};

int setup_semijoin_loosescan(JOIN *join);
int setup_semijoin_dups_elimination(JOIN *join, ulonglong options, 
                                    uint no_jbuf_after);
void destroy_sj_tmp_tables(JOIN *join);
int clear_sj_tmp_tables(JOIN *join);
int rewrite_to_index_subquery_engine(JOIN *join);


void get_delayed_table_estimates(TABLE *table,
                                 ha_rows *out_rows, 
                                 double *scan_time,
                                 double *startup_cost);

enum_nested_loop_state join_tab_execution_startup(JOIN_TAB *tab);

