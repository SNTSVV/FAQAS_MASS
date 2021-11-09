
/* Wrapping main template for the function My2ndTypeNested_Decode defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    TypeNested pVal;
    struct BitStream_t pBitStrm;
    int pErrCode;
    memset(&pVal, 0, sizeof(pVal));
    memset(&pBitStrm, 0, sizeof(pBitStrm));
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal"); //TypeNested
    klee_make_symbolic(&pBitStrm, sizeof(pBitStrm), "pBitStrm"); //struct BitStream_t

    // Call function under test
    result_faqas_semu = My2ndTypeNested_Decode(&pVal, &pBitStrm, &pErrCode);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}