
/* Wrapping main template for the function __ieee754_hypotf defined in the file /home/ubuntu/workspace/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/ef_hypot.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    float result_faqas_semu;

    // Declare arguments and make input ones symbolic
    float x;
    float y;
    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));
    klee_make_symbolic(&x, sizeof(x), "x"); //float
    klee_make_symbolic(&y, sizeof(y), "y"); //float

    // Call function under test
    result_faqas_semu = __ieee754_hypotf(x, y);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %g\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
