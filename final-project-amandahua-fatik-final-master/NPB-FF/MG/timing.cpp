#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <inttypes.h>


#define USEC_TO_SEC(x) ((double)x*1.0e-9)


uint64_t ktiming_getmark(void) {
    struct timespec temp;
    uint64_t nanos;

    int stat = clock_gettime(CLOCK_MONOTONIC , &temp);
    if (stat != 0) {
        perror("ktiming_getmark()");
        exit(-1);
    }
    nanos = temp.tv_nsec;
    nanos += ((uint64_t)temp.tv_sec) * 1000 * 1000 * 1000;
    return nanos;
}

uint64_t ktiming_diff_usec(const uint64_t* const
                           start, const uint64_t* const end) {
    return *end - *start;
}
