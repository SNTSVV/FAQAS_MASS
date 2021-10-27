
/* Wrapping main template for the function ConfigString_Decode defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    char val[ 21 ];
    struct BitStream_t pBitStrm;
    int pErrCode;
    memset(&val, 0, sizeof(val));
    static byte encBuff[T_POS_SET_REQUIRED_BYTES_FOR_ENCODING + 1];
	BitStream_Init(&pBitStrm, encBuff, T_POS_SET_REQUIRED_BYTES_FOR_ENCODING);
    klee_make_symbolic(&val, sizeof(val), "val"); //char [ 21 ]

    // Call function under test
    result_faqas_semu = ConfigString_Decode(val, &pBitStrm, &pErrCode);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\n", pErrCode);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
