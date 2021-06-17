
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c*/

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    flag result;
    My2ndInt val0;
    BitStream bitStrm;
    flag checkC;
    int errCode;
    klee_make_symbolic(&val0, sizeof(val0), "val0");
    klee_make_symbolic(&bitStrm, sizeof(bitStrm), "bitStrm");
    klee_make_symbolic(&checkC, sizeof(checkC), "checkC");

    result = My2ndInt_Encode(&val0, &bitStrm, &errCode, checkC);
    printf("%d\n", errCode);
    return (int)result;
}
