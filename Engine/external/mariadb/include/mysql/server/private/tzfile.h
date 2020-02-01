#ifndef TZFILE_INCLUDED
#define TZFILE_INCLUDED







#ifndef TZDIR
#define TZDIR	"/usr/share/zoneinfo" 
#endif 



#define	TZ_MAGIC	"TZif"

struct tzhead {
 	uchar	tzh_magic[4];		
	uchar	tzh_reserved[16];	
	uchar	tzh_ttisgmtcnt[4];	
	uchar	tzh_ttisstdcnt[4];	
	uchar	tzh_leapcnt[4];		
	uchar	tzh_timecnt[4];		
	uchar	tzh_typecnt[4];		
	uchar	tzh_charcnt[4];		
};





#ifndef TZ_MAX_TIMES

#define TZ_MAX_TIMES	370
#endif 

#ifndef TZ_MAX_TYPES
#ifdef SOLAR
#define TZ_MAX_TYPES	256 
#else

#define TZ_MAX_TYPES	20	
#endif 
#endif 

#ifndef TZ_MAX_CHARS
#define TZ_MAX_CHARS	50	
				
#endif 

#ifndef TZ_MAX_LEAPS
#define TZ_MAX_LEAPS	50	
#endif 

#ifndef TZ_MAX_REV_RANGES
#ifdef SOLAR

#define TZ_MAX_REV_RANGES (TZ_MAX_TIMES*2+TZ_MAX_LEAPS*2+2)
#else
#define TZ_MAX_REV_RANGES (TZ_MAX_TIMES+TZ_MAX_LEAPS+2)
#endif
#endif

#define SECS_PER_MIN	60
#define MINS_PER_HOUR	60
#define HOURS_PER_DAY	24
#define DAYS_PER_WEEK	7
#define DAYS_PER_NYEAR	365
#define DAYS_PER_LYEAR	366
#define SECS_PER_HOUR	(SECS_PER_MIN * MINS_PER_HOUR)
#define SECS_PER_DAY	((long) SECS_PER_HOUR * HOURS_PER_DAY)
#define MONS_PER_YEAR	12

#define TM_YEAR_BASE	1900

#define EPOCH_YEAR	1970



#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

#endif
