
/* Wrapping main template for the function exp defined in the file /home/ubuntu/workspace/faqas_semu/case_studies/MLFS/util_codes/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2/libm/math/w_exp.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    double result_faqas_semu;

    // Declare arguments and make input ones symbolic
    double x;
    klee_make_symbolic(&x, sizeof(x), "x");

    // Call function under test
    result_faqas_semu = exp(x);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %G\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
