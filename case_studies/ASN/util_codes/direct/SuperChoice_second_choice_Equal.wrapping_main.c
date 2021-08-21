
/* Wrapping main template for the function SuperChoice_second_choice_Equal defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    _Bool result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned long pVal1;
    unsigned long pVal2;
    klee_make_symbolic(&pVal1, sizeof(pVal1), "pVal1");
    klee_make_symbolic(&pVal2, sizeof(pVal2), "pVal2");

    // Call function under test
    result_faqas_semu = SuperChoice_second_choice_Equal(&pVal1, &pVal2);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
