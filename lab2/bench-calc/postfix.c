#include "postfix.h"

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
        if(args[i][0] == '+' || (args[i][0] == '-' && args[i][1] == '\0') || args[i][0] == 'X' || 			args[i][0] == '/' || args[i][0] == '^')
        {
            double operand2 = double_stack_pop(stack);
            double operand1 = double_stack_pop(stack);
            double_stack_push(stack, evaluate(args[i][0],operand1,operand2));
        }
        else
        {
            double_stack_push(stack,atof(args[i]));
        }

    }

    double return_value = double_stack_pop(stack);

    free(stack->items);
    free(stack);

    return return_value;
}
