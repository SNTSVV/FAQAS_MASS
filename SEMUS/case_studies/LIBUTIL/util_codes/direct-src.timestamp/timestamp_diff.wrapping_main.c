
/* Wrapping main template for the function timestamp_diff defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/timestamp.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    gs_timestamp_t base;
    gs_timestamp_t diff;
    memset(&base, 0, sizeof(base));
    memset(&diff, 0, sizeof(diff));
    klee_make_symbolic(&base, sizeof(base), "base"); //gs_timestamp_t
    klee_make_symbolic(&diff, sizeof(diff), "diff"); //gs_timestamp_t

    // Call function under test
    result_faqas_semu = timestamp_diff(&base, &diff);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", base.tv_sec, base.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
