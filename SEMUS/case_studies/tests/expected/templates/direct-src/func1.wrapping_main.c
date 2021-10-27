
/* Wrapping main template for the function func1 defined in the file /home/ubuntu/faqas_semu/case_studies/tests/WORKSPACE/DOWNLOADED/src.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

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
    klee_make_symbolic(&v, sizeof(v), "v"); //int
    klee_make_symbolic(&mytype_var, sizeof(mytype_var), "mytype_var"); //MyType

    // Call function under test
    result_faqas_semu = func1(v, &mytype_var);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: x=%d, y=%d, z=%s\n", mytype_var.x, mytype_var.y, mytype_var.z);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
