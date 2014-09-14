/*
 * @file main.c
 * @date 14 Sep 2014
 * @author Chester Gillon
 */

#include <stdio.h>
#include <stdint.h>

#include <xdc/runtime/Types.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/c64p/TimestampProvider.h>

Int main (void)
{
    Types_FreqHz timer_freq;
    Types_Timestamp64 now;
    uint64_t start_time, current_time;
    double elapsed_duration_seconds;

    TimestampProvider_getFreq (&timer_freq);

    printf ("Starting timer test\n");

    TimestampProvider_get64 (&now);
    start_time = ((uint64_t) now.hi << 32) + now.lo;

    do
    {
        TimestampProvider_get64 (&now);
        current_time = ((uint64_t) now.hi << 32) + now.lo;
        elapsed_duration_seconds = (double) (current_time - start_time) / timer_freq.lo;
    } while (elapsed_duration_seconds < 10);

    printf ("Timer test complete\n");

    BIOS_start();    /* does not return */
    return(0);
}

