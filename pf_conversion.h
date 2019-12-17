#include <bits/stdc++.h>
struct Stack
{
    int top;
    unsigned capacity;
    int* array;
};

struct Stack* createStack( unsigned capacity )
{
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));

    if (!stack)
        return NULL;

    stack->top = -1;
    stack->capacity = capacity;

    stack->array = (int*) malloc(stack->capacity * sizeof(int));

    if (!stack->array)
        return NULL;
    return stack;
}
int isEmpty(struct Stack* stack)
{
    return stack->top == -1 ;
}
char peek(struct Stack* stack)
{
    return stack->array[stack->top];
}
char pop(struct Stack* stack)
{
    if (!isEmpty(stack))
        return stack->array[stack->top--] ;
    return '$';
}
void push(struct Stack* stack, char op)
{
    stack->array[++stack->top] = op;
}



int isOperand(char ch)      // function to check if the given character is operand
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


int Prec(char ch)   //function to return precedence of an operator.Higher value implies greater precedence
{

    switch (ch)
    {
        case '|': return 1;
        case '&': return 2;
        case '!': return 3;
        case '<': return 4;
        case '>' : return 4;
        case '='  : return 4;

    }
    return -1;
}


char* infixToPostfix(char* exp)     //converts an infix string to postfix string
{
    int i, k;

    // Create a stack of capacity equal to expression size
    struct Stack* stack = createStack(strlen(exp));
    if(!stack) // to check if stack was successfully created
        {
		std::cerr<<"error in postfix conversion.this might be a technical glitch";
		exit(-1);
	}

    for (i = 0, k = -1; exp[i]; ++i)
    {
         // If the scanned character is an operand, add it to output.
        if (isOperand(exp[i]))
            exp[++k] = exp[i];

        // If the scanned character is an ‘(‘, push it to the stack.
        else if (exp[i] == '(')
            push(stack, exp[i]);

        //  If the scanned character is an ‘)’, pop and output from the stack
        // until an ‘(‘ is encountered.
        else if (exp[i] == ')')
        {
            while (!isEmpty(stack) && peek(stack) != '(')
                exp[++k] = pop(stack);
            if (!isEmpty(stack) && peek(stack) != '('){
               std::cerr<<"error in postfix conversion.this might be a technical glitch";
                exit(-1); // invalid expression
                }
            else
                pop(stack);
        }
        else // an operator is encountered
        {
            while (!isEmpty(stack) && Prec(exp[i]) <= Prec(peek(stack)))
                exp[++k] = pop(stack);
            push(stack, exp[i]);
        }

    }

    while (!isEmpty(stack)) {        // pop all the operators from the stack
        exp[++k] = pop(stack);
    }
    exp[++k] = '\0';
    return exp;
}
