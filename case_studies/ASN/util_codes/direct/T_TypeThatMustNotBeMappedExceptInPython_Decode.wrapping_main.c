
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c*/

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
    int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    T_TypeThatMustNotBeMappedExceptInPython pVal;
    struct BitStream_t pBitStrm;
    int pErrCode;
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal");
    klee_make_symbolic(&pBitStrm, sizeof(pBitStrm), "pBitStrm");

    // Call function under test
    result_faqas_semu = T_TypeThatMustNotBeMappedExceptInPython_Decode(&pVal, &pBitStrm, &pErrCode);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
