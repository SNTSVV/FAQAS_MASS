
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
    T_STRING pVal;
    struct BitStream_t pBitStrm;
    int pErrCode;
    int bCheckConstraints;
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal");
    klee_make_symbolic(&pBitStrm, sizeof(pBitStrm), "pBitStrm");
    klee_make_symbolic(&bCheckConstraints, sizeof(bCheckConstraints), "bCheckConstraints");

    // Call function under test
    result_faqas_semu = My2ndString_Encode(&pVal, &pBitStrm, &pErrCode, bCheckConstraints);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
