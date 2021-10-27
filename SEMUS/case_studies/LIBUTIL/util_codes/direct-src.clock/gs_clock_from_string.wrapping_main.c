
/* Wrapping main template for the function gs_clock_from_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/clock.c */
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
    char str[GS_CLOCK_ISO8601_BUFFER_LENGTH];
    gs_timestamp_t ts;
    memset(&str, 0, sizeof(str));
    klee_make_symbolic(&str, sizeof(str), "str"); //char

    // Call function under test
    result_faqas_semu = gs_clock_from_string(str, &ts);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", ts.tv_sec, ts.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
