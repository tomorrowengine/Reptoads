#ifndef UNIREG_INCLUDED
#define UNIREG_INCLUDED




#include <mysql_version.h>                      



#ifndef NO_ALARM_LOOP
#define NO_ALARM_LOOP		
#endif



#define LANGUAGE	"english/"
#define ERRMSG_FILE	"errmsg.sys"
#define TEMP_PREFIX	"MY"
#define LOG_PREFIX	"ML"
#define PROGDIR		"bin/"
#ifndef MYSQL_DATADIR
#define MYSQL_DATADIR		"data/"
#endif
#ifndef SHAREDIR
#define SHAREDIR	"share/"
#endif
#ifndef PLUGINDIR
#define PLUGINDIR	"lib/plugin"
#endif

#define MAX_ERROR_RANGES 4  
#define ERRORS_PER_RANGE 1000

#define DEFAULT_ERRMSGS           my_default_lc_messages->errmsgs->errmsgs
#define CURRENT_THD_ERRMSGS       (current_thd)->variables.errmsgs

#ifndef mysqld_error_find_printf_error_used
#define ER_DEFAULT(X) DEFAULT_ERRMSGS[((X)-ER_ERROR_FIRST) / ERRORS_PER_RANGE][(X)% ERRORS_PER_RANGE]
#define ER_THD(thd,X) ((thd)->variables.errmsgs[((X)-ER_ERROR_FIRST) / ERRORS_PER_RANGE][(X) % ERRORS_PER_RANGE])
#define ER(X)         ER_THD(current_thd, (X))
#endif
#define ER_THD_OR_DEFAULT(thd,X) ((thd) ? ER_THD(thd, (X)) : ER_DEFAULT(X))


#define ME_INFO (ME_HOLDTANG | ME_NOREFRESH)
#define ME_ERROR (ME_BELL | ME_NOREFRESH)
#define MYF_RW MYF(MY_WME+MY_NABP)		

#define SPECIAL_USE_LOCKS	1		
#define SPECIAL_NO_NEW_FUNC	2		
#define SPECIAL_SKIP_SHOW_DB    4               
#define SPECIAL_WAIT_IF_LOCKED	8		
#define SPECIAL_SAME_DB_NAME   16		
#define SPECIAL_ENGLISH        32		
#define SPECIAL_NO_RESOLVE     64		
#define SPECIAL_NO_PRIOR	128		
#define SPECIAL_BIG_SELECTS	256		
#define SPECIAL_NO_HOST_CACHE	512		
#define SPECIAL_SHORT_LOG_FORMAT 1024
#define SPECIAL_SAFE_MODE	2048
#define SPECIAL_LOG_QUERIES_NOT_USING_INDEXES 4096 

	
#define store_record(A,B) memcpy((A)->B,(A)->record[0],(size_t) (A)->s->reclength)
#define restore_record(A,B) memcpy((A)->record[0],(A)->B,(size_t) (A)->s->reclength)
#define cmp_record(A,B) memcmp((A)->record[0],(A)->B,(size_t) (A)->s->reclength)
#define empty_record(A) { \
                          restore_record((A),s->default_values); \
                          bfill((A)->null_flags,(A)->s->null_bytes,255);\
                        }

	

#define READ_ALL               (1 <<  0) 
#define EXTRA_RECORD           (1 <<  3) 
#define DELAYED_OPEN           (1 << 12) 
#define OPEN_VIEW_NO_PARSE     (1 << 14) 

#define OPEN_FRM_FILE_ONLY     (1 << 15)

#define OPEN_TABLE_ONLY        (1 << 16)

#define OPEN_VIEW_ONLY         (1 << 17)

#define OPEN_VIEW_FULL         (1 << 18)

#define OPTIMIZE_I_S_TABLE     (1 << 19)

#define CHECK_METADATA_VERSION (1 << 20)


#define OPEN_TRIGGER_ONLY      (1 << 21)


#define MIN_TURBOBM_PATTERN_LEN 3



#define BIN_LOG_HEADER_SIZE    4 

#define DEFAULT_KEY_CACHE_NAME "default"




#include "mysqld_error.h"
#include "structs.h"				
#include "sql_list.h"                           
#include "field.h"                              


enum extra2_frm_value_type {
  EXTRA2_TABLEDEF_VERSION=0,
  EXTRA2_DEFAULT_PART_ENGINE=1,
  EXTRA2_GIS=2,
  EXTRA2_PERIOD_FOR_SYSTEM_TIME=4,

#define EXTRA2_ENGINE_IMPORTANT 128

  EXTRA2_ENGINE_TABLEOPTS=128,
  EXTRA2_FIELD_FLAGS=129
};

enum extra2_field_flags {
  VERS_OPTIMIZED_UPDATE= 1 << INVISIBLE_MAX_BITS
};

int rea_create_table(THD *thd, LEX_CUSTRING *frm,
                     const char *path, const char *db, const char *table_name,
                     HA_CREATE_INFO *create_info, handler *file,
                     bool no_ha_create_table);
LEX_CUSTRING build_frm_image(THD *thd, const LEX_CSTRING *table,
                             HA_CREATE_INFO *create_info,
                             List<Create_field> &create_fields,
                             uint keys, KEY *key_info, handler *db_file);

#define FRM_HEADER_SIZE 64
#define FRM_FORMINFO_SIZE 288
#define FRM_MAX_SIZE (1024*1024)

static inline bool is_binary_frm_header(uchar *head)
{
  return head[0] == 254
      && head[1] == 1
      && head[2] >= FRM_VER
      && head[2] <= FRM_VER_CURRENT;
}

#endif
