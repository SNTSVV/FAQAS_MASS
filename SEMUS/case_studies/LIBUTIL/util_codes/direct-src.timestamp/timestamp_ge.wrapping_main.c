
/* Wrapping main template for the function timestamp_ge defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/timestamp.c */
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
    gs_timestamp_t test;
    memset(&base, 0, sizeof(base));
    memset(&test, 0, sizeof(test));
    klee_make_symbolic(&base, sizeof(base), "base"); //gs_timestamp_t
    klee_make_symbolic(&test, sizeof(test), "test"); //gs_timestamp_t

    // Call function under test
    result_faqas_semu = timestamp_ge(&base, &test);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
