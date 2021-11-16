
/* Wrapping main template for the function timestamp_diff defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/timestamp.c */
/* The following mutants (IDs) were targeted to generated this test: [4] */

#include <stdio.h>
#include <string.h>



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    gs_timestamp_t base;
    gs_timestamp_t diff;
    memset(&base, 0, sizeof(base));
    memset(&diff, 0, sizeof(diff));
    const unsigned char base_faqas_semu_test_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char diff_faqas_semu_test_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    memcpy(&base, base_faqas_semu_test_data, sizeof(base));
    memcpy(&diff, diff_faqas_semu_test_data, sizeof(diff));

    // Call function under test
    result_faqas_semu = timestamp_diff(&base, &diff);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", base.tv_sec, base.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}