
/* Wrapping main template for the function __bswap_32 defined in the file /home/ubuntu/workspace/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/ef_atan2.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    unsigned int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned int __bsx;
    klee_make_symbolic(&__bsx, sizeof(__bsx), "__bsx");

    // Call function under test
    result_faqas_semu = __bswap_32(__bsx);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %u\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
