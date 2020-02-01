


#ifndef _SP_HEAD_H_
#define _SP_HEAD_H_

#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif


#include "sql_class.h"                          
#include "set_var.h"                            
#include "sp_pcontext.h"                        
#include <stddef.h>
#include "sp.h"



Item::Type
sp_map_item_type(const Type_handler *handler);

uint
sp_get_flags_for_command(LEX *lex);

class sp_instr;
class sp_instr_opt_meta;
class sp_instr_jump_if_not;





class Stored_program_creation_ctx :public Default_object_creation_ctx
{
public:
  CHARSET_INFO *get_db_cl()
  {
    return m_db_cl;
  }

public:
  virtual Stored_program_creation_ctx *clone(MEM_ROOT *mem_root) = 0;

protected:
  Stored_program_creation_ctx(THD *thd)
    : Default_object_creation_ctx(thd),
      m_db_cl(thd->variables.collation_database)
  { }

  Stored_program_creation_ctx(CHARSET_INFO *client_cs,
                              CHARSET_INFO *connection_cl,
                              CHARSET_INFO *db_cl)
    : Default_object_creation_ctx(client_cs, connection_cl),
      m_db_cl(db_cl)
  { }

protected:
  virtual void change_env(THD *thd) const
  {
    thd->variables.collation_database= m_db_cl;

    Default_object_creation_ctx::change_env(thd);
  }

protected:
  
  CHARSET_INFO *m_db_cl;
};



class sp_name : public Sql_alloc,
                public Database_qualified_name
{
public:
  bool       m_explicit_name;                   

  sp_name(const LEX_CSTRING *db, const LEX_CSTRING *name,
          bool use_explicit_name)
    : Database_qualified_name(db, name), m_explicit_name(use_explicit_name)
  {
    if (lower_case_table_names && m_db.str)
      m_db.length= my_casedn_str(files_charset_info, (char*) m_db.str);
  }

  
  sp_name(const MDL_key *key, char *qname_buff);

  ~sp_name()
  {}
};


bool
check_routine_name(const LEX_CSTRING *ident);

class sp_head :private Query_arena,
               public Database_qualified_name
{
  sp_head(const sp_head &);	
  void operator=(sp_head &);

protected:
  MEM_ROOT main_mem_root;
public:
  
  enum {
    HAS_RETURN= 1,              
    MULTI_RESULTS= 8,           
    CONTAINS_DYNAMIC_SQL= 16,   
    IS_INVOKED= 32,             
    HAS_SET_AUTOCOMMIT_STMT= 64,
    
    HAS_COMMIT_OR_ROLLBACK= 128,
    LOG_SLOW_STATEMENTS= 256,   
    LOG_GENERAL_LOG= 512,        
    HAS_SQLCOM_RESET= 1024,
    HAS_SQLCOM_FLUSH= 2048,

    
    MODIFIES_DATA= 4096,
    
    HAS_COLUMN_TYPE_REFS= 8192,
    
    HAS_AGGREGATE_INSTR= 16384
  };

  sp_package *m_parent;
  const Sp_handler *m_handler;
  uint m_flags;                 

  Column_definition m_return_field_def; 

  const char *m_tmp_query;	
private:
  
  Sp_chistics m_chistics;
public:
  sql_mode_t m_sql_mode;		
  bool       m_explicit_name;                   
  LEX_CSTRING m_qname;		
  LEX_CSTRING m_params;
  LEX_CSTRING m_body;
  LEX_CSTRING m_body_utf8;
  LEX_CSTRING m_defstr;
  AUTHID      m_definer;

  const st_sp_chistics &chistics() const { return m_chistics; }
  const LEX_CSTRING &comment() const { return m_chistics.comment; }
  void set_suid(enum_sp_suid_behaviour suid) { m_chistics.suid= suid; }
  enum_sp_suid_behaviour suid() const { return m_chistics.suid; }
  bool detistic() const { return m_chistics.detistic; }
  enum_sp_data_access daccess() const { return m_chistics.daccess; }
  enum_sp_aggregate_type agg_type() const { return m_chistics.agg_type; }
  
  virtual bool is_invoked() const { return m_flags & IS_INVOKED; }

  
  ulong sp_cache_version() const;

  
  void set_sp_cache_version(ulong version_arg) const
  {
    m_sp_cache_version= version_arg;
  }

  sp_rcontext *rcontext_create(THD *thd, Field *retval, List<Item> *args);
  sp_rcontext *rcontext_create(THD *thd, Field *retval,
                               Item **args, uint arg_count);
  sp_rcontext *rcontext_create(THD *thd, Field *retval,
                               Row_definition_list *list,
                               bool switch_security_ctx);
  bool eq_routine_spec(const sp_head *) const;
private:
  
  mutable ulong m_sp_cache_version;
  Stored_program_creation_ctx *m_creation_ctx;
  
  uint32 unsafe_flags;

public:
  inline Stored_program_creation_ctx *get_creation_ctx()
  {
    return m_creation_ctx;
  }

  inline void set_creation_ctx(Stored_program_creation_ctx *creation_ctx)
  {
    m_creation_ctx= creation_ctx->clone(mem_root);
  }

  longlong m_created;
  longlong m_modified;
  
  ulong m_recursion_level;
  
  sp_head *m_next_cached_sp;
  
  sp_head *m_first_instance;
  
  sp_head *m_first_free_instance;
  
  sp_head *m_last_cached_sp;
  
  HASH m_sroutines;
  
  const char *m_param_begin;
  const char *m_param_end;

private:
  const char *m_body_begin;

public:
  
  Security_context m_security_ctx;

  
  SQL_I_List<Item_trigger_field> m_trg_table_fields;

  static void *
  operator new(size_t size) throw ();

  static void
  operator delete(void *ptr, size_t size) throw ();

  sp_head(sp_package *parent, const Sp_handler *handler);

  
  void
  init(LEX *lex);

  
  void
  init_sp_name(const sp_name *spname);

  
  void
  set_body_start(THD *thd, const char *begin_ptr);

  
  void
  set_stmt_end(THD *thd);

  virtual ~sp_head();

  bool
  execute_trigger(THD *thd,
                  const LEX_CSTRING *db_name,
                  const LEX_CSTRING *table_name,
                  GRANT_INFO *grant_info);

  bool
  execute_function(THD *thd, Item **args, uint argcount, Field *return_fld,
                   sp_rcontext **nctx, Query_arena *call_arena);

  bool
  execute_procedure(THD *thd, List<Item> *args);

  static void
  show_create_routine_get_fields(THD *thd, const Sp_handler *sph,
                                 List<Item> *fields);

  bool
  show_create_routine(THD *thd, const Sp_handler *sph);

  MEM_ROOT *get_main_mem_root() { return &main_mem_root; }

  int
  add_instr(sp_instr *instr);

  bool
  add_instr_jump(THD *thd, sp_pcontext *spcont);

  bool
  add_instr_jump(THD *thd, sp_pcontext *spcont, uint dest);

  bool
  add_instr_jump_forward_with_backpatch(THD *thd, sp_pcontext *spcont,
                                        sp_label *lab);
  bool
  add_instr_jump_forward_with_backpatch(THD *thd, sp_pcontext *spcont)
  {
    return add_instr_jump_forward_with_backpatch(thd, spcont,
                                                 spcont->last_label());
  }

  bool
  add_instr_freturn(THD *thd, sp_pcontext *spcont, Item *item, LEX *lex);

  bool
  add_instr_preturn(THD *thd, sp_pcontext *spcont);

  Item *adjust_assignment_source(THD *thd, Item *val, Item *val2);
  
  bool set_local_variable(THD *thd, sp_pcontext *spcont,
                          const Sp_rcontext_handler *rh,
                          sp_variable *spv, Item *val, LEX *lex,
                          bool responsible_to_free_lex);
  bool set_local_variable_row_field(THD *thd, sp_pcontext *spcont,
                                    const Sp_rcontext_handler *rh,
                                    sp_variable *spv, uint field_idx,
                                    Item *val, LEX *lex);
  bool set_local_variable_row_field_by_name(THD *thd, sp_pcontext *spcont,
                                            const Sp_rcontext_handler *rh,
                                            sp_variable *spv,
                                            const LEX_CSTRING *field_name,
                                            Item *val, LEX *lex);
  bool check_package_routine_end_name(const LEX_CSTRING &end_name) const;
private:
  
  bool add_set_cursor_param_variable(THD *thd,
                                     sp_pcontext *param_spcont, uint idx,
                                     sp_assignment_lex *prm)
  {
    DBUG_ASSERT(idx < param_spcont->context_var_count());
    sp_variable *spvar= param_spcont->get_context_variable(idx);
    
    DBUG_ASSERT(m_thd->free_list == NULL);
    m_thd->free_list= prm->get_free_list();
    if (set_local_variable(thd, param_spcont,
                           &sp_rcontext_handler_local,
                           spvar, prm->get_item(), prm, true))
      return true;
    
    prm->set_item_and_free_list(NULL, NULL);
    return false;
  }

  
  bool add_set_cursor_param_variables(THD *thd, sp_pcontext *param_spcont,
                                      List<sp_assignment_lex> *parameters)
  {
    DBUG_ASSERT(param_spcont->context_var_count() == parameters->elements);
    sp_assignment_lex *prm;
    List_iterator<sp_assignment_lex> li(*parameters);
    for (uint idx= 0; (prm= li++); idx++)
    {
      if (add_set_cursor_param_variable(thd, param_spcont, idx, prm))
        return true;
    }
    return false;
  }

  
  bool add_set_for_loop_cursor_param_variables(THD *thd,
                                               sp_pcontext *param_spcont,
                                               sp_assignment_lex *param_lex,
                                               Item_args *parameters);

public:
  
  bool add_open_cursor(THD *thd, sp_pcontext *spcont,
                       uint offset,
                       sp_pcontext *param_spcont,
                       List<sp_assignment_lex> *parameters);

  
  bool add_for_loop_open_cursor(THD *thd, sp_pcontext *spcont,
                                sp_variable *index,
                                const sp_pcursor *pcursor, uint coffset,
                                sp_assignment_lex *param_lex,
                                Item_args *parameters);
  
  bool modifies_data() const
  { return m_flags & MODIFIES_DATA; }

  inline uint instructions()
  { return m_instr.elements; }

  inline sp_instr *
  last_instruction()
  {
    sp_instr *i;

    get_dynamic(&m_instr, (uchar*)&i, m_instr.elements-1);
    return i;
  }

  bool replace_instr_to_nop(THD *thd, uint ip);

  
  bool
  reset_lex(THD *thd);

  bool
  reset_lex(THD *thd, sp_lex_local *sublex);

  
  bool
  merge_lex(THD *thd, LEX *oldlex, LEX *sublex);

  
  bool
  restore_lex(THD *thd)
  {
    DBUG_ENTER("sp_head::restore_lex");
    LEX *oldlex= (LEX *) m_lex.pop();
    if (!oldlex)
      DBUG_RETURN(false); 
    LEX *sublex= thd->lex;
    if (thd->restore_from_local_lex_to_old_lex(oldlex))
      DBUG_RETURN(true);
    if (!sublex->sp_lex_in_use)
    {
      sublex->sphead= NULL;
      lex_end(sublex);
      delete sublex;
    }
    DBUG_RETURN(false);
  }

  
  int
  push_backpatch(THD *thd, sp_instr *, sp_label *);
  int
  push_backpatch_goto(THD *thd, sp_pcontext *ctx, sp_label *lab);

  
  
  void
  backpatch(sp_label *);
  void
  backpatch_goto(THD *thd, sp_label *, sp_label *);

  
  bool
  check_unresolved_goto();

  
  int
  new_cont_backpatch(sp_instr_opt_meta *i);

  
  int
  add_cont_backpatch(sp_instr_opt_meta *i);

  
  void
  do_cont_backpatch();

  
  bool sp_add_instr_cpush_for_cursors(THD *thd, sp_pcontext *pcontext);

  const LEX_CSTRING *name() const
  { return &m_name; }

  char *create_string(THD *thd, ulong *lenp);

  Field *create_result_field(uint field_max_length, const LEX_CSTRING *field_name,
                             TABLE *table) const;


  
  bool fill_field_definition(THD *thd, Column_definition *field_def)
  {
    const Type_handler *h= field_def->type_handler();
    return h->Column_definition_fix_attributes(field_def) ||
           field_def->sp_prepare_create_field(thd, mem_root);
  }
  bool row_fill_field_definitions(THD *thd, Row_definition_list *row)
  {
    
    List_iterator<Spvar_definition> it(*row);
    for (Spvar_definition *def= it++; def; def= it++)
    {
      if (fill_spvar_definition(thd, def))
        return true;
    }
    return false;
  }
  
  bool fill_spvar_definition(THD *thd, Column_definition *def)
  {
    if (fill_field_definition(thd, def))
      return true;
    def->pack_flag|= FIELDFLAG_MAYBE_NULL;
    return false;
  }
  bool fill_spvar_definition(THD *thd, Column_definition *def,
                             LEX_CSTRING *name)
  {
    def->field_name= *name;
    return fill_spvar_definition(thd, def);
  }

private:
  
  void fill_spvar_using_type_reference(sp_variable *spvar,
                                       Qualified_column_ident *ref)
  {
    spvar->field_def.set_column_type_ref(ref);
    spvar->field_def.field_name= spvar->name;
    m_flags|= sp_head::HAS_COLUMN_TYPE_REFS;
  }

  void fill_spvar_using_table_rowtype_reference(THD *thd,
                                                sp_variable *spvar,
                                                Table_ident *ref)
  {
    spvar->field_def.set_table_rowtype_ref(ref);
    spvar->field_def.field_name= spvar->name;
    fill_spvar_definition(thd, &spvar->field_def);
    m_flags|= sp_head::HAS_COLUMN_TYPE_REFS;
  }

public:
  bool spvar_fill_row(THD *thd, sp_variable *spvar, Row_definition_list *def);
  bool spvar_fill_type_reference(THD *thd, sp_variable *spvar,
                                 const LEX_CSTRING &table,
                                 const LEX_CSTRING &column);
  bool spvar_fill_type_reference(THD *thd, sp_variable *spvar,
                                 const LEX_CSTRING &db,
                                 const LEX_CSTRING &table,
                                 const LEX_CSTRING &column);
  bool spvar_fill_table_rowtype_reference(THD *thd, sp_variable *spvar,
                                          const LEX_CSTRING &table);
  bool spvar_fill_table_rowtype_reference(THD *thd, sp_variable *spvar,
                                          const LEX_CSTRING &db,
                                          const LEX_CSTRING &table);

  void set_chistics(const st_sp_chistics &chistics);
  inline void set_chistics_agg_type(enum enum_sp_aggregate_type type)
  {
    m_chistics.agg_type= type;
  }
  void set_info(longlong created, longlong modified,
		const st_sp_chistics &chistics, sql_mode_t sql_mode);

  void set_definer(const char *definer, size_t definerlen)
  {
    AUTHID tmp;
    tmp.parse(definer, definerlen);
    m_definer.copy(mem_root, &tmp.user, &tmp.host);
  }
  void set_definer(const LEX_CSTRING *user_name, const LEX_CSTRING *host_name)
  {
    m_definer.copy(mem_root, user_name, host_name);
  }

  void reset_thd_mem_root(THD *thd);

  void restore_thd_mem_root(THD *thd);

  
  void optimize();

  
  void add_mark_lead(uint ip, List<sp_instr> *leads);

  inline sp_instr *
  get_instr(uint i)
  {
    sp_instr *ip;

    if (i < m_instr.elements)
      get_dynamic(&m_instr, (uchar*)&ip, i);
    else
      ip= NULL;
    return ip;
  }

  
  bool add_used_tables_to_table_list(THD *thd,
                                     TABLE_LIST ***query_tables_last_ptr,
                                     TABLE_LIST *belong_to_view);

  
  bool is_not_allowed_in_function(const char *where)
  {
    if (m_flags & CONTAINS_DYNAMIC_SQL)
      my_error(ER_STMT_NOT_ALLOWED_IN_SF_OR_TRG, MYF(0), "Dynamic SQL");
    else if (m_flags & MULTI_RESULTS)
      my_error(ER_SP_NO_RETSET, MYF(0), where);
    else if (m_flags & HAS_SET_AUTOCOMMIT_STMT)
      my_error(ER_SP_CANT_SET_AUTOCOMMIT, MYF(0));
    else if (m_flags & HAS_COMMIT_OR_ROLLBACK)
      my_error(ER_COMMIT_NOT_ALLOWED_IN_SF_OR_TRG, MYF(0));
    else if (m_flags & HAS_SQLCOM_RESET)
      my_error(ER_STMT_NOT_ALLOWED_IN_SF_OR_TRG, MYF(0), "RESET");
    else if (m_flags & HAS_SQLCOM_FLUSH)
      my_error(ER_STMT_NOT_ALLOWED_IN_SF_OR_TRG, MYF(0), "FLUSH");

    return MY_TEST(m_flags &
                  (CONTAINS_DYNAMIC_SQL | MULTI_RESULTS |
                   HAS_SET_AUTOCOMMIT_STMT | HAS_COMMIT_OR_ROLLBACK |
                   HAS_SQLCOM_RESET | HAS_SQLCOM_FLUSH));
  }

#ifndef DBUG_OFF
  int show_routine_code(THD *thd);
#endif

  
  void propagate_attributes(Query_tables_list *prelocking_ctx)
  {
    DBUG_ENTER("sp_head::propagate_attributes");
    
    DBUG_PRINT("info", ("lex->get_stmt_unsafe_flags(): 0x%x",
                        prelocking_ctx->get_stmt_unsafe_flags()));
    DBUG_PRINT("info", ("sp_head(%p=%s)->unsafe_flags: 0x%x",
                        this, name()->str, unsafe_flags));
    prelocking_ctx->set_stmt_unsafe_flags(unsafe_flags);
    DBUG_VOID_RETURN;
  }

  sp_pcontext *get_parse_context() { return m_pcont; }

  
  bool check_execute_access(THD *thd) const;

  virtual sp_package *get_package()
  {
    return NULL;
  }

protected:

  MEM_ROOT *m_thd_root;		
  THD *m_thd;			

  sp_pcontext *m_pcont;		
  List<LEX> m_lex;		
  DYNAMIC_ARRAY m_instr;	

  enum backpatch_instr_type { GOTO, CPOP, HPOP };
  typedef struct
  {
    sp_label *lab;
    sp_instr *instr;
    backpatch_instr_type instr_type;
  } bp_t;
  List<bp_t> m_backpatch;	
  List<bp_t> m_backpatch_goto; 

  
  List<sp_instr_opt_meta> m_cont_backpatch;
  uint m_cont_level;            

  
  HASH m_sptabs;

  bool
  execute(THD *thd, bool merge_da_on_success);

  
  void opt_mark();

  
  bool merge_table_list(THD *thd, TABLE_LIST *table, LEX *lex_for_tmp_check);

  
  int
  push_backpatch(THD *thd, sp_instr *, sp_label *, List<bp_t> *list,
                 backpatch_instr_type itype);

}; 


class sp_package: public sp_head
{
  bool validate_public_routines(THD *thd, sp_package *spec);
  bool validate_private_routines(THD *thd);
public:
  class LexList: public List<LEX>
  {
  public:
    LexList() { elements= 0; }
    
    LEX *find(const LEX_CSTRING &name, stored_procedure_type type);
    
    LEX *find_qualified(const LEX_CSTRING &name, stored_procedure_type type);
    
    bool check_dup_qualified(const LEX_CSTRING &name, const Sp_handler *sph)
    {
      if (!find_qualified(name, sph->type()))
        return false;
      my_error(ER_SP_ALREADY_EXISTS, MYF(0), sph->type_str(), name.str);
      return true;
    }
    bool check_dup_qualified(const sp_head *sp)
    {
      return check_dup_qualified(sp->m_name, sp->m_handler);
    }
    void cleanup();
  };
  
  LEX *m_current_routine;
  LexList m_routine_implementations;
  LexList m_routine_declarations;

  LEX *m_top_level_lex;
  sp_rcontext *m_rcontext;
  uint m_invoked_subroutine_count;
  bool m_is_instantiated;
  bool m_is_cloning_routine;

  sp_package(LEX *top_level_lex,
             const sp_name *name,
             const Sp_handler *sph);
  ~sp_package();
  bool add_routine_declaration(LEX *lex)
  {
    return m_routine_declarations.check_dup_qualified(lex->sphead) ||
           m_routine_declarations.push_back(lex, &main_mem_root);
  }
  bool add_routine_implementation(LEX *lex)
  {
    return m_routine_implementations.check_dup_qualified(lex->sphead) ||
           m_routine_implementations.push_back(lex, &main_mem_root);
  }
  sp_package *get_package() { return this; }
  bool is_invoked() const
  {
    
    return sp_head::is_invoked() || m_invoked_subroutine_count > 0;
  }
  sp_variable *find_package_variable(const LEX_CSTRING *name) const
  {
    
    sp_pcontext *ctx= m_pcont->child_context(0);
    return ctx ? ctx->find_variable(name, true) : NULL;
  }
  bool validate_after_parser(THD *thd);
  bool instantiate_if_needed(THD *thd);
};


class sp_lex_cursor: public sp_lex_local, public Query_arena
{
public:
  sp_lex_cursor(THD *thd, const LEX *oldlex, MEM_ROOT *mem_root_arg)
   :sp_lex_local(thd, oldlex),
    Query_arena(mem_root_arg, STMT_INITIALIZED_FOR_SP)
  { }
  sp_lex_cursor(THD *thd, const LEX *oldlex)
   :sp_lex_local(thd, oldlex),
    Query_arena(thd->lex->sphead->get_main_mem_root(), STMT_INITIALIZED_FOR_SP)
  { }
  ~sp_lex_cursor() { free_items(); }
  void cleanup_stmt() { }
  Query_arena *query_arena() { return this; }
  bool validate()
  {
    DBUG_ASSERT(sql_command == SQLCOM_SELECT);
    if (result)
    {
      my_error(ER_SP_BAD_CURSOR_SELECT, MYF(0));
      return true;
    }
    return false;
  }
  bool stmt_finalize(THD *thd)
  {
    if (validate())
      return true;
    sp_lex_in_use= true;
    free_list= thd->free_list;
    thd->free_list= NULL;
    return false;
  }
};






class sp_instr :public Query_arena, public Sql_alloc
{
  sp_instr(const sp_instr &);	
  void operator=(sp_instr &);

public:

  uint marked;
  uint m_ip;			
  sp_pcontext *m_ctx;		
  uint m_lineno;

  
  sp_instr(uint ip, sp_pcontext *ctx)
    :Query_arena(0, STMT_INITIALIZED_FOR_SP), marked(0), m_ip(ip), m_ctx(ctx)
  {}

  virtual ~sp_instr()
  { free_items(); }


  

  virtual int execute(THD *thd, uint *nextp) = 0;

  
  int exec_open_and_lock_tables(THD *thd, TABLE_LIST *tables);

  
  virtual uint get_cont_dest() const;

  
  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str) = 0;

  virtual void backpatch(uint dest, sp_pcontext *dst_ctx)
  {}

  
  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads)
  {
    marked= 1;
    return m_ip+1;
  }

  
  virtual uint opt_shortcut_jump(sp_head *sp, sp_instr *start)
  {
    return m_ip;
  }

  
  virtual void opt_move(uint dst, List<sp_instr> *ibp)
  {
    m_ip= dst;
  }

}; 




class sp_lex_keeper
{
  
  sp_lex_keeper(const sp_lex_keeper &);
  void operator=(sp_lex_keeper &);
public:

  sp_lex_keeper(LEX *lex, bool lex_resp)
    : m_lex(lex), m_lex_resp(lex_resp), 
      lex_query_tables_own_last(NULL)
  {
    lex->sp_lex_in_use= TRUE;
  }
  virtual ~sp_lex_keeper()
  {
    if (m_lex_resp)
    {
      
      m_lex->sphead= NULL;
      lex_end(m_lex);
      delete m_lex;
    }
  }

  
  int reset_lex_and_exec_core(THD *thd, uint *nextp, bool open_tables,
                              sp_instr* instr);

  int cursor_reset_lex_and_exec_core(THD *thd, uint *nextp, bool open_tables,
                                     sp_instr *instr);

  inline uint sql_command() const
  {
    return (uint)m_lex->sql_command;
  }

  void disable_query_cache()
  {
    m_lex->safe_to_cache_query= 0;
  }

private:

  LEX *m_lex;
  
  bool m_lex_resp;

  
  
  
  TABLE_LIST *prelocking_tables;

  
  TABLE_LIST **lex_query_tables_own_last;
};



class sp_instr_stmt : public sp_instr
{
  sp_instr_stmt(const sp_instr_stmt &);	
  void operator=(sp_instr_stmt &);

public:

  LEX_STRING m_query;		

  sp_instr_stmt(uint ip, sp_pcontext *ctx, LEX *lex)
    : sp_instr(ip, ctx), m_lex_keeper(lex, TRUE)
  {
    m_query.str= 0;
    m_query.length= 0;
  }

  virtual ~sp_instr_stmt()
  {};

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

private:

  sp_lex_keeper m_lex_keeper;

}; 


class sp_instr_set : public sp_instr
{
  sp_instr_set(const sp_instr_set &);	
  void operator=(sp_instr_set &);

public:

  sp_instr_set(uint ip, sp_pcontext *ctx,
               const Sp_rcontext_handler *rh,
	       uint offset, Item *val,
               LEX *lex, bool lex_resp)
    : sp_instr(ip, ctx),
      m_rcontext_handler(rh), m_offset(offset), m_value(val),
      m_lex_keeper(lex, lex_resp)
  {}

  virtual ~sp_instr_set()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

protected:
  sp_rcontext *get_rcontext(THD *thd) const;
  const Sp_rcontext_handler *m_rcontext_handler;
  uint m_offset;		
  Item *m_value;
  sp_lex_keeper m_lex_keeper;
}; 



class sp_instr_set_row_field : public sp_instr_set
{
  sp_instr_set_row_field(const sp_instr_set_row_field &); 
  void operator=(sp_instr_set_row_field &);
  uint m_field_offset;

public:

  sp_instr_set_row_field(uint ip, sp_pcontext *ctx,
                         const Sp_rcontext_handler *rh,
                         uint offset, uint field_offset,
                         Item *val,
                         LEX *lex, bool lex_resp)
    : sp_instr_set(ip, ctx, rh, offset, val, lex, lex_resp),
      m_field_offset(field_offset)
  {}

  virtual ~sp_instr_set_row_field()
  {}

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);
}; 



class sp_instr_set_row_field_by_name : public sp_instr_set
{
  
  sp_instr_set_row_field_by_name(const sp_instr_set_row_field &);
  void operator=(sp_instr_set_row_field_by_name &);
  const LEX_CSTRING m_field_name;

public:

  sp_instr_set_row_field_by_name(uint ip, sp_pcontext *ctx,
                                 const Sp_rcontext_handler *rh,
                                 uint offset, const LEX_CSTRING &field_name,
                                 Item *val,
                                 LEX *lex, bool lex_resp)
    : sp_instr_set(ip, ctx, rh, offset, val, lex, lex_resp),
      m_field_name(field_name)
  {}

  virtual ~sp_instr_set_row_field_by_name()
  {}

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);
}; 



class sp_instr_set_trigger_field : public sp_instr
{
  sp_instr_set_trigger_field(const sp_instr_set_trigger_field &);
  void operator=(sp_instr_set_trigger_field &);

public:

  sp_instr_set_trigger_field(uint ip, sp_pcontext *ctx,
                             Item_trigger_field *trg_fld,
                             Item *val, LEX *lex)
    : sp_instr(ip, ctx),
      trigger_field(trg_fld),
      value(val), m_lex_keeper(lex, TRUE)
  {}

  virtual ~sp_instr_set_trigger_field()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

private:
  Item_trigger_field *trigger_field;
  Item *value;
  sp_lex_keeper m_lex_keeper;
}; 



class sp_instr_opt_meta : public sp_instr
{
public:

  uint m_dest;			
  uint m_cont_dest;             

  sp_instr_opt_meta(uint ip, sp_pcontext *ctx)
    : sp_instr(ip, ctx),
      m_dest(0), m_cont_dest(0), m_optdest(0), m_cont_optdest(0)
  {}

  sp_instr_opt_meta(uint ip, sp_pcontext *ctx, uint dest)
    : sp_instr(ip, ctx),
      m_dest(dest), m_cont_dest(0), m_optdest(0), m_cont_optdest(0)
  {}

  virtual ~sp_instr_opt_meta()
  {}

  virtual void set_destination(uint old_dest, uint new_dest)
    = 0;

  virtual uint get_cont_dest() const;

protected:

  sp_instr *m_optdest;		
  sp_instr *m_cont_optdest;     

}; 

class sp_instr_jump : public sp_instr_opt_meta
{
  sp_instr_jump(const sp_instr_jump &);	
  void operator=(sp_instr_jump &);

public:

  sp_instr_jump(uint ip, sp_pcontext *ctx)
    : sp_instr_opt_meta(ip, ctx)
  {}

  sp_instr_jump(uint ip, sp_pcontext *ctx, uint dest)
    : sp_instr_opt_meta(ip, ctx, dest)
  {}

  virtual ~sp_instr_jump()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads);

  virtual uint opt_shortcut_jump(sp_head *sp, sp_instr *start);

  virtual void opt_move(uint dst, List<sp_instr> *ibp);

  virtual void backpatch(uint dest, sp_pcontext *dst_ctx)
  {
    
    DBUG_ASSERT(m_dest == 0);
    m_dest= dest;
  }

  
  virtual void set_destination(uint old_dest, uint new_dest)
  {
    if (m_dest == old_dest)
      m_dest= new_dest;
  }

}; 


class sp_instr_jump_if_not : public sp_instr_jump
{
  sp_instr_jump_if_not(const sp_instr_jump_if_not &); 
  void operator=(sp_instr_jump_if_not &);

public:

  sp_instr_jump_if_not(uint ip, sp_pcontext *ctx, Item *i, LEX *lex)
    : sp_instr_jump(ip, ctx), m_expr(i),
      m_lex_keeper(lex, TRUE)
  {}

  sp_instr_jump_if_not(uint ip, sp_pcontext *ctx, Item *i, uint dest, LEX *lex)
    : sp_instr_jump(ip, ctx, dest), m_expr(i),
      m_lex_keeper(lex, TRUE)
  {}

  virtual ~sp_instr_jump_if_not()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads);

  
  virtual uint opt_shortcut_jump(sp_head *sp, sp_instr *start)
  {
    return m_ip;
  }

  virtual void opt_move(uint dst, List<sp_instr> *ibp);

  virtual void set_destination(uint old_dest, uint new_dest)
  {
    sp_instr_jump::set_destination(old_dest, new_dest);
    if (m_cont_dest == old_dest)
      m_cont_dest= new_dest;
  }

private:

  Item *m_expr;			
  sp_lex_keeper m_lex_keeper;

}; 


class sp_instr_preturn : public sp_instr
{
  sp_instr_preturn(const sp_instr_preturn &);	
  void operator=(sp_instr_preturn &);

public:

  sp_instr_preturn(uint ip, sp_pcontext *ctx)
    : sp_instr(ip, ctx)
  {}

  virtual ~sp_instr_preturn()
  {}

  virtual int execute(THD *thd, uint *nextp)
  {
    DBUG_ENTER("sp_instr_preturn::execute");
    *nextp= UINT_MAX;
    DBUG_RETURN(0);
  }

  virtual void print(String *str)
  {
    str->append(STRING_WITH_LEN("preturn"));
  }

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads)
  {
    marked= 1;
    return UINT_MAX;
  }

}; 


class sp_instr_freturn : public sp_instr
{
  sp_instr_freturn(const sp_instr_freturn &);	
  void operator=(sp_instr_freturn &);

public:

  sp_instr_freturn(uint ip, sp_pcontext *ctx,
		   Item *val, const Type_handler *handler, LEX *lex)
    : sp_instr(ip, ctx), m_value(val), m_type_handler(handler),
      m_lex_keeper(lex, TRUE)
  {}

  virtual ~sp_instr_freturn()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads)
  {
    marked= 1;
    return UINT_MAX;
  }

protected:

  Item *m_value;
  const Type_handler *m_type_handler;
  sp_lex_keeper m_lex_keeper;

}; 


class sp_instr_hpush_jump : public sp_instr_jump
{
  sp_instr_hpush_jump(const sp_instr_hpush_jump &); 
  void operator=(sp_instr_hpush_jump &);

public:

  sp_instr_hpush_jump(uint ip,
                      sp_pcontext *ctx,
                      sp_handler *handler)
   :sp_instr_jump(ip, ctx),
    m_handler(handler),
    m_opt_hpop(0),
    m_frame(ctx->current_var_count())
  {
    DBUG_ASSERT(m_handler->condition_values.elements == 0);
  }

  virtual ~sp_instr_hpush_jump()
  {
    m_handler->condition_values.empty();
    m_handler= NULL;
  }

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads);

  
  virtual uint opt_shortcut_jump(sp_head *sp, sp_instr *start)
  {
    return m_ip;
  }

  virtual void backpatch(uint dest, sp_pcontext *dst_ctx)
  {
    DBUG_ASSERT(!m_dest || !m_opt_hpop);
    if (!m_dest)
      m_dest= dest;
    else
      m_opt_hpop= dest;
  }

  void add_condition(sp_condition_value *condition_value)
  { m_handler->condition_values.push_back(condition_value); }

  sp_handler *get_handler()
  { return m_handler; }

private:
  
  sp_handler *m_handler;

  
  uint m_opt_hpop;

  
  
  uint m_frame;

}; 


class sp_instr_hpop : public sp_instr
{
  sp_instr_hpop(const sp_instr_hpop &);	
  void operator=(sp_instr_hpop &);

public:

  sp_instr_hpop(uint ip, sp_pcontext *ctx, uint count)
    : sp_instr(ip, ctx), m_count(count)
  {}

  virtual ~sp_instr_hpop()
  {}

  void update_count(uint count)
  {
    m_count= count;
  }

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

private:

  uint m_count;

}; 


class sp_instr_hreturn : public sp_instr_jump
{
  sp_instr_hreturn(const sp_instr_hreturn &);	
  void operator=(sp_instr_hreturn &);

public:

  sp_instr_hreturn(uint ip, sp_pcontext *ctx)
   :sp_instr_jump(ip, ctx),
    m_frame(ctx->current_var_count())
  {}

  virtual ~sp_instr_hreturn()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  
  virtual uint opt_shortcut_jump(sp_head *sp, sp_instr *start)
  {
    return m_ip;
  }

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads);

private:

  uint m_frame;

}; 



class sp_instr_cpush : public sp_instr,
                       public sp_cursor
{
  sp_instr_cpush(const sp_instr_cpush &); 
  void operator=(sp_instr_cpush &);

public:

  sp_instr_cpush(uint ip, sp_pcontext *ctx, LEX *lex, uint offset)
    : sp_instr(ip, ctx), m_lex_keeper(lex, TRUE), m_cursor(offset)
  {}

  virtual ~sp_instr_cpush()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  
  virtual void cleanup_stmt() {  }
private:

  sp_lex_keeper m_lex_keeper;
  uint m_cursor;                

}; 


class sp_instr_cpop : public sp_instr
{
  sp_instr_cpop(const sp_instr_cpop &); 
  void operator=(sp_instr_cpop &);

public:

  sp_instr_cpop(uint ip, sp_pcontext *ctx, uint count)
    : sp_instr(ip, ctx), m_count(count)
  {}

  virtual ~sp_instr_cpop()
  {}

  void update_count(uint count)
  {
    m_count= count;
  }

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

private:

  uint m_count;

}; 


class sp_instr_copen : public sp_instr
{
  sp_instr_copen(const sp_instr_copen &); 
  void operator=(sp_instr_copen &);

public:

  sp_instr_copen(uint ip, sp_pcontext *ctx, uint c)
    : sp_instr(ip, ctx), m_cursor(c)
  {}

  virtual ~sp_instr_copen()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

private:

  uint m_cursor;		

}; 



class sp_instr_cursor_copy_struct: public sp_instr
{
  
  sp_instr_cursor_copy_struct(const sp_instr_cursor_copy_struct &);
  void operator=(sp_instr_cursor_copy_struct &);
  sp_lex_keeper m_lex_keeper;
  uint m_cursor;
  uint m_var;
public:
  sp_instr_cursor_copy_struct(uint ip, sp_pcontext *ctx, uint coffs,
                              sp_lex_cursor *lex, uint voffs)
    : sp_instr(ip, ctx), m_lex_keeper(lex, FALSE),
      m_cursor(coffs),
      m_var(voffs)
  {}
  virtual ~sp_instr_cursor_copy_struct()
  {}
  virtual int execute(THD *thd, uint *nextp);
  virtual int exec_core(THD *thd, uint *nextp);
  virtual void print(String *str);
};


class sp_instr_cclose : public sp_instr
{
  sp_instr_cclose(const sp_instr_cclose &); 
  void operator=(sp_instr_cclose &);

public:

  sp_instr_cclose(uint ip, sp_pcontext *ctx, uint c)
    : sp_instr(ip, ctx), m_cursor(c)
  {}

  virtual ~sp_instr_cclose()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

private:

  uint m_cursor;

}; 


class sp_instr_cfetch : public sp_instr
{
  sp_instr_cfetch(const sp_instr_cfetch &); 
  void operator=(sp_instr_cfetch &);

public:

  sp_instr_cfetch(uint ip, sp_pcontext *ctx, uint c, bool error_on_no_data)
    : sp_instr(ip, ctx), m_cursor(c), m_error_on_no_data(error_on_no_data)
  {
    m_varlist.empty();
  }

  virtual ~sp_instr_cfetch()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  void add_to_varlist(sp_variable *var)
  {
    m_varlist.push_back(var);
  }

private:

  uint m_cursor;
  List<sp_variable> m_varlist;
  bool m_error_on_no_data;

}; 



class sp_instr_agg_cfetch : public sp_instr
{
  sp_instr_agg_cfetch(const sp_instr_cfetch &); 
  void operator=(sp_instr_cfetch &);

public:

  sp_instr_agg_cfetch(uint ip, sp_pcontext *ctx)
    : sp_instr(ip, ctx){}

  virtual ~sp_instr_agg_cfetch()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);
}; 




class sp_instr_error : public sp_instr
{
  sp_instr_error(const sp_instr_error &); 
  void operator=(sp_instr_error &);

public:

  sp_instr_error(uint ip, sp_pcontext *ctx, int errcode)
    : sp_instr(ip, ctx), m_errcode(errcode)
  {}

  virtual ~sp_instr_error()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads)
  {
    marked= 1;
    return UINT_MAX;
  }

private:

  int m_errcode;

}; 


class sp_instr_set_case_expr : public sp_instr_opt_meta
{
public:

  sp_instr_set_case_expr(uint ip, sp_pcontext *ctx, uint case_expr_id,
                         Item *case_expr, LEX *lex)
    : sp_instr_opt_meta(ip, ctx),
      m_case_expr_id(case_expr_id), m_case_expr(case_expr),
      m_lex_keeper(lex, TRUE)
  {}

  virtual ~sp_instr_set_case_expr()
  {}

  virtual int execute(THD *thd, uint *nextp);

  virtual int exec_core(THD *thd, uint *nextp);

  virtual void print(String *str);

  virtual uint opt_mark(sp_head *sp, List<sp_instr> *leads);

  virtual void opt_move(uint dst, List<sp_instr> *ibp);

  virtual void set_destination(uint old_dest, uint new_dest)
  {
    if (m_cont_dest == old_dest)
      m_cont_dest= new_dest;
  }

private:

  uint m_case_expr_id;
  Item *m_case_expr;
  sp_lex_keeper m_lex_keeper;

}; 


#ifndef NO_EMBEDDED_ACCESS_CHECKS
bool
sp_change_security_context(THD *thd, sp_head *sp,
                           Security_context **backup);
void
sp_restore_security_context(THD *thd, Security_context *backup);

bool
set_routine_security_ctx(THD *thd, sp_head *sp, Security_context **save_ctx);
#endif 

TABLE_LIST *
sp_add_to_query_tables(THD *thd, LEX *lex,
		       const LEX_CSTRING *db, const LEX_CSTRING *name,
                       thr_lock_type locktype,
                       enum_mdl_type mdl_type);



#endif 
