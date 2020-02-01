#ifndef ITEM_XMLFUNC_INCLUDED
#define ITEM_XMLFUNC_INCLUDED







typedef struct my_xml_node_st MY_XML_NODE;


class Item_xml_str_func: public Item_str_func
{
protected:
  
  class XML
  {
    bool m_cached;
    String *m_raw_ptr;   
    String m_raw_buf;    
    String m_parsed_buf; 
    bool parse();
    void reset()
    {
      m_cached= false;
      m_raw_ptr= (String *) 0;
    }
  public:
    XML() { reset(); }
    void set_charset(CHARSET_INFO *cs) { m_parsed_buf.set_charset(cs); }
    String *raw() { return m_raw_ptr; }
    String *parsed() { return &m_parsed_buf; }
    const MY_XML_NODE *node(uint idx);
    bool cached() { return m_cached; }
    bool parse(String *raw, bool cache);
    bool parse(Item *item, bool cache)
    {
      String *res;
      if (!(res= item->val_str(&m_raw_buf)))
      {
        m_raw_ptr= (String *) 0;
        m_cached= cache;
        return true;
      }
      return parse(res, cache);
    }
  };
  String m_xpath_query; 
  Item *nodeset_func;
  XML xml;
  bool get_xml(XML *xml_arg, bool cache= false)
  {
    if (!cache && xml_arg->cached())
      return xml_arg->raw() == 0;
    return xml_arg->parse(args[0], cache);
  }
public:
  Item_xml_str_func(THD *thd, Item *a, Item *b): Item_str_func(thd, a, b)
  {
    maybe_null= TRUE;
  }
  Item_xml_str_func(THD *thd, Item *a, Item *b, Item *c):
    Item_str_func(thd, a, b, c)
  {
    maybe_null= TRUE;
  }
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec();
  bool const_item() const
  {
    return const_item_cache && (!nodeset_func || nodeset_func->const_item());
  }
};


class Item_func_xml_extractvalue: public Item_xml_str_func
{
public:
  Item_func_xml_extractvalue(THD *thd, Item *a, Item *b):
    Item_xml_str_func(thd, a, b) {}
  const char *func_name() const { return "extractvalue"; }
  String *val_str(String *);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_xml_extractvalue>(thd, this); }
};


class Item_func_xml_update: public Item_xml_str_func
{
  String tmp_value2, tmp_value3;
  bool collect_result(String *str,
                      const MY_XML_NODE *cut,
                      const String *replace);
public:
  Item_func_xml_update(THD *thd, Item *a, Item *b, Item *c):
    Item_xml_str_func(thd, a, b, c) {}
  const char *func_name() const { return "updatexml"; }
  String *val_str(String *);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_xml_update>(thd, this); }
};

#endif 
