/*
 * measure.h
 * (c) 2013-2016
 * Daniel Prokesch <daniel@vmars.tuwien.ac.at>
 * Benedikt Huber <benedikt@vmars.tuwien.ac.at>
 * WCET Analysis Lab, Real-Time Systems Group,
 * Institute of Computer Engineering
 * Vienna University of Technology
 *
 * Macros for execution time measurement on Patmos.
 * These are copied from the newlib function clock().
 * On Patmos, cycle counter is a 64-bit value.
 */

#ifndef _MEASURE_H_
#define _MEASURE_H_

#include <stdio.h>

typedef volatile unsigned long long cycles_t;


#if defined(__PATMOS__) && defined(PATMOS_ENABLE_MEASURE)


#include <machine/rtc.h>

#define _MEASUREMENT_CLOCK(c,d) do {\
  (c) = get_cpu_cycles() - (d); \
} while (0);

/*
 * Start a measurement
 */
#define MEASUREMENT_START(x) do {\
  _MEASUREMENT_CLOCK((x),0); \
} while (0);

/*
 * Stop a measurement
 */
#define MEASUREMENT_STOP(x) do {\
  _MEASUREMENT_CLOCK((x),(x)); \
} while (0);




#define _S(x) #x
#define _S_(x) _S(x)

/*
 * Print the number of cycles elapsed (including overhead),
 * together with its name and line number information
 */
#define MEASUREMENT_DUMP(x) do {\
  (void) fprintf(stderr, "MEASUREMENT(" __FILE__ ":" _S_(__LINE__)  ") " \
      #x " = %llu\n", (x)); \
} while (0);



#else

/* dummy implementation */
#define MEASUREMENT_START(x) do { x = 0; } while (0)
#define MEASUREMENT_STOP(x) do {} while (0)
#define MEASUREMENT_DUMP(x) do { (void)(x); } while (0)

#endif /* __PATMOS__ */

#endif /* _MEASURE_H_ */
