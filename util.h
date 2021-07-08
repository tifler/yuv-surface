#ifndef	__UTIL_H__
#define	__UTIL_H__

/*!
 * @macros ROUND_UP(x,d)
 * @brief round up func.
 */
#ifndef ROUND_UP
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )
#endif

/*!
 * @macros ROUND_DOWN(x,d)
 * @brief round down func.
 */
#ifndef ROUND_DOWN
#define ROUND_DOWN(x, n)    ((x) & ~((n) - 1))
#endif

/*!
 * @macros MIN(a, b)
 * @brief return minimum value of a and b.
 */
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*!
 * @macros MAX(a, b)
 * @brief return maximum value of a and b.
 */
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif


#endif	/*__UTIL_H__*/
