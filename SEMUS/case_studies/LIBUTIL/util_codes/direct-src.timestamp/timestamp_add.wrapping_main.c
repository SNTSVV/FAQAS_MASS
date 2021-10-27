
/* Wrapping main template for the function timestamp_add defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/timestamp.c */
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
    gs_timestamp_t add;
    memset(&base, 0, sizeof(base));
    memset(&add, 0, sizeof(add));
    klee_make_symbolic(&base, sizeof(base), "base"); //gs_timestamp_t
    klee_make_symbolic(&add, sizeof(add), "add"); //gs_timestamp_t

    // Call function under test
    result_faqas_semu = timestamp_add(&base, &add);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", base.tv_sec, base.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
