
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
    char val[ 21 ];
    struct BitStream_t pBitStrm;
    int pErrCode;
    klee_make_symbolic(&val, sizeof(val), "val");
    klee_make_symbolic(&pBitStrm, sizeof(pBitStrm), "pBitStrm");

    // Call function under test
    result_faqas_semu = ConfigString_Decode(&val, &pBitStrm, &pErrCode);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
