
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
    char val1[ 6 ];
    char val2[ 6 ];
    klee_make_symbolic(&val1, sizeof(val1), "val1");
    klee_make_symbolic(&val2, sizeof(val2), "val2");

    // Call function under test
    result_faqas_semu = FixedLenConfigString_Equal(val1, val2);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
