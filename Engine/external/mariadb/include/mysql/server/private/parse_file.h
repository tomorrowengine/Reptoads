


#ifndef _PARSE_FILE_H_
#define _PARSE_FILE_H_

#include "sql_string.h"                         
#include "sql_alloc.h"                          

class THD;

typedef struct st_mem_root MEM_ROOT;

#define PARSE_FILE_TIMESTAMPLENGTH 19

enum file_opt_type {
  FILE_OPTIONS_STRING,		
  FILE_OPTIONS_ESTRING,		
  FILE_OPTIONS_ULONGLONG,	
  FILE_OPTIONS_VIEW_ALGO,	
  FILE_OPTIONS_TIMESTAMP,	
  FILE_OPTIONS_STRLIST,         
  FILE_OPTIONS_ULLLIST          
};

struct File_option
{
  LEX_CSTRING name;             
  my_ptrdiff_t offset;          
  file_opt_type type;           
};




class Unknown_key_hook
{
public:
  Unknown_key_hook() {}                       
  virtual ~Unknown_key_hook() {}              
  virtual bool process_unknown_string(const char *&unknown_key, uchar* base,
                                      MEM_ROOT *mem_root, const char *end)= 0;
};




class File_parser_dummy_hook: public Unknown_key_hook
{
public:
  File_parser_dummy_hook() {}                 
  virtual bool process_unknown_string(const char *&unknown_key, uchar* base,
                                      MEM_ROOT *mem_root, const char *end);
};

extern File_parser_dummy_hook file_parser_dummy_hook;

bool get_file_options_ulllist(const char *&ptr, const char *end,
                              const char *line, uchar* base,
                              File_option *parameter,
                              MEM_ROOT *mem_root);

const char *
parse_escaped_string(const char *ptr, const char *end, MEM_ROOT *mem_root,
                     LEX_CSTRING *str);

class File_parser;
File_parser *sql_parse_prepare(const LEX_CSTRING *file_name,
			       MEM_ROOT *mem_root, bool bad_format_errors);

my_bool
sql_create_definition_file(const LEX_CSTRING *dir,
                           const LEX_CSTRING *file_name,
			   const LEX_CSTRING *type,
			   uchar* base, File_option *parameters);
my_bool rename_in_schema_file(THD *thd,
                              const char *schema, const char *old_name,
                              const char *new_db, const char *new_name);

class File_parser: public Sql_alloc
{
  char *start, *end;
  LEX_CSTRING file_type;
  bool content_ok;
public:
  File_parser() :start(0), end(0), content_ok(0)
    { file_type.str= 0; file_type.length= 0; }

  bool ok() { return content_ok; }
  const LEX_CSTRING *type() const { return &file_type; }
  my_bool parse(uchar* base, MEM_ROOT *mem_root,
		struct File_option *parameters, uint required,
                Unknown_key_hook *hook) const;

  friend File_parser *sql_parse_prepare(const LEX_CSTRING *file_name,
					MEM_ROOT *mem_root,
					bool bad_format_errors);
};
#endif 
