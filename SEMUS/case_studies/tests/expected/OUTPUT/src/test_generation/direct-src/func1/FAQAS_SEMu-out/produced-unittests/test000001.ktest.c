
/* Wrapping main template for the function func1 defined in the file /home/ubuntu/faqas_semu/case_studies/tests/WORKSPACE/DOWNLOADED/src.c */
/* The following mutants (IDs) were targeted to generated this test: [9] */

#include <stdio.h>
#include <string.h>



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    int v;
    MyType mytype_var;
    memset(&v, 0, sizeof(v));
    memset(&mytype_var, 0, sizeof(mytype_var));
    const unsigned char v_faqas_semu_test_data[] = {0x00, 0x00, 0x00, 0x80};
    const unsigned char mytype_var_faqas_semu_test_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    memcpy(&v, v_faqas_semu_test_data, sizeof(v)); // Integer val is -2147483648
    memcpy(&mytype_var, mytype_var_faqas_semu_test_data, sizeof(mytype_var));

    // Call function under test
    result_faqas_semu = func1(v, &mytype_var);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: x=%d, y=%d, z=%s\n", mytype_var.x, mytype_var.y, mytype_var.z);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}