
/* Wrapping main template for the function ConfigString_Equal defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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
    char val1[ 21 ];
    char val2[ 21 ];
    memset(&val1, 0, sizeof(val1));
    memset(&val2, 0, sizeof(val2));
    klee_make_symbolic(&val1, sizeof(val1), "val1"); //char [ 21 ]
    klee_make_symbolic(&val2, sizeof(val2), "val2"); //char [ 21 ]

    // Call function under test
    result_faqas_semu = ConfigString_Equal(val1, val2);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}