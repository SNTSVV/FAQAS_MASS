
/* Wrapping main template for the function SuperRestrictedChoice_Encode defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    SuperChoice pVal;
    struct BitStream_t pBitStrm;
    int pErrCode;
    _Bool bCheckConstraints;
    memset(&pVal, 0, sizeof(pVal));
    memset(&bCheckConstraints, 0, sizeof(bCheckConstraints));
    static byte encBuff[T_POS_SET_REQUIRED_BYTES_FOR_ENCODING + 1];
	BitStream_Init(&pBitStrm, encBuff, T_POS_SET_REQUIRED_BYTES_FOR_ENCODING);
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal"); //SuperChoice
    klee_make_symbolic(&bCheckConstraints, sizeof(bCheckConstraints), "bCheckConstraints"); //_Bool

    // Call function under test
    result_faqas_semu = SuperRestrictedChoice_Encode(&pVal, &pBitStrm, &pErrCode, bCheckConstraints);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
