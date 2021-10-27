
/* Wrapping main template for the function gs_error_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/error.c */
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
    int error;
    memset(&error, 0, sizeof(error));
    klee_make_symbolic(&error, sizeof(error), "error"); //int

    // Call function under test
    result_faqas_semu = gs_error_string(error);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n",atoi(result_faqas_semu));
    return 0;
}
