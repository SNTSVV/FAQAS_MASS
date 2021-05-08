

/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c*/

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    flag result;
    MyInt val0;
    BitStream bitStrm;
    int errCode;
    klee_make_symbolic(&val0, sizeof(val0), "val0");
    klee_make_symbolic(&bitStrm, sizeof(bitStrm), "bitStrm");

    result = MyInt_Decode(&val0, &bitStrm, &errCode);
    return (int)result;
}
