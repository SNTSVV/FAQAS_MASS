#include "klee/klee.h"

#include "asn1crt.h"
#include "test.h"

int main(int argc, char** argv)
{
    flag result;
    int errCode;
    MyInt tmp0;
    MyInt tc_data;
    klee_make_symbolic(&tmp0, sizeof(tmp0), "tmp0");
    klee_make_symbolic(&tc_data, sizeof(tc_data), "tc_data");

    MyInt_Initialize((&(tmp0)));
    result = MyInt_enc_dec(&tc_data, &errCode, "test_case_UPER_*");
    return int(result);
}

