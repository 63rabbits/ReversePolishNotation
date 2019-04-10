#include <unistd.h>
//#define checkMemoryLeak

#include <stdio.h>
#include <stdlib.h>
#include "ReversePolishNotation.h"

//////////////////////////////////////////////////
void test(void);

//////////////////////////////////////////////////
int main(int argc, const char * argv[]) {
    
#ifdef checkMemoryLeak
    while (true) {
        test();
        sleep(1);
    }
#else
    test();
#endif
    
    return EXIT_SUCCESS;
}

void test() {
    //    char *expression = "(A+B*C)/D";
    //    char *expression = "((A+B)*C)/(D+E)";
    char *expression = "f=(A+B*C)/(~D+E^F*G)";
    printf("Expression              : %s\n", expression);
    char *rpn = convertToRPN(expression, "");
    printf("Reverse Polish Notation : %s\n", rpn);
    free(rpn);
    return;
}
