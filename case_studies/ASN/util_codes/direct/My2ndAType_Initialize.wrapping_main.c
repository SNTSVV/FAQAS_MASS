
/* Wrapping main template for the function My2ndAType_Initialize defined in the file ../DOWNLOADED/casestudy/test.c */
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
    AType pVal;
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal");

    // Call function under test
    My2ndAType_Initialize(&pVal);

    // Make some output
    return 0;
}
