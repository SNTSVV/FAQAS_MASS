
/* Wrapping main template for the function ConfigString_Initialize defined in the file ../DOWNLOADED/casestudy/test.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare arguments and make input ones symbolic
    char val[ 21 ];
    klee_make_symbolic(&val, sizeof(val), "val");

    // Call function under test
    ConfigString_Initialize(val);

    // Make some output
    return 0;
}
