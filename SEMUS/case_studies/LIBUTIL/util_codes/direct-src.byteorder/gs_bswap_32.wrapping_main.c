
/* Wrapping main template for the function gs_bswap_32 defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/byteorder.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    unsigned int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned int value;
    memset(&value, 0, sizeof(value));
    klee_make_symbolic(&value, sizeof(value), "value"); //unsigned int

    // Call function under test
    result_faqas_semu = gs_bswap_32(value);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %u\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
