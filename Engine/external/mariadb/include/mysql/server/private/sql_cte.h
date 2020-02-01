

#ifndef SQL_CTE_INCLUDED
#define SQL_CTE_INCLUDED
#include "sql_list.h"
#include "sql_lex.h"
#include "sql_select.h"

class select_unit;
struct st_unit_ctxt_elem;




class With_element : public Sql_alloc
{
private:
  With_clause *owner;      
  With_element *next;      
  uint number;  
  table_map elem_map;  
  
  table_map base_dep_map; 
  
  table_map derived_dep_map;
     
  table_map sq_dep_map;
  table_map work_dep_map;  
  
  table_map mutually_recursive;
   
  table_map top_level_dep_map;
  
  TABLE_LIST *sq_rec_ref;
    
  With_element *next_mutually_recursive;
   
  uint references;
  
  LEX_CSTRING unparsed_spec;
  
  my_ptrdiff_t unparsed_spec_offset;

  
  bool stmt_prepare_mode;

  
  table_map get_elem_map() { return (table_map) 1 << number; }
 
public:
  
  LEX_CSTRING *query_name;
  
  List <LEX_CSTRING> column_list;
  
  st_select_lex_unit *spec;
  
  bool is_recursive;
  
  uint rec_outer_references;
    
   
  bool with_anchor;
  
  st_select_lex *first_recursive;
  
  
  uint level;

  
  select_union_recursive *rec_result;

  
  SQL_I_List<Item_subselect> sq_with_rec_ref;
  
  SQL_I_List<TABLE_LIST> derived_with_rec_ref;

  With_element(LEX_CSTRING *name,
               List <LEX_CSTRING> list,
               st_select_lex_unit *unit)
    : next(NULL), base_dep_map(0), derived_dep_map(0),
      sq_dep_map(0), work_dep_map(0), mutually_recursive(0),
      top_level_dep_map(0), sq_rec_ref(NULL),
      next_mutually_recursive(NULL), references(0), 
      query_name(name), column_list(list), spec(unit),
      is_recursive(false), rec_outer_references(0), with_anchor(false),
      level(0), rec_result(NULL)
  { unit->with_element= this; }

  bool check_dependencies_in_spec();
  
  void check_dependencies_in_select(st_select_lex *sl, st_unit_ctxt_elem *ctxt,
                                    bool in_subq, table_map *dep_map);
      
  void check_dependencies_in_unit(st_select_lex_unit *unit,
                                  st_unit_ctxt_elem *ctxt,
                                  bool in_subq,
                                  table_map *dep_map);

  void check_dependencies_in_with_clause(With_clause *with_clause, 
                                         st_unit_ctxt_elem *ctxt,
                                         bool in_subq,
                                         table_map *dep_map);

  void  set_dependency_on(With_element *with_elem)
  { base_dep_map|= with_elem->get_elem_map(); }

  bool check_dependency_on(With_element *with_elem)
  { return base_dep_map & with_elem->get_elem_map(); }

  TABLE_LIST *find_first_sq_rec_ref_in_select(st_select_lex *sel);

  bool set_unparsed_spec(THD *thd, char *spec_start, char *spec_end,
                         my_ptrdiff_t spec_offset);

  st_select_lex_unit *clone_parsed_spec(THD *thd, TABLE_LIST *with_table);

  bool is_referenced() { return references != 0; }

  void inc_references() { references++; }

  bool rename_columns_of_derived_unit(THD *thd, st_select_lex_unit *unit);

  bool prepare_unreferenced(THD *thd);

  bool check_unrestricted_recursive(st_select_lex *sel,
                                    table_map &unrestricted,
                                    table_map &encountered);

  void print(String *str, enum_query_type query_type);

  With_clause *get_owner() { return owner; }

  bool contains_sq_with_recursive_reference()
  { return sq_dep_map & mutually_recursive; }

  bool no_rec_ref_on_top_level()
  { return !(top_level_dep_map & mutually_recursive); }

  table_map get_mutually_recursive() { return mutually_recursive; }

  With_element *get_next_mutually_recursive()
  { return next_mutually_recursive; }

  TABLE_LIST *get_sq_rec_ref() { return sq_rec_ref; }

  bool is_anchor(st_select_lex *sel);

  void move_anchors_ahead(); 

  bool is_unrestricted();

  bool is_with_prepared_anchor();

  void mark_as_with_prepared_anchor();

  bool is_cleaned();

  void mark_as_cleaned();

  void reset_recursive_for_exec();

  void cleanup_stabilized();

  void set_as_stabilized();

  bool is_stabilized();

  bool all_are_stabilized();

  bool instantiate_tmp_tables();

  void prepare_for_next_iteration();

  friend class With_clause;
};

const uint max_number_of_elements_in_with_clause= sizeof(table_map)*8;

 

class With_clause : public Sql_alloc
{
private:
  st_select_lex_unit *owner; 

  
  SQL_I_List<With_element> with_list; 
  
  With_clause *embedding_with_clause;
  
  With_clause *next_with_clause;
  
  bool dependencies_are_checked; 

   
  table_map unrestricted;
  
  table_map with_prepared_anchor;
  table_map cleaned;
  
  table_map stabilized;

public:
 
  bool with_recursive;

  With_clause(bool recursive_fl, With_clause *emb_with_clause)
    : owner(NULL),
      embedding_with_clause(emb_with_clause), next_with_clause(NULL),
      dependencies_are_checked(false),  unrestricted(0),
      with_prepared_anchor(0), cleaned(0), stabilized(0),
      with_recursive(recursive_fl)
  { }

  bool add_with_element(With_element *elem);

  
  void add_to_list(With_clause ** &last_next)
  {
    *last_next= this;
    last_next= &this->next_with_clause;
  }

  void set_owner(st_select_lex_unit *unit) { owner= unit; }

  With_clause *pop() { return embedding_with_clause; }
      
  bool check_dependencies();

  bool check_anchors();

  void move_anchors_ahead();

  With_element *find_table_def(TABLE_LIST *table, With_element *barrier);

  With_element *find_table_def_in_with_clauses(TABLE_LIST *table);

  bool prepare_unreferenced_elements(THD *thd);

  void add_unrestricted(table_map map) { unrestricted|= map; }

  void print(String *str, enum_query_type query_type);

  friend class With_element;

  friend
  bool
  check_dependencies_in_with_clauses(With_clause *with_clauses_list);
};

inline
bool With_element::is_unrestricted() 
{
  return owner->unrestricted & get_elem_map();
}

inline

bool With_element::is_with_prepared_anchor() 
{
  return owner->with_prepared_anchor & get_elem_map();
}

inline
void With_element::mark_as_with_prepared_anchor() 
{
  owner->with_prepared_anchor|= mutually_recursive;
}


inline
bool With_element::is_cleaned() 
{
  return owner->cleaned & get_elem_map();
}


inline
void With_element::mark_as_cleaned() 
{
  owner->cleaned|= get_elem_map();
}


inline
void With_element::reset_recursive_for_exec()
{
  DBUG_ASSERT(is_recursive);
  level= 0;
  owner->with_prepared_anchor&= ~mutually_recursive;
  owner->cleaned&= ~get_elem_map();
  cleanup_stabilized();
  spec->columns_are_renamed= false;
}



inline
void With_element::cleanup_stabilized()
{
  owner->stabilized&= ~mutually_recursive;
}


inline
void With_element::set_as_stabilized()
{
  owner->stabilized|= get_elem_map();
}


inline
bool With_element::is_stabilized()
{
  return owner->stabilized & get_elem_map();
}


inline
bool With_element::all_are_stabilized()
{
  return (owner->stabilized & mutually_recursive) == mutually_recursive;
}


inline
void With_element::prepare_for_next_iteration()
{
  With_element *with_elem= this;
  while ((with_elem= with_elem->get_next_mutually_recursive()) != this)
  {
    TABLE *rec_table= with_elem->rec_result->first_rec_table_to_update;
    if (rec_table)
      rec_table->reginfo.join_tab->preread_init_done= false;        
  }
}


inline
void  st_select_lex_unit::set_with_clause(With_clause *with_cl)
{ 
    with_clause= with_cl;
    if (with_clause)
      with_clause->set_owner(this);
}


inline
void st_select_lex::set_with_clause(With_clause *with_clause)
{
  master_unit()->with_clause= with_clause;
  if (with_clause)
    with_clause->set_owner(master_unit());
}

#endif 
