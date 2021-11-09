
/* Wrapping main template for the function T_SETOF_Initialize defined in the file ../WORKSPACE/DOWNLOADED/casestudy/test.c */
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

    // Declare arguments and make input ones symbolic
    T_SETOF pVal;
    memset(&pVal, 0, sizeof(pVal));
    klee_make_symbolic(&pVal, sizeof(pVal), "pVal"); //T_SETOF

    // Call function under test
    T_SETOF_Initialize(&pVal);

    // Make some output
    return 0;
}