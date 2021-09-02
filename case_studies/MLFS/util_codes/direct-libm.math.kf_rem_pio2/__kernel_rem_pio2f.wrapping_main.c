
/* Wrapping main template for the function __kernel_rem_pio2f defined in the file /home/FAQAS/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/kf_rem_pio2.c */
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
    float x;
    float y;
    int e0;
    int nx;
    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));
    memset(&e0, 0, sizeof(e0));
    memset(&nx, 0, sizeof(nx));
    klee_make_symbolic(&x, sizeof(x), "x");
    klee_make_symbolic(&y, sizeof(y), "y");
    klee_make_symbolic(&e0, sizeof(e0), "e0");
    klee_make_symbolic(&nx, sizeof(nx), "nx");

    // Call function under test
    result_faqas_semu = __kernel_rem_pio2f(&x, &y, e0, nx);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
