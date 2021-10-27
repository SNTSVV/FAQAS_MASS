
/* Wrapping main template for the function getNextTMQueueItemSize defined in the file /opt/faqas_semu_fork/case_studies/ESAIL/util_codes/../WORKSPACE/DOWNLOADED/Obsw/Source/ProtocolLayer/TMFrameBuilder/Source/TMFrameBuilder.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    unsigned short result_faqas_semu;

    // Declare arguments and make input ones symbolic
    unsigned char vc;
    memset(&vc, 0, sizeof(vc));
    klee_make_symbolic(&vc, sizeof(vc), "vc"); //unsigned char

    // Call function under test
    result_faqas_semu = getNextTMQueueItemSize(vc);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %hu\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
