#include "infix.h"


int get_precedence(char* operator)
{
    switch (operator[0]) {
        case '+': return 0;
        case '-': return 0;
        case 'X': return 1;
        case '/': return 1;
        case '^': return 2;
        default: return -100;
    }
}

// evaluate expression stored as an array of string tokens
double evaluate_infix_expression(char ** args, int nargs) {
    struct double_stack* stack_of_index = double_stack_new(nargs);

    char** output = malloc(nargs*sizeof(char*));
    int postfix_length = 0;
    for(int i = 0; i < nargs; i++)
    {
        if((args[i][0]>='0' && args[i][0] <= '9') ||
		 (args[i][0] =='-' && args[i][1]>='0' && args[i][1] <= '9'))
        {
            output[postfix_length++] = args[i];
        }
        else if (args[i][0] == '(')
        {
            double_stack_push(stack_of_index,i);
        }
        else if(args[i][0] == '+' || (args[i][0] == '-' && args[i][1] == '\0') ||
 		args[i][0] =='X'|| args[i][0] == '/' || args[i][0] == '^')
        {
            while(double_stack_peek(stack_of_index)>0&&
		get_precedence(args[(int)double_stack_peek(stack_of_index)])>=
			get_precedence(args[i]))
            {
                output[postfix_length++] = args[(int)double_stack_pop(stack_of_index)];
            }
            double_stack_push(stack_of_index, i);
        }
        else if(args[i][0]==')')
        {
            while(double_stack_peek(stack_of_index)>0&&
		args[(int)double_stack_peek(stack_of_index)][0]!='(')
            {
                output[postfix_length++] = args[(int)double_stack_pop(stack_of_index)];
            }

            double_stack_pop(stack_of_index);
        }
    }
    while(!double_stack_empty(stack_of_index))
    {
        output[postfix_length++] = args[(int)double_stack_pop(stack_of_index)];
    }
    
    double return_value = evaluate_postfix_expression(output,postfix_length);

    free(output); 
    free(stack_of_index->items);
    free(stack_of_index);

    return return_value;
}



