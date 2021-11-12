
/* Wrapping main template for the function gs_clock_from_string defined in the file /opt/faqas_semu_fork/case_studies/LIBUTIL/util_codes/../WORKSPACE/DOWNLOADED/libutil/src/clock.c */
/* The following mutants (IDs) were targeted to generated this test: [32] */

#include <stdio.h>
#include <string.h>



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    enum gs_error_t result_faqas_semu;

    // Declare arguments and make input ones symbolic
    char str[GS_CLOCK_ISO8601_BUFFER_LENGTH];
    gs_timestamp_t ts;
    memset(&str, 0, sizeof(str));
    const unsigned char str_faqas_semu_test_data[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    memcpy(&str, str_faqas_semu_test_data, sizeof(str));

    // Call function under test
    result_faqas_semu = gs_clock_from_string(str, &ts);

    // Make some output
    printf("FAQAS-SEMU-TEST-OUTPUT: result_faqas_semu = tv_sec: %u, tv_nsec: %u\n", ts.tv_sec, ts.tv_nsec);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}