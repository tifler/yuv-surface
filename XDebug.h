#ifndef __XDEBUG_H__
#define __XDEBUG_H__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#ifdef  APPEND_NEWLINE
#define __NL        "\n"
#else
#define __NL
#endif  /*APPEND_NEWLINE*/

static inline const char *__basename(const char *path)
{
	const char *tail = strrchr(path, '/');
	return tail ? tail + 1 : path;
}

static inline float clock_gettime_float(void)
{
	float ret = 0.0;
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	ret += tp.tv_sec;
	ret += (tp.tv_nsec / 1000000000.0);
	return ret;
}

#define __dbg_output(fmt, ...)       \
    do { fprintf(stderr, \
            "[%s:%d] " fmt __NL, __func__, __LINE__, ## __VA_ARGS__); } while(0)

#define DIE(fmt, ...)       \
    do { __dbg_output(fmt, ## __VA_ARGS__); assert(0); } while(0)

#ifdef  XDEBUG
// [[XDEBUG
#define ASSERT                  assert
#define DBG                     __dbg_output
#define TRACE_FUNC              \
    do { fprintf(stderr, "[%.3f-%s:%d]\n", \
			clock_gettime_float(), __func__, __LINE__); } while(0)

#define TRACE_LINE              \
    do { fprintf(stderr, "[%.3f-%s:%d]\n", \
			clock_gettime_float(), __basename(__FILE__), __LINE__); } while(0)

#define TRACE_LINE_IF(cond)		\
    do { if (!(cond)) break;	\
		fprintf(stderr, "[%.3f-%s:%d]\n", \
				clock_gettime_float(), __basename(__FILE__), __LINE__); } while(0)

#define ESTIMATE_START(title) \
    do { \
        unsigned int __diff_usec; \
        unsigned int __boundary = 0; \
        const char *__estimate_title = title; \
        struct timeval __s_tv, __e_tv; \
        gettimeofday(&__s_tv, (struct timezone *)0)

#define ESTIMATE_START_BOUND(title, b) \
    do { \
        unsigned int __diff_usec; \
        unsigned int __boundary = b; \
        const char *__estimate_title = title; \
        struct timeval __s_tv, __e_tv; \
        gettimeofday(&__s_tv, (struct timezone *)0)

#define ESTIMATE_STOP() \
        gettimeofday(&__e_tv, (struct timezone *)0); \
        __diff_usec = (__e_tv.tv_sec - __s_tv.tv_sec) * 1000000; \
        __diff_usec += (__e_tv.tv_usec - __s_tv.tv_usec); \
        if(__diff_usec >= __boundary) \
        DBG("ESTIMATED TIME[%s] : %d(usec)\n", \
                __estimate_title, __diff_usec); \
    } while(0)

// ]]XDEBUG
#else
// [[!XDEBUG
#define ASSERT(...)						do { } while (0)
#define DBG(...)						do { } while (0)
#define TRACE_FUNC						do { } while (0)
#define TRACE_LINE						do { } while (0)
#define TRACE_LINE_IF(...)				do { } while (0)
#define ESTIMATE_START(...)				do { } while (0)
#define ESTIMATE_START_BOUND(...)		do { } while (0)
#define ESTIMATE_STOP()					do { } while (0)
// ]]!XDEBUG
#endif  /*XDEBUG*/

#define __CHECK_LINE__					TRACE_LINE
#define __CHECK_FUNC__					TRACE_FUNC
#define __CHECK_LINE_IF__(cond)			TRACE_LINE_IF(cond)

#endif  /*__XDEBUG_H__*/
