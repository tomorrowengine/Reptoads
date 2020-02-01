#ifndef JSON_LIB_INCLUDED
#define JSON_LIB_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define JSON_DEPTH_LIMIT 32


enum json_errors {
  JE_BAD_CHR= -1,      

  JE_NOT_JSON_CHR= -2, 
                       

  JE_EOS= -3,          

  JE_SYN= -4,          

  JE_STRING_CONST= -5, 

  JE_ESCAPING= -6,     

  JE_DEPTH= -7,        
};


typedef struct st_json_string_t
{
  const uchar *c_str;    
  const uchar *str_end;  
  my_wc_t c_next;        
  int error;             

  CHARSET_INFO *cs;      

  my_charset_conv_mb_wc wc; 
                            
} json_string_t;


void json_string_set_cs(json_string_t *s, CHARSET_INFO *i_cs);
void json_string_set_str(json_string_t *s,
                         const uchar *str, const uchar *end);
#define json_next_char(j) \
  (j)->wc((j)->cs, &(j)->c_next, (j)->c_str, (j)->str_end)
#define json_eos(j) ((j)->c_str >= (j)->str_end)

int json_read_string_const_chr(json_string_t *js);






enum json_path_step_types
{
  JSON_PATH_KEY_NULL=0,
  JSON_PATH_KEY=1,   
  JSON_PATH_ARRAY=2, 
  JSON_PATH_KEY_OR_ARRAY=3,
  JSON_PATH_WILD=4, 
  JSON_PATH_DOUBLE_WILD=8, 
  JSON_PATH_KEY_WILD= 1+4,
  JSON_PATH_KEY_DOUBLEWILD= 1+8,
  JSON_PATH_ARRAY_WILD= 2+4,
  JSON_PATH_ARRAY_DOUBLEWILD= 2+8
};


typedef struct st_json_path_step_t
{
  enum json_path_step_types type;  
                                   
  const uchar *key; 
  const uchar *key_end;  
  uint n_item;      
} json_path_step_t;


typedef struct st_json_path_t
{
  json_string_t s;  
  json_path_step_t steps[JSON_DEPTH_LIMIT]; 
  json_path_step_t *last_step; 

  int mode_strict; 
  enum json_path_step_types types_used; 
} json_path_t;


int json_path_setup(json_path_t *p,
                    CHARSET_INFO *i_cs, const uchar *str, const uchar *end);






enum json_states {
  JST_VALUE,       
  JST_KEY,         
  JST_OBJ_START,   
  JST_OBJ_END,     
  JST_ARRAY_START, 
  JST_ARRAY_END,   
  NR_JSON_USER_STATES
};


enum json_value_types
{
  JSON_VALUE_OBJECT=1,
  JSON_VALUE_ARRAY=2,
  JSON_VALUE_STRING,
  JSON_VALUE_NUMBER,
  JSON_VALUE_TRUE,
  JSON_VALUE_FALSE,
  JSON_VALUE_NULL
};


enum json_num_flags
{
  JSON_NUM_NEG=1,        
  JSON_NUM_FRAC_PART=2,  
  JSON_NUM_EXP=4,        
};


typedef struct st_json_engine_t
{
  json_string_t s;  
  int sav_c_len;    

  int state; 

  
  enum json_value_types value_type; 
  const uchar *value;      
  const uchar *value_begin;
  int value_escaped;       
  uint num_flags;  

  

  const uchar *value_end; 
  int value_len; 
                 

  int stack[JSON_DEPTH_LIMIT]; 
  int stack_p;                 
} json_engine_t;


int json_scan_start(json_engine_t *je,
                        CHARSET_INFO *i_cs, const uchar *str, const uchar *end);
int json_scan_next(json_engine_t *j);




int json_read_keyname_chr(json_engine_t *j);



int json_key_matches(json_engine_t *je, json_string_t *k);



int json_read_value(json_engine_t *j);




int json_skip_key(json_engine_t *j);


typedef const int *json_level_t;


#define json_get_level(j) (j->stack_p)

int json_skip_to_level(json_engine_t *j, int level);


#define json_skip_level(json_engine) \
  json_skip_to_level((json_engine), (json_engine)->stack_p)



int json_skip_level_and_count(json_engine_t *j, int *n_items_skipped);

#define json_skip_array_item json_skip_key


#define json_value_scalar(je)  ((je)->value_type > JSON_VALUE_ARRAY)



int json_find_path(json_engine_t *je,
                   json_path_t *p, json_path_step_t **p_cur_step,
                   uint *array_counters);


typedef struct st_json_find_paths_t
{
  uint n_paths;
  json_path_t *paths;
  uint cur_depth;
  uint *path_depths;
  uint array_counters[JSON_DEPTH_LIMIT];
} json_find_paths_t;


int json_find_paths_first(json_engine_t *je, json_find_paths_t *state,
                          uint n_paths, json_path_t *paths, uint *path_depths);
int json_find_paths_next(json_engine_t *je, json_find_paths_t *state);



int json_unescape(CHARSET_INFO *json_cs,
                  const uchar *json_str, const uchar *json_end,
                  CHARSET_INFO *res_cs,
                  uchar *res, uchar *res_end);


int json_escape(CHARSET_INFO *str_cs, const uchar *str, const uchar *str_end,
                CHARSET_INFO *json_cs, uchar *json, uchar *json_end);



int json_append_ascii(CHARSET_INFO *json_cs,
                      uchar *json, uchar *json_end,
                      const uchar *ascii, const uchar *ascii_end);




int json_get_path_start(json_engine_t *je, CHARSET_INFO *i_cs,
                        const uchar *str, const uchar *end,
                        json_path_t *p);


int json_get_path_next(json_engine_t *je, json_path_t *p);


int json_path_parts_compare(
        const json_path_step_t *a, const json_path_step_t *a_end,
        const json_path_step_t *b, const json_path_step_t *b_end,
        enum json_value_types vt);
int json_path_compare(const json_path_t *a, const json_path_t *b,
                      enum json_value_types vt);


#ifdef  __cplusplus
}
#endif

#endif 

