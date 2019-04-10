#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "ReversePolishNotation.h"

//////////////////////////////////////////////////
const RPNE_t operators[] = {
    // operator/operand,  　　　　priority, type
    // {OPERAND,                INT_MAX,    OPERAND         },
    {   OPERATOR_LEFT_BRACKET,  INT_MAX-1,  OTHER_OPERATOR  },  //    (
    {   OPERATOR_POWER,         60,         BINARY_OPERATOR },  //    ^
    {   OPERATOR_MINUS,         50,         UNARY_OPERATOR  },  //    ~
    {   OPERATOR_REMAINDER,     40,         BINARY_OPERATOR },  //    %
    {   OPERATOR_DIVIDE,        40,         BINARY_OPERATOR },  //    /
    {   OPERATOR_MULTIPLY,      40,         BINARY_OPERATOR },  //    *
    {   OPERATOR_SUBTRACT,      30,         BINARY_OPERATOR },  //    -
    {   OPERATOR_ADD,           30,         BINARY_OPERATOR },  //    +
    {   OPERATOR_RIGHT_BRACKET, 2,          OTHER_OPERATOR  },  //    )
    {   OPERATOR_EQUAL,         1,          BINARY_OPERATOR },  //    =
    {   "",                     0,          0               }   //    terminator.
};

//////////////////////////////////////////////////
char *convertToRPN(char *expression, char *separator) {
    char *s = "\0";
    char *rpn = calloc(strlen(expression) * 2 + 1, sizeof(char));
    Queue_t *Q = convertQueueToRPN(expression);
    while (!isEmptyQueue(Q)) {
        char *token = deQueue(Q);
        strcat(rpn, s);
        strcat(rpn, token);
        free(token);
        s = separator;
    }
    destroyQueue(Q, QUEUE_OPTION_WITH_ELEMENT);
    return rpn;
}

Queue_t *convertQueueToRPN(char *expression) {
    Queue_t *tokenQ = createTokenQueue(expression);
    Queue_t *Q = createQueue();
    Stack_t *S = createStack();
    while (!isEmptyQueue(tokenQ)) {
        char *token = deQueue(tokenQ);
        RPNE_t *element = createCellOnRPN(token);
        
        RPNE_t *stackElement = peekStack(S);
        while ((stackElement != NULL) &&
               (strcmp(stackElement->token, OPERATOR_LEFT_BRACKET) != 0) &&
               (stackElement->priority >= element->priority)) {
            stackElement = popStack(S);
            enQueue(Q, stackElement->token);
            destroyCellOnRPN(stackElement, RPN_OPTION_NONE);
            stackElement = peekStack(S);
        }
        
        if (strcmp(element->token, OPERATOR_RIGHT_BRACKET) != 0) {
            pushStack(S, element);
        }
        else {
            destroyCellOnRPN(element, RPN_OPTION_WITH_ELEMENT);
            stackElement = peekStack(S);
            if ((stackElement != NULL) &&
                (strcmp(stackElement->token, OPERATOR_LEFT_BRACKET) == 0)) {
                stackElement = popStack(S);
                destroyCellOnRPN(stackElement, RPN_OPTION_WITH_ELEMENT);
            }
        }
    }
    while (!isEmptyStack(S)) {
        RPNE_t *stackElement = popStack(S);
        enQueue(Q, stackElement->token);
        destroyCellOnRPN(stackElement, RPN_OPTION_NONE);
    }
    destroyStack(S, STACK_OPTION_WITH_ELEMENT);
    destroyQueue(tokenQ, QUEUE_OPTION_WITH_ELEMENT);
    
    return Q;
}

int getPriorityOnRPN(char *token) {
    int i = 0;
    while (operators[i].priority > 0) {
        if (strcmp(token, operators[i].token) == 0) {
            return operators[i].priority;
        }
        i++;
    }
    return INT_MAX; // operand's priority.
}

RPNE_t *createCellOnRPN(char *token) {
    RPNE_t *element = malloc(sizeof(RPNE_t));
    if (element == NULL) return NULL;
    element->token = token;
    element->priority = getPriorityOnRPN(token);
    return element;
}

bool destroyCellOnRPN(RPNE_t *element, RPN_OPTION_e option) {
    // Block illegal parameters.
    if (element == NULL) return false;
    if ((option == RPN_OPTION_WITH_ELEMENT) &&
        (element->token != NULL)) {
        free(element->token);
    }
    free(element);
    
    return true;
}

Queue_t *createTokenQueue(char *expression) {
    char *exp = calloc(strlen(expression) + 1, sizeof(char));
    strcpy(exp, expression);
    
    Queue_t *inQ = createQueue();
    Queue_t *outQ = createQueue();
    enQueue(outQ, exp);
    
    int i = 0;
    while (operators[i].priority > 0) {
        // swap in/out queue.
        Queue_t *tempQ = inQ;
        inQ = outQ;
        outQ = tempQ;

        while (!isEmptyQueue(inQ)) {
            char *string = deQueue(inQ);
            splitStringRPN(outQ, operators[i].token, string);
            free(string);
        }
        i++;
    }
    
    destroyQueue(inQ, QUEUE_OPTION_WITH_ELEMENT);
    return outQ;
}

Queue_t *splitStringRPN(Queue_t *Q, char *keyword, char *string) {
    // Block illegal parameters.
    if (Q == NULL) return NULL;
    
    int lengthOfKeyword = (int)strlen(keyword);
    char *checkString = string;
    char *token = "\0";
    while (true) {
        int lengthOfCheckString = (int)strlen(checkString);
        if (lengthOfCheckString < lengthOfKeyword) {
            if (lengthOfCheckString > 0) {
                token = calloc(strlen(checkString) + 1, sizeof(char));
                strcpy(token, checkString);
                enQueue(Q, token);
            }
            break;
        }
        
        bool find = false;
        for (int i=0; i<lengthOfCheckString; i++) {
            int check = strncmp(keyword, &checkString[i], lengthOfKeyword);
            if (check == 0) {
                find = true;
                if (i == 0) {   // split into two.
                    token = calloc(lengthOfKeyword + 1, sizeof(char));
                    strncpy(token, checkString, lengthOfKeyword);
                    enQueue(Q, token);
                    
                    checkString = &checkString[lengthOfKeyword];
                }
                else {  // split into three.
                    token = calloc(i + 1, sizeof(char));
                    strncpy(token, checkString, i);
                    enQueue(Q, token);
                    
                    token = calloc(lengthOfKeyword + 1, sizeof(char));
                    strncpy(token, &checkString[i], lengthOfKeyword);
                    enQueue(Q, token);
                    
                    checkString = &checkString[i + lengthOfKeyword];
                }
                break;
            }
        }
        if (!find) {
            token = calloc(strlen(checkString) + 1, sizeof(char));
            strcpy(token, checkString);
            enQueue(Q, token);
            break;
        }
    }
    
    return Q;
}
