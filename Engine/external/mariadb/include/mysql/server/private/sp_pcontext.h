


#ifndef _SP_PCONTEXT_H_
#define _SP_PCONTEXT_H_

#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_string.h"                         
#include "mysql_com.h"                          
#include "field.h"                              
#include "sql_array.h"                          





class sp_variable : public Sql_alloc
{
public:
  enum enum_mode
  {
    MODE_IN,
    MODE_OUT,
    MODE_INOUT
  };

  
  LEX_CSTRING name;

  
  enum_mode mode;

  
  
  
  
  
  uint offset;

  
  Item *default_value;

  
  Spvar_definition field_def;

  
  const Type_handler *type_handler() const { return field_def.type_handler(); }

public:
  sp_variable(const LEX_CSTRING *name_arg, uint offset_arg)
   :Sql_alloc(),
    name(*name_arg),
    mode(MODE_IN),
    offset(offset_arg),
    default_value(NULL)
  { }
  
  const Spvar_definition *find_row_field(const LEX_CSTRING *var_name,
                                         const LEX_CSTRING *field_name,
                                         uint *row_field_offset);
};











class sp_label : public Sql_alloc
{
public:
  enum enum_type
  {
    
    IMPLICIT,

    
    BEGIN,

    
    ITERATION,

    
    GOTO
  };

  
  LEX_CSTRING name;

  
  uint ip;

  
  enum_type type;

  
  class sp_pcontext *ctx;

public:
  sp_label(const LEX_CSTRING *_name,
           uint _ip, enum_type _type, sp_pcontext *_ctx)
   :Sql_alloc(),
    name(*_name),
    ip(_ip),
    type(_type),
    ctx(_ctx)
  { }
};











class sp_condition_value : public Sql_alloc, public Sql_state_errno
{
  bool m_is_user_defined;
public:
  enum enum_type
  {
    ERROR_CODE,
    SQLSTATE,
    WARNING,
    NOT_FOUND,
    EXCEPTION
  };

  
  enum_type type;

public:
  sp_condition_value(uint _mysqlerr)
   :Sql_alloc(),
    Sql_state_errno(_mysqlerr),
    m_is_user_defined(false),
    type(ERROR_CODE)
  { }

  sp_condition_value(uint _mysqlerr, const char *_sql_state)
   :Sql_alloc(),
    Sql_state_errno(_mysqlerr, _sql_state),
    m_is_user_defined(false),
    type(ERROR_CODE)
  { }

  sp_condition_value(const char *_sql_state, bool is_user_defined= false)
   :Sql_alloc(),
    Sql_state_errno(0, _sql_state),
    m_is_user_defined(is_user_defined),
    type(SQLSTATE)
  { }

  sp_condition_value(enum_type _type)
   :Sql_alloc(),
    m_is_user_defined(false),
    type(_type)
  {
    DBUG_ASSERT(type != ERROR_CODE && type != SQLSTATE);
  }

  
  
  
  
  
  bool equals(const sp_condition_value *cv) const;


  
  bool matches(const Sql_condition_identity &identity,
               const sp_condition_value *found_cv) const;

  Sql_user_condition_identity get_user_condition_identity() const
  {
    return Sql_user_condition_identity(m_is_user_defined ? this : NULL);
  }
};


class sp_condition_value_user_defined: public sp_condition_value
{
public:
  sp_condition_value_user_defined()
   :sp_condition_value("45000", true)
  { }
};







class sp_condition : public Sql_alloc
{
public:
  
  LEX_CSTRING name;

  
  sp_condition_value *value;

public:
  sp_condition(const LEX_CSTRING *name_arg, sp_condition_value *value_arg)
   :Sql_alloc(),
    name(*name_arg),
    value(value_arg)
  { }
  sp_condition(const char *name_arg, size_t name_length_arg,
               sp_condition_value *value_arg)
   :value(value_arg)
  {
    name.str=    name_arg;
    name.length= name_length_arg;
  }
  bool eq_name(const LEX_CSTRING *str) const
  {
    return my_strnncoll(system_charset_info,
                        (const uchar *) name.str, name.length,
                        (const uchar *) str->str, str->length) == 0;
  }
};





class sp_pcursor: public LEX_CSTRING
{
  class sp_pcontext *m_param_context; 
  class sp_lex_cursor *m_lex;         
public:
  sp_pcursor(const LEX_CSTRING *name, class sp_pcontext *param_ctx,
             class sp_lex_cursor *lex)
   :LEX_CSTRING(*name), m_param_context(param_ctx), m_lex(lex)
  { }
  class sp_pcontext *param_context() const { return m_param_context; }
  class sp_lex_cursor *lex() const { return m_lex; }
  bool check_param_count_with_error(uint param_count) const;
};






class sp_handler : public Sql_alloc
{
public:
  
  
  enum enum_type
  {
    EXIT,
    CONTINUE
  };

  
  enum_type type;

  
  List<sp_condition_value> condition_values;

public:
  
  
  
  sp_handler(enum_type _type)
   :Sql_alloc(),
    type(_type)
  { }
};


























class sp_pcontext : public Sql_alloc
{
public:
  enum enum_scope
  {
    
    REGULAR_SCOPE,

    
    HANDLER_SCOPE
  };

  class Lex_for_loop: public Lex_for_loop_st
  {
  public:
    Lex_for_loop() { init(); }
  };

public:
  sp_pcontext();
  ~sp_pcontext();


  

  
  
  
  sp_pcontext *push_context(THD *thd, enum_scope scope);

  
  
  sp_pcontext *pop_context();

  sp_pcontext *parent_context() const
  { return m_parent; }

  sp_pcontext *child_context(uint i) const
  { return i < m_children.elements() ? m_children.at(i) : NULL; }

  
  
  
  
  
  
  
  
  
  
  uint diff_handlers(const sp_pcontext *ctx, bool exclusive) const;

  
  
  
  
  
  
  
  
  
  
  uint diff_cursors(const sp_pcontext *ctx, bool exclusive) const;

  
  
  

  
  
  
  uint max_var_index() const
  { return m_max_var_index; }

  
  
  uint current_var_count() const
  { return m_var_offset + (uint)m_vars.elements(); }

  
  uint context_var_count() const
  { return (uint)m_vars.elements(); }

  
  sp_variable *get_context_variable(uint i) const
  {
    DBUG_ASSERT(i < m_vars.elements());
    return m_vars.at(i);
  }

  
  sp_variable *get_last_context_variable(uint i= 0) const
  {
    DBUG_ASSERT(i < m_vars.elements());
    return m_vars.at(m_vars.elements() - i - 1);
  }

  
  
  
  
  
  
  sp_variable *add_variable(THD *thd, const LEX_CSTRING *name);

  
  
  
  
  void retrieve_field_definitions(List<Spvar_definition> *field_def_lst) const;

  
  
  
  
  
  
  
  
  
  
  
  sp_variable *find_variable(const LEX_CSTRING *name, bool current_scope_only) const;

  
  
  
  
  
  
  
  
  
  
  sp_variable *find_variable(uint offset) const;

  
  
  
  void declare_var_boundary(uint n)
  { m_pboundary= n; }

  
  
  

  int register_case_expr()
  { return m_num_case_exprs++; }

  int get_num_case_exprs() const
  { return m_num_case_exprs; }

  bool push_case_expr_id(int case_expr_id)
  { return m_case_expr_ids.append(case_expr_id); }

  void pop_case_expr_id()
  { m_case_expr_ids.pop(); }

  int get_current_case_expr_id() const
  { return *m_case_expr_ids.back(); }

  
  
  

  sp_label *push_label(THD *thd, const LEX_CSTRING *name, uint ip,
                       sp_label::enum_type type, List<sp_label> * list);

  sp_label *push_label(THD *thd, const LEX_CSTRING *name, uint ip,
                       sp_label::enum_type type)
  { return push_label(thd, name, ip, type, &m_labels); }

  sp_label *push_goto_label(THD *thd, const LEX_CSTRING *name, uint ip,
                            sp_label::enum_type type)
  { return push_label(thd, name, ip, type, &m_goto_labels); }

  sp_label *push_label(THD *thd, const LEX_CSTRING *name, uint ip)
  { return push_label(thd, name, ip, sp_label::IMPLICIT); }

  sp_label *push_goto_label(THD *thd, const LEX_CSTRING *name, uint ip)
  { return push_goto_label(thd, name, ip, sp_label::GOTO); }

  sp_label *find_label(const LEX_CSTRING *name);

  sp_label *find_goto_label(const LEX_CSTRING *name, bool recusive);

  sp_label *find_goto_label(const LEX_CSTRING *name)
  { return find_goto_label(name, true); }

  sp_label *find_label_current_loop_start();

  sp_label *last_label()
  {
    sp_label *label= m_labels.head();

    if (!label && m_parent)
      label= m_parent->last_label();

    return label;
  }

  sp_label *last_goto_label()
  {
    return m_goto_labels.head();
  }

  sp_label *pop_label()
  { return m_labels.pop(); }

  bool block_label_declare(LEX_CSTRING *label)
  {
    sp_label *lab= find_label(label);
    if (lab)
    {
      my_error(ER_SP_LABEL_REDEFINE, MYF(0), label->str);
      return true;
    }
    return false;
  }

  
  
  

  bool add_condition(THD *thd, const LEX_CSTRING *name,
                               sp_condition_value *value);

  
  sp_condition_value *find_condition(const LEX_CSTRING *name,
                                     bool current_scope_only) const;

  sp_condition_value *
  find_declared_or_predefined_condition(THD *thd, const LEX_CSTRING *name) const;

  bool declare_condition(THD *thd, const LEX_CSTRING *name,
                                   sp_condition_value *val)
  {
    if (find_condition(name, true))
    {
      my_error(ER_SP_DUP_COND, MYF(0), name->str);
      return true;
    }
    return add_condition(thd, name, val);
  }

  
  
  

  sp_handler *add_handler(THD* thd, sp_handler::enum_type type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool check_duplicate_handler(const sp_condition_value *cond_value) const;

  
  
  
  
  
  
  
  sp_handler *find_handler(const Sql_condition_identity &identity) const;

  
  
  

  bool add_cursor(const LEX_CSTRING *name, sp_pcontext *param_ctx,
                  class sp_lex_cursor *lex);

  
  const sp_pcursor *find_cursor(const LEX_CSTRING *name,
                                uint *poff, bool current_scope_only) const;

  const sp_pcursor *find_cursor_with_error(const LEX_CSTRING *name,
                                           uint *poff,
                                           bool current_scope_only) const
  {
    const sp_pcursor *pcursor= find_cursor(name, poff, current_scope_only);
    if (!pcursor)
    {
      my_error(ER_SP_CURSOR_MISMATCH, MYF(0), name->str);
      return NULL;
    }
    return pcursor;
  }
  
  const sp_pcursor *find_cursor(uint offset) const;

  const sp_pcursor *get_cursor_by_local_frame_offset(uint offset) const
  { return &m_cursors.at(offset); }

  uint cursor_offset() const
  { return m_cursor_offset; }

  uint frame_cursor_count() const
  { return (uint)m_cursors.elements(); }

  uint max_cursor_index() const
  { return m_max_cursor_index + (uint)m_cursors.elements(); }

  uint current_cursor_count() const
  { return m_cursor_offset + (uint)m_cursors.elements(); }

  void set_for_loop(const Lex_for_loop_st &for_loop)
  {
    m_for_loop.init(for_loop);
  }
  const Lex_for_loop_st &for_loop()
  {
    return m_for_loop;
  }

private:
  
  
  
  sp_pcontext(sp_pcontext *prev, enum_scope scope);

  void init(uint var_offset, uint cursor_offset, int num_case_expressions);

  
  sp_pcontext(const sp_pcontext &);
  void operator=(sp_pcontext &);

  sp_condition_value *find_predefined_condition(const LEX_CSTRING *name) const;

private:
  
  
  
  
  
  
  
  uint m_max_var_index;

  
  uint m_max_cursor_index;

  
  sp_pcontext *m_parent;

  
  
  
  
  
  
  uint m_var_offset;

  
  uint m_cursor_offset;

  
  
  
  
  uint m_pboundary;

  int m_num_case_exprs;

  
  Dynamic_array<sp_variable *> m_vars;

  
  Dynamic_array<int> m_case_expr_ids;

  
  Dynamic_array<sp_condition *> m_conditions;

  
  Dynamic_array<sp_pcursor> m_cursors;

  
  Dynamic_array<sp_handler *> m_handlers;

  
  
  List<sp_label> m_labels;
  
  List<sp_label> m_goto_labels;

  
  Dynamic_array<sp_pcontext *> m_children;

  
  enum_scope m_scope;

  
  Lex_for_loop m_for_loop;
}; 


#endif 
