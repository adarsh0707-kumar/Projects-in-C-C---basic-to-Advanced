#include <stdio.h>
#include "tests.h"

int testsRun = 0;
int testsFailed = 0;

int main(void)
{
    printf("===== Running Calculator Unit Tests (Phase 20) =====\n\n");

    printf("-- run_stack_tests --\n");
    run_stack_tests();

    printf("-- run_tokenizer_tests --\n");
    run_tokenizer_tests();

    printf("-- run_validator_tests --\n");
    run_validator_tests();

    printf("-- run_parser_tests --\n");
    run_parser_tests();

    printf("-- run_postfix_tests --\n");
    run_postfix_tests();

    printf("-- run_functions_tests --\n");
    run_functions_tests();

    printf("-- run_variables_tests --\n");
    run_variables_tests();

    printf("-- run_history_tests --\n");
    run_history_tests();

    printf("-- run_memory_tests --\n");
    run_memory_tests();

    printf("\n=====================================================\n");

    if (testsFailed == 0)
        printf("ALL %d TESTS PASSED\n", testsRun);
    else
        printf("%d/%d TESTS FAILED\n", testsFailed, testsRun);

    printf("=====================================================\n");

    return testsFailed == 0 ? 0 : 1;
}