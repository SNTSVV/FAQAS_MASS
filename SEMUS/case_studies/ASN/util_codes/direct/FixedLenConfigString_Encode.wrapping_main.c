
/* Wrapping main template for the function FixedLenConfigString_Encode defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    char val[ 6 ];
    struct BitStream_t pBitStrm;
    int pErrCode;
    _Bool bCheckConstraints;
    memset(&val, 0, sizeof(val));
    memset(&pBitStrm, 0, sizeof(pBitStrm));
    memset(&bCheckConstraints, 0, sizeof(bCheckConstraints));
    klee_make_symbolic(&val, sizeof(val), "val"); //char [ 6 ]
    klee_make_symbolic(&pBitStrm, sizeof(pBitStrm), "pBitStrm"); //struct BitStream_t
    klee_make_symbolic(&bCheckConstraints, sizeof(bCheckConstraints), "bCheckConstraints"); //_Bool

    // Call function under test
    result_faqas_semu = FixedLenConfigString_Encode(val, &pBitStrm, &pErrCode, bCheckConstraints);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
