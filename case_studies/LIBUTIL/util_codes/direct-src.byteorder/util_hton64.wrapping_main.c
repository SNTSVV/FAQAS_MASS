
/* Wrapping main template for the function util_hton64 defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/byteorder.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    unsigned long result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned long h64;
    memset(&h64, 0, sizeof(h64));
    klee_make_symbolic(&h64, sizeof(h64), "h64"); //unsigned long

    // Call function under test
    result_faqas_semu = util_hton64(h64);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %lu\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
