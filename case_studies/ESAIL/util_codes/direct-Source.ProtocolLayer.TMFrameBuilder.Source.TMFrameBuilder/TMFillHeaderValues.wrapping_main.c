
/* Wrapping main template for the function TMFillHeaderValues defined in the file /opt/faqas_semu_fork/case_studies/ESAIL/util_codes/../WORKSPACE/DOWNLOADED/Obsw/Source/ProtocolLayer/TMFrameBuilder/Source/TMFrameBuilder.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare arguments and make input ones symbolic
    unsigned short headerValues[TM_HEADER_ITEMS];
    unsigned char virtualChannel;
    unsigned short masterCount;
    unsigned short virtualCount;
    _Bool isIdle;
    memset(&virtualChannel, 0, sizeof(virtualChannel));
    memset(&masterCount, 0, sizeof(masterCount));
    memset(&virtualCount, 0, sizeof(virtualCount));
    memset(&isIdle, 0, sizeof(isIdle));
    klee_make_symbolic(&virtualChannel, sizeof(virtualChannel), "virtualChannel"); //unsigned char
    klee_make_symbolic(&masterCount, sizeof(masterCount), "masterCount"); //unsigned short
    klee_make_symbolic(&virtualCount, sizeof(virtualCount), "virtualCount"); //unsigned short
    klee_make_symbolic(&isIdle, sizeof(isIdle), "isIdle"); //_Bool

    // Call function under test
    TMFillHeaderValues(headerValues, virtualChannel, masterCount, virtualCount, isIdle);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: headerValues = ");
    int i;
    for (i = 0; i < TM_HEADER_ITEMS; i++) {
        printf("%hu", headerValues[i]);
    }
    printf("\n");
    return 0;
}
