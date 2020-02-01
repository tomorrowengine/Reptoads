



#ifndef SQL_PRIV_INCLUDED
#define SQL_PRIV_INCLUDED

#ifndef MYSQL_CLIENT


#define WARN_DEPRECATED(Thd,VerHi,VerLo,Old,New)                            \
  do {                                                                      \
    compile_time_assert(MYSQL_VERSION_ID < VerHi * 10000 + VerLo * 100);    \
    if (((THD *) Thd) != NULL)                                              \
      push_warning_printf(((THD *) Thd), Sql_condition::WARN_LEVEL_WARN,    \
                         ER_WARN_DEPRECATED_SYNTAX,                          \
                         ER_THD(((THD *) Thd), ER_WARN_DEPRECATED_SYNTAX), \
                         (Old), (New));                                      \
    else                                                                    \
      sql_print_warning("The syntax '%s' is deprecated and will be removed " \
                        "in a future release. Please use %s instead.",      \
                        (Old), (New));                                      \
  } while(0)




#define WARN_DEPRECATED_NO_REPLACEMENT(Thd,Old)                             \
  do {                                                                      \
    THD *thd_= ((THD*) Thd);                                                \
    if (thd_ != NULL)                                                       \
      push_warning_printf(thd_, Sql_condition::WARN_LEVEL_WARN,             \
                         ER_WARN_DEPRECATED_SYNTAX_NO_REPLACEMENT,          \
                         ER_THD(thd_, ER_WARN_DEPRECATED_SYNTAX_NO_REPLACEMENT), \
                         (Old));                                            \
    else                                                                    \
      sql_print_warning("'%s' is deprecated and will be removed "           \
                        "in a future release.", (Old));                     \
  } while(0)



#endif



#define SELECT_DISTINCT         (1ULL << 0)     
#define SELECT_STRAIGHT_JOIN    (1ULL << 1)     
#define SELECT_DESCRIBE         (1ULL << 2)     
#define SELECT_SMALL_RESULT     (1ULL << 3)     
#define SELECT_BIG_RESULT       (1ULL << 4)     
#define OPTION_FOUND_ROWS       (1ULL << 5)     
#define OPTION_TO_QUERY_CACHE   (1ULL << 6)     
#define SELECT_NO_JOIN_CACHE    (1ULL << 7)     

#define OPTION_AUTOCOMMIT       (1ULL << 8)    
#define OPTION_BIG_SELECTS      (1ULL << 9)     
#define OPTION_LOG_OFF          (1ULL << 10)    
#define OPTION_QUOTE_SHOW_CREATE (1ULL << 11)   
#define TMP_TABLE_ALL_COLUMNS   (1ULL << 12)    
#define OPTION_WARNINGS         (1ULL << 13)    
#define OPTION_AUTO_IS_NULL     (1ULL << 14)    
#define OPTION_NO_CHECK_CONSTRAINT_CHECKS  (1ULL << 15)
#define OPTION_SAFE_UPDATES     (1ULL << 16)    
#define OPTION_BUFFER_RESULT    (1ULL << 17)    
#define OPTION_BIN_LOG          (1ULL << 18)    
#define OPTION_NOT_AUTOCOMMIT   (1ULL << 19)    
#define OPTION_BEGIN            (1ULL << 20)    
#define OPTION_TABLE_LOCK       (1ULL << 21)    
#define OPTION_QUICK            (1ULL << 22)    
#define OPTION_KEEP_LOG         (1ULL << 23)    


#define SELECT_ALL              (1ULL << 24)    
#define OPTION_GTID_BEGIN       (1ULL << 25)    


#define OPTION_NO_FOREIGN_KEY_CHECKS    (1ULL << 26) 

#define OPTION_RELAXED_UNIQUE_CHECKS    (1ULL << 27) 
#define SELECT_NO_UNLOCK                (1ULL << 28) 
#define OPTION_SCHEMA_TABLE             (1ULL << 29) 

#define OPTION_SETUP_TABLES_DONE        (1ULL << 30) 

#define OPTION_SQL_NOTES                (1ULL << 31) 

#define TMP_TABLE_FORCE_MYISAM          (1ULL << 32)
#define OPTION_PROFILING                (1ULL << 33)

#define SELECT_HIGH_PRIORITY            (1ULL << 34)     

#define OPTION_MASTER_SQL_ERROR         (1ULL << 35)

#define OPTION_SKIP_REPLICATION         (1ULL << 37) 
#define OPTION_RPL_SKIP_PARALLEL        (1ULL << 38)
#define OPTION_FOUND_COMMENT            (1ULL << 39) 


#ifndef MYSQL_CLIENT


#define OPTIMIZER_SWITCH_INDEX_MERGE               (1ULL << 0)
#define OPTIMIZER_SWITCH_INDEX_MERGE_UNION         (1ULL << 1)
#define OPTIMIZER_SWITCH_INDEX_MERGE_SORT_UNION    (1ULL << 2)
#define OPTIMIZER_SWITCH_INDEX_MERGE_INTERSECT     (1ULL << 3)
#define OPTIMIZER_SWITCH_INDEX_MERGE_SORT_INTERSECT (1ULL << 4)
#define deprecated_ENGINE_CONDITION_PUSHDOWN       (1ULL << 5)
#define OPTIMIZER_SWITCH_INDEX_COND_PUSHDOWN       (1ULL << 6)
#define OPTIMIZER_SWITCH_DERIVED_MERGE             (1ULL << 7)
#define OPTIMIZER_SWITCH_DERIVED_WITH_KEYS         (1ULL << 8)
#define OPTIMIZER_SWITCH_FIRSTMATCH                (1ULL << 9)
#define OPTIMIZER_SWITCH_LOOSE_SCAN                (1ULL << 10)
#define OPTIMIZER_SWITCH_MATERIALIZATION           (1ULL << 11)
#define OPTIMIZER_SWITCH_IN_TO_EXISTS              (1ULL << 12)
#define OPTIMIZER_SWITCH_SEMIJOIN                  (1ULL << 13)
#define OPTIMIZER_SWITCH_PARTIAL_MATCH_ROWID_MERGE (1ULL << 14)
#define OPTIMIZER_SWITCH_PARTIAL_MATCH_TABLE_SCAN  (1ULL << 15)
#define OPTIMIZER_SWITCH_SUBQUERY_CACHE            (1ULL << 16)

#define OPTIMIZER_SWITCH_MRR                       (1ULL << 17)

#define OPTIMIZER_SWITCH_MRR_COST_BASED            (1ULL << 18)
#define OPTIMIZER_SWITCH_MRR_SORT_KEYS             (1ULL << 19)
#define OPTIMIZER_SWITCH_OUTER_JOIN_WITH_CACHE     (1ULL << 20)
#define OPTIMIZER_SWITCH_SEMIJOIN_WITH_CACHE       (1ULL << 21)
#define OPTIMIZER_SWITCH_JOIN_CACHE_INCREMENTAL    (1ULL << 22)
#define OPTIMIZER_SWITCH_JOIN_CACHE_HASHED         (1ULL << 23)
#define OPTIMIZER_SWITCH_JOIN_CACHE_BKA            (1ULL << 24)
#define OPTIMIZER_SWITCH_OPTIMIZE_JOIN_BUFFER_SIZE (1ULL << 25)
#define OPTIMIZER_SWITCH_TABLE_ELIMINATION         (1ULL << 26)
#define OPTIMIZER_SWITCH_EXTENDED_KEYS             (1ULL << 27)
#define OPTIMIZER_SWITCH_EXISTS_TO_IN              (1ULL << 28)
#define OPTIMIZER_SWITCH_ORDERBY_EQ_PROP           (1ULL << 29)
#define OPTIMIZER_SWITCH_COND_PUSHDOWN_FOR_DERIVED (1ULL << 30)
#define OPTIMIZER_SWITCH_SPLIT_MATERIALIZED        (1ULL << 31)

#define OPTIMIZER_SWITCH_DEFAULT   (OPTIMIZER_SWITCH_INDEX_MERGE | \
                                    OPTIMIZER_SWITCH_INDEX_MERGE_UNION | \
                                    OPTIMIZER_SWITCH_INDEX_MERGE_SORT_UNION | \
                                    OPTIMIZER_SWITCH_INDEX_MERGE_INTERSECT | \
                                    OPTIMIZER_SWITCH_INDEX_COND_PUSHDOWN | \
                                    OPTIMIZER_SWITCH_DERIVED_MERGE | \
                                    OPTIMIZER_SWITCH_DERIVED_WITH_KEYS | \
                                    OPTIMIZER_SWITCH_TABLE_ELIMINATION | \
                                    OPTIMIZER_SWITCH_EXTENDED_KEYS | \
                                    OPTIMIZER_SWITCH_IN_TO_EXISTS | \
                                    OPTIMIZER_SWITCH_MATERIALIZATION | \
                                    OPTIMIZER_SWITCH_PARTIAL_MATCH_ROWID_MERGE|\
                                    OPTIMIZER_SWITCH_PARTIAL_MATCH_TABLE_SCAN|\
                                    OPTIMIZER_SWITCH_OUTER_JOIN_WITH_CACHE | \
                                    OPTIMIZER_SWITCH_SEMIJOIN_WITH_CACHE | \
                                    OPTIMIZER_SWITCH_JOIN_CACHE_INCREMENTAL | \
                                    OPTIMIZER_SWITCH_JOIN_CACHE_HASHED | \
                                    OPTIMIZER_SWITCH_JOIN_CACHE_BKA | \
                                    OPTIMIZER_SWITCH_SUBQUERY_CACHE | \
                                    OPTIMIZER_SWITCH_SEMIJOIN | \
                                    OPTIMIZER_SWITCH_FIRSTMATCH | \
                                    OPTIMIZER_SWITCH_LOOSE_SCAN | \
                                    OPTIMIZER_SWITCH_EXISTS_TO_IN | \
                                    OPTIMIZER_SWITCH_ORDERBY_EQ_PROP | \
                                    OPTIMIZER_SWITCH_COND_PUSHDOWN_FOR_DERIVED | \
                                    OPTIMIZER_SWITCH_SPLIT_MATERIALIZED)





 
#define CONTEXT_ANALYSIS_ONLY_PREPARE 1
 
#define CONTEXT_ANALYSIS_ONLY_VIEW    2

#define CONTEXT_ANALYSIS_ONLY_DERIVED 4
 
#define CONTEXT_ANALYSIS_ONLY_VCOL_EXPR 8




#define UNCACHEABLE_DEPENDENT_GENERATED   1

#define UNCACHEABLE_RAND        2

#define UNCACHEABLE_SIDEEFFECT	4

#define UNCACHEABLE_EXPLAIN     8

#define UNCACHEABLE_UNITED     16
#define UNCACHEABLE_CHECKOPTION 32

#define UNCACHEABLE_DEPENDENT_INJECTED  64

#define UNCACHEABLE_DEPENDENT (UNCACHEABLE_DEPENDENT_GENERATED | \
                               UNCACHEABLE_DEPENDENT_INJECTED)


#define UNDEF_POS (-1)

#define IN_SUBQUERY_CONVERSION_THRESHOLD 1000

#endif 



#define BINLOG_DUMP_NON_BLOCK   1
#define BINLOG_SEND_ANNOTATE_ROWS_EVENT   2

#ifndef MYSQL_CLIENT


#define IS_EQUAL_NO 0
#define IS_EQUAL_YES 1
#define IS_EQUAL_PACK_LENGTH 2

enum enum_parsing_place
{
  NO_MATTER,
  IN_HAVING,
  SELECT_LIST,
  IN_WHERE,
  IN_ON,
  IN_GROUP_BY,
  IN_ORDER_BY,
  IN_UPDATE_ON_DUP_KEY,
  IN_PART_FUNC,
  PARSING_PLACE_SIZE 
};


class sys_var;

enum enum_yes_no_unknown
{
  TVL_YES, TVL_NO, TVL_UNKNOWN
};

#ifdef MYSQL_SERVER





#ifndef DBUG_OFF
extern void turn_parser_debug_on();

#endif



inline int hexchar_to_int(char c)
{
  if (c <= '9' && c >= '0')
    return c-'0';
  c|=32;
  if (c <= 'f' && c >= 'a')
    return c-'a'+10;
  return -1;
}


#define ER_NOT_RW_DIR_PATHSIZE 200

#define IS_TABLESPACES_TABLESPACE_NAME    0
#define IS_TABLESPACES_ENGINE             1
#define IS_TABLESPACES_TABLESPACE_TYPE    2
#define IS_TABLESPACES_LOGFILE_GROUP_NAME 3
#define IS_TABLESPACES_EXTENT_SIZE        4
#define IS_TABLESPACES_AUTOEXTEND_SIZE    5
#define IS_TABLESPACES_MAXIMUM_SIZE       6
#define IS_TABLESPACES_NODEGROUP_ID       7
#define IS_TABLESPACES_TABLESPACE_COMMENT 8

bool db_name_is_in_ignore_db_dirs_list(const char *dbase);

#endif 

#endif 

#endif 
