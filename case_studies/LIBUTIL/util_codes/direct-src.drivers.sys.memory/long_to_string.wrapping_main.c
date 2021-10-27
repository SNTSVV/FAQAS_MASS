
/* Wrapping main template for the function long_to_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/drivers/sys/memory.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    const char * result_faqas_semu;

    // Declare arguments and make input ones symbolic
    char buf;
    unsigned long buf_size;
    long lvalue;
    memset(&buf_size, 0, sizeof(buf_size));
    memset(&lvalue, 0, sizeof(lvalue));
    klee_make_symbolic(&buf_size, sizeof(buf_size), "buf_size"); //unsigned long
    klee_make_symbolic(&lvalue, sizeof(lvalue), "lvalue"); //long

    // Call function under test
    result_faqas_semu = long_to_string(&buf, buf_size, lvalue);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: buf = %d\n", buf);
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = %d\n", atoi(result_faqas_semu));
    return 0;
}
