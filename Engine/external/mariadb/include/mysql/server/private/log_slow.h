



#ifndef LOG_SLOW_INCLUDED
#define LOG_SLOW_INCLUDED

#define LOG_SLOW_VERBOSITY_INIT           0
#define LOG_SLOW_VERBOSITY_INNODB         (1U << 0)
#define LOG_SLOW_VERBOSITY_QUERY_PLAN     (1U << 1)
#define LOG_SLOW_VERBOSITY_EXPLAIN        (1U << 2)

#define QPLAN_INIT            QPLAN_QC_NO

#define QPLAN_ADMIN           (1U << 0)
#define QPLAN_FILESORT        (1U << 1)
#define QPLAN_FILESORT_DISK   (1U << 2)
#define QPLAN_FILESORT_PRIORITY_QUEUE       (1U << 3)
#define QPLAN_FULL_JOIN       (1U << 4)
#define QPLAN_FULL_SCAN       (1U << 5)
#define QPLAN_NOT_USING_INDEX (1U << 6)
#define QPLAN_QC              (1U << 7)
#define QPLAN_QC_NO           (1U << 8)
#define QPLAN_TMP_TABLE       (1U << 9)
#define QPLAN_TMP_DISK        (1U << 10)


#define QPLAN_STATUS          (1UL << 31) 
#define QPLAN_MAX             (1UL << 31) 


#define LOG_SLOW_DISABLE_ADMIN (1 << 0)
#define LOG_SLOW_DISABLE_CALL  (1 << 1)
#define LOG_SLOW_DISABLE_SLAVE (1 << 2)
#define LOG_SLOW_DISABLE_SP    (1 << 3)


#define LOG_DISABLE_SLAVE (1 << 0)
#define LOG_DISABLE_SP    (1 << 1)

#endif 
