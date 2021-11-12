
/* Wrapping main template for the function gs_error_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/error.c */
/* The following mutants (IDs) were targeted to generated this test: [1] */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    const char * result_faqas_semu;

    // Declare arguments and make input ones symbolic
    int error;
    memset(&error, 0, sizeof(error));
    const unsigned char error_faqas_semu_test_data[] = {0xf0, 0xff, 0xff, 0xff};
    memcpy(&error, error_faqas_semu_test_data, sizeof(error)); // Integer val is -16

    // Call function under test
    result_faqas_semu = gs_error_string(error);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n",atoi(result_faqas_semu));
    return 0;
}