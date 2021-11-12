
/* Wrapping main template for the function timestamp_add defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/timestamp.c */
/* The following mutants (IDs) were targeted to generated this test: [1] */

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
    gs_timestamp_t add;
    memset(&base, 0, sizeof(base));
    memset(&add, 0, sizeof(add));
    const unsigned char base_faqas_semu_test_data[] = {0x7c, 0x7c, 0x7c, 0x7c, 0x80, 0x7c, 0x98, 0x33};
    const unsigned char add_faqas_semu_test_data[] = {0x4d, 0x4d, 0x4d, 0x4d, 0x80, 0x4d, 0x02, 0x08};
    memcpy(&base, base_faqas_semu_test_data, sizeof(base));
    memcpy(&add, add_faqas_semu_test_data, sizeof(add));

    // Call function under test
    result_faqas_semu = timestamp_add(&base, &add);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", base.tv_sec, base.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}