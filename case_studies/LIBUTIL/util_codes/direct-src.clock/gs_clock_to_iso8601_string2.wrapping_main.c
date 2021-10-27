
/* Wrapping main template for the function gs_clock_to_iso8601_string2 defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/clock.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    enum gs_error_t result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned int utc_sec;
    char buf[GS_CLOCK_ISO8601_BUFFER_LENGTH];
    //unsigned long buf_size;
    memset(&utc_sec, 0, sizeof(utc_sec));
    //memset(&buf_size, 0, sizeof(buf_size));
    klee_make_symbolic(&utc_sec, sizeof(utc_sec), "utc_sec"); //unsigned int
    //klee_make_symbolic(&buf_size, sizeof(buf_size), "buf_size"); //unsigned long

    // Call function under test
    result_faqas_semu = gs_clock_to_iso8601_string2(utc_sec, buf, GS_CLOCK_ISO8601_BUFFER_LENGTH);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: buf = %d\n", buf);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
