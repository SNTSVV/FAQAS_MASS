
/* Wrapping main template for the function long_to_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/drivers/sys/memory.c */
/* The following mutants (IDs) were targeted to generated this test: [13] */

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
    char buf;
    unsigned long buf_size;
    long lvalue;
    memset(&buf_size, 0, sizeof(buf_size));
    memset(&lvalue, 0, sizeof(lvalue));
    const unsigned char buf_size_faqas_semu_test_data[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    const unsigned char lvalue_faqas_semu_test_data[] = {0x04, 0xc8, 0x22, 0xa1, 0x07, 0x00, 0x00, 0x00};
    memcpy(&buf_size, buf_size_faqas_semu_test_data, sizeof(buf_size)); // Unsigned val is 18446744073709551615
    memcpy(&lvalue, lvalue_faqas_semu_test_data, sizeof(lvalue)); // Integer val is 32768182276

    // Call function under test
    result_faqas_semu = long_to_string(&buf, buf_size, lvalue);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: buf = %d\n", buf);
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = %d\n", atoi(result_faqas_semu));
    return 0;
}