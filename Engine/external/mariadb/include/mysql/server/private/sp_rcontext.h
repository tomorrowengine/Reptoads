


#ifndef _SP_RCONTEXT_H_
#define _SP_RCONTEXT_H_

#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_class.h"                    
#include "sp_pcontext.h"                  





class sp_cursor;
class sp_lex_keeper;
class sp_instr_cpush;
class sp_instr_hpush_jump;
class Query_arena;
class sp_head;
class Item_cache;
class Virtual_tmp_table;




class sp_rcontext : public Sql_alloc
{
public:
  
  
  
  
  
  
  
  
  
  
  static sp_rcontext *create(THD *thd,
                             const sp_head *owner,
                             const sp_pcontext *root_parsing_ctx,
                             Field *return_value_fld,
                             Row_definition_list &defs);

  ~sp_rcontext();

private:
  sp_rcontext(const sp_head *owner,
              const sp_pcontext *root_parsing_ctx,
              Field *return_value_fld,
              bool in_sub_stmt);

  
  sp_rcontext(const sp_rcontext &);
  void operator=(sp_rcontext &);

public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class Sql_condition_info : public Sql_alloc,
                             public Sql_condition_identity
  {
  public:
    
    char *message;

    
    
    
    
    Sql_condition_info(const Sql_condition *_sql_condition,
                       Query_arena *arena)
      :Sql_condition_identity(*_sql_condition)
    {
      message= strdup_root(arena->mem_root, _sql_condition->get_message_text());
    }
  };

private:
  
  
  
  class Handler_call_frame : public Sql_alloc
  {
  public:
    
    const Sql_condition_info *sql_condition;

    
    
    uint continue_ip;

    
    
    
    
    Handler_call_frame(const Sql_condition_info *_sql_condition,
                       uint _continue_ip)
     :sql_condition(_sql_condition),
      continue_ip(_continue_ip)
    { }
 };

public:
  
  
  
  Query_arena *callers_arena;

  
  
  
  bool end_partial_result_set;
  bool pause_state;
  bool quit_func;
  uint instr_ptr;

  
  
  
  
  const sp_head *m_sp;

  
  
  

  uint argument_count() const
  {
    return m_root_parsing_ctx->context_var_count();
  }

  int set_variable(THD *thd, uint var_idx, Item **value);
  int set_variable_row_field(THD *thd, uint var_idx, uint field_idx,
                             Item **value);
  int set_variable_row_field_by_name(THD *thd, uint var_idx,
                                     const LEX_CSTRING &field_name,
                                     Item **value);
  int set_variable_row(THD *thd, uint var_idx, List<Item> &items);

  int set_parameter(THD *thd, uint var_idx, Item **value)
  {
    DBUG_ASSERT(var_idx < argument_count());
    return set_variable(thd, var_idx, value);
  }

  Item_field *get_variable(uint var_idx) const
  { return m_var_items[var_idx]; }

  Item **get_variable_addr(uint var_idx) const
  { return ((Item **) m_var_items.array()) + var_idx; }

  Item_field *get_parameter(uint var_idx) const
  {
    DBUG_ASSERT(var_idx < argument_count());
    return get_variable(var_idx);
  }

  bool find_row_field_by_name_or_error(uint *field_idx, uint var_idx,
                                       const LEX_CSTRING &field_name);

  bool set_return_value(THD *thd, Item **return_value_item);

  bool is_return_value_set() const
  { return m_return_value_set; }

  
  
  

  
  
  
  
  
  
  
  bool push_handler(sp_instr_hpush_jump *entry);

  
  
  
  
  void pop_handlers(size_t count);

  const Sql_condition_info *raised_condition() const
  {
    return m_handler_call_stack.elements() ?
      (*m_handler_call_stack.back())->sql_condition : NULL;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool handle_sql_condition(THD *thd,
                            uint *ip,
                            const sp_instr *cur_spi);

  
  
  
  
  
  uint exit_handler(Diagnostics_area *da);

  
  
  

  
  
  
  
  void push_cursor(sp_cursor *cur);

  void pop_cursor(THD *thd);
  
  
  
  void pop_cursors(THD *thd, size_t count);

  void pop_all_cursors(THD *thd)
  { pop_cursors(thd, m_ccount); }

  sp_cursor *get_cursor(uint i) const
  { return m_cstack[i]; }

  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool set_case_expr(THD *thd, int case_expr_id, Item **case_expr_item_ptr);

  Item *get_case_expr(int case_expr_id) const
  { return m_case_expr_holders[case_expr_id]; }

  Item ** get_case_expr_addr(int case_expr_id) const
  { return (Item**) m_case_expr_holders.array() + case_expr_id; }

private:
  
  
  
  
  
  bool alloc_arrays(THD *thd);

  
  
  
  
  
  
  
  bool init_var_table(THD *thd, List<Spvar_definition> &defs);

  
  
  
  
  
  
  
  bool init_var_items(THD *thd, List<Spvar_definition> &defs);

  
  
  
  
  
  
  
  
  
  
  Item_cache *create_case_expr_holder(THD *thd, const Item *item) const;

  Virtual_tmp_table *virtual_tmp_table_for_row(uint idx);

private:
  
  const sp_pcontext *m_root_parsing_ctx;

  
  Virtual_tmp_table *m_var_table;

  
  
  Bounds_checked_array<Item_field *> m_var_items;

  
  
  Field *m_return_value_fld;

  
  
  bool m_return_value_set;

  
  bool m_in_sub_stmt;

  
  Dynamic_array<sp_instr_hpush_jump *> m_handlers;

  
  Dynamic_array<Handler_call_frame *> m_handler_call_stack;

  
  Bounds_checked_array<sp_cursor *> m_cstack;

  
  uint m_ccount;

  
  Bounds_checked_array<Item_cache *> m_case_expr_holders;
}; 

#endif 
