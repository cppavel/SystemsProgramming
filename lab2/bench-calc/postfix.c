#include "postfix.h"

typedef struct double_stack double_stack;

double evaluate(char operator, double operand1, double operand2)
{
    double result = -99;
    switch(operator)
    {
        case '+': result = operand1 + operand2; break;
        case '-': result = operand1 - operand2; break;
        case 'X': result = operand1 * operand2; break;
        case '/': result = operand1 / operand2; break;
        case '^': result = pow(operand1,operand2); break;
        default: break;
    }

    return result;
}
// evaluate expression stored as an array of string tokens
double evaluate_postfix_expression(char ** args, int nargs) {
    struct double_stack* stack = double_stack_new(nargs);

    for(int i = 0; i < nargs; i++)
    {
        if(args[i][0] == '+' || (args[i][0] == '-' && args[i][1] == '\0') || args[i][0] == 'X' || args[i][0] == '/' || args[i][0] == '^')
        {
            double operand2 = double_stack_pop(stack);
            double operand1 = double_stack_pop(stack);
            printf("%f %c %f = %f\n", operand1, args[i][0], operand2,evaluate(args[i][0],operand1,operand2));
            double_stack_push(stack, evaluate(args[i][0],operand1,operand2));
        }
        else
        {
            printf("pushed %f\n",atof(args[i]));
            double_stack_push(stack,atof(args[i]));
        }

    }

    return double_stack_pop(stack);
}
