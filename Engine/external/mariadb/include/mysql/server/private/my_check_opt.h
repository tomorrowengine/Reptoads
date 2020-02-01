

#ifndef _my_check_opt_h
#define _my_check_opt_h

#ifdef	__cplusplus
extern "C" {
#endif



#define T_AUTO_INC		(1UL << 0)
#define T_AUTO_REPAIR		(1UL << 1)
#define T_BACKUP_DATA		(1UL << 2)
#define T_CALC_CHECKSUM		(1UL << 3)
#define T_CHECK			(1UL << 4)
#define T_CHECK_ONLY_CHANGED	(1UL << 5)
#define T_CREATE_MISSING_KEYS	(1UL << 6)
#define T_DESCRIPT		(1UL << 7)
#define T_DONT_CHECK_CHECKSUM	(1UL << 8)
#define T_EXTEND		(1UL << 9)
#define T_FAST			(1UL << 10)
#define T_FORCE_CREATE		(1UL << 11)
#define T_FORCE_UNIQUENESS	(1UL << 12)
#define T_INFO			(1UL << 13)

#define T_MEDIUM		(1UL << 14)

#define T_QUICK			(1UL << 15)
#define T_READONLY		(1UL << 16)
#define T_REP			(1UL << 17)
#define T_REP_BY_SORT		(1UL << 18)
#define T_REP_PARALLEL		(1UL << 19)
#define T_RETRY_WITHOUT_QUICK	(1UL << 20)
#define T_SAFE_REPAIR		(1UL << 21)
#define T_SILENT		(1UL << 22)
#define T_SORT_INDEX		(1UL << 23)
#define T_SORT_RECORDS		(1UL << 24)
#define T_STATISTICS		(1UL << 25)
#define T_UNPACK		(1UL << 26)
#define T_UPDATE_STATE		(1UL << 27)
#define T_VERBOSE		(1UL << 28)
#define T_VERY_SILENT		(1UL << 29)
#define T_WAIT_FOREVER		(1UL << 30)
#define T_WRITE_LOOP		(1UL << 31)
#define T_ZEROFILL              (1ULL << 32)
#define T_ZEROFILL_KEEP_LSN     (1ULL << 33)

#define T_NO_CREATE_RENAME_LSN  (1ULL << 34)

#define T_NO_LOCKS		(1ULL << 35)
#define T_CREATE_UNIQUE_BY_SORT (1ULL << 36)
#define T_SUPPRESS_ERR_HANDLING (1ULL << 37)
#define T_FORCE_SORT_MEMORY     (1ULL << 38)

#define T_REP_ANY		(T_REP | T_REP_BY_SORT | T_REP_PARALLEL)

#ifdef	__cplusplus
}
#endif
#endif
