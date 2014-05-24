/*
 * Module defining utility MACROs.
 */

/*
 * Debug printing MACRO.
 */
#ifdef DEBUG
#define DEBUG_PRINT(s, ...) do {fprintf(stderr, "DEBUG %s:%d: \n" s "\n", \
				__FILE__, __LINE__, ##__VA_ARGS__);} while (0)
#else
#define DEBUG_PRINT(s, ...) do {} while (0)
#endif

/*
 * MACRO which returns the maximum of two integers. 
 */
#define MAX(a, b) a >= b ? a : b

