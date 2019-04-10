#ifndef ReversePolishNotation_h
#define ReversePolishNotation_h

#include <stdbool.h>
#include "Stack.h"
#include "Queue.h"

//////////////////////////////////////////////////
#define OPERATOR_LEFT_BRACKET "("
#define OPERATOR_POWER "^"
#define OPERATOR_MINUS "~"
#define OPERATOR_REMAINDER "%"
#define OPERATOR_DIVIDE "/"
#define OPERATOR_MULTIPLY "*"
#define OPERATOR_SUBTRACT "-"
#define OPERATOR_ADD "+"
#define OPERATOR_RIGHT_BRACKET ")"
#define OPERATOR_EQUAL "="

#define BINARY_OPERATOR 2
#define UNARY_OPERATOR 1
#define OTHER_OPERATOR 0

typedef enum RPNOption {
    RPN_OPTION_NONE,
    RPN_OPTION_WITH_ELEMENT,
    
    
} RPN_OPTION_e;

typedef struct Element_RPN {
    char *token;
    int priority;
    int type;
} RPNE_t;

extern const RPNE_t operators[];

//////////////////////////////////////////////////
char *convertToRPN(char *expression, char *separator);
QUEUE_t *convertQueueToRPN(char *expression);
int getPriorityOnRPN(char *token);
RPNE_t *createCellOnRPN(char *token);
bool destroyCellOnRPN(RPNE_t *element, RPN_OPTION_e option);
QUEUE_t *createTokenQueue(char *expression);
QUEUE_t *splitStringRPN(QUEUE_t *Q, char *keyword, char *string);

#endif

