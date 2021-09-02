
/* Wrapping main template for the function __kernel_sin defined in the file /home/FAQAS/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/k_sin.c */
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
    double y;
    int iy;
    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));
    memset(&iy, 0, sizeof(iy));
    klee_make_symbolic(&x, sizeof(x), "x");
    klee_make_symbolic(&y, sizeof(y), "y");
    klee_make_symbolic(&iy, sizeof(iy), "iy");

    // Call function under test
    result_faqas_semu = __kernel_sin(x, y, iy);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %G\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
