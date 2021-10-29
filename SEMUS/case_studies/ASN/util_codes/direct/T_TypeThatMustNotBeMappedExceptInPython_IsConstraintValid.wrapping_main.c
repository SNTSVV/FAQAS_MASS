
/* Wrapping main template for the function T_TypeThatMustNotBeMappedExceptInPython_IsConstraintValid defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>

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
    T_TypeThatMustNotBeMappedExceptInPython pVal;
    int pErrCode;
    memset(&pVal, 0, sizeof(pVal));
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal"); //T_TypeThatMustNotBeMappedExceptInPython

    // Call function under test
    result_faqas_semu = T_TypeThatMustNotBeMappedExceptInPython_IsConstraintValid(&pVal, &pErrCode);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
