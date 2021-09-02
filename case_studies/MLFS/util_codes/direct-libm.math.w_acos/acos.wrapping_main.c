
/* Wrapping main template for the function acos defined in the file /home/FAQAS/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/w_acos.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    double result_faqas_semu;

    // Declare arguments and make input ones symbolic
    double x;
    memset(&x, 0, sizeof(x));
    klee_make_symbolic(&x, sizeof(x), "x");

    // Call function under test
    result_faqas_semu = acos(x);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %G\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
