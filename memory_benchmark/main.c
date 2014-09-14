/*
 * @file main.c
 * @date 13 Sep 2014
 * @author Chester Gillon
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <xdc/runtime/Types.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/sysbios/family/c64p/TimestampProvider.h>

#define MIN_LOG2N_BUFFER_SIZE 10
#define MAX_LOG2N_BUFFER_SIZE 25
#define MAX_BUFFER_SIZE (1 << MAX_LOG2N_BUFFER_SIZE)
#define NUM_BUFFER_SIZES (MAX_LOG2N_BUFFER_SIZE - MIN_LOG2N_BUFFER_SIZE + 1)

#define NUM_TIMED_ITERATIONS 1000

static void time_buffer_write (char *const buffer, size_t size,
                               Types_Timestamp64 *const start_time, Types_Timestamp64 *const stop_time)
{
    int iteration;

    TimestampProvider_get64 (start_time);
    for (iteration = 0; iteration < NUM_TIMED_ITERATIONS; iteration++)
    {
        memset (buffer, 0, size);
    }
    TimestampProvider_get64 (stop_time);
}

Int main (void)
{
    char *buffer;
    Cache_Size cache_size;
    Types_FreqHz timer_freq;
    Types_Timestamp64 start_times[NUM_BUFFER_SIZES], stop_times[NUM_BUFFER_SIZES];
    int buffer_size_index;
    double total_mbytes, test_duration, total_duration;
    uint64_t start_time, stop_time;
    size_t buffer_size;

    TimestampProvider_getFreq (&timer_freq);
    Cache_getSize (&cache_size);

    buffer = malloc (MAX_BUFFER_SIZE);

    printf ("Starting memory write test\n");
    for (buffer_size_index = 0; buffer_size_index < NUM_BUFFER_SIZES; buffer_size_index++)
    {
        time_buffer_write (buffer, 1 << (buffer_size_index + MIN_LOG2N_BUFFER_SIZE),
                           &start_times[buffer_size_index], &stop_times[buffer_size_index]);
    }

    total_duration = 0.0;
    for (buffer_size_index = 0; buffer_size_index < NUM_BUFFER_SIZES; buffer_size_index++)
    {
        buffer_size = (1 << (buffer_size_index + MIN_LOG2N_BUFFER_SIZE));
        total_mbytes = ((double) NUM_TIMED_ITERATIONS * (double) buffer_size) / (1024.0 * 1024.0);
        start_time = ((uint64_t) start_times[buffer_size_index].hi << 32) + start_times[buffer_size_index].lo;
        stop_time = ((uint64_t) stop_times[buffer_size_index].hi << 32) + stop_times[buffer_size_index].lo;
        test_duration = (double) (stop_time - start_time) / (double) timer_freq.lo;
        printf ("With buffer size of %u wrote %.1f Mbytes over %.6f seconds, or %.1f Mbytes/sec\n",
                buffer_size, total_mbytes, test_duration, total_mbytes / test_duration);
        total_duration += test_duration;
    }
    printf ("Total test duration = %.6f seconds\n", total_duration);

    BIOS_start();    /* does not return */
    return(0);
}

