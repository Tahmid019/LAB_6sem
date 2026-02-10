%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int result = 0;
int operand1 = 0;
int operand2 = 0;
char operator = '+';
%}

%%

[0-9]+  {
    operand2 = atoi(yytext);
    switch(operator) {
        case '+': result = operand1 + operand2; break;
        case '-': result = operand1 - operand2; break;
        case '*': result = operand1 * operand2; break;
        case '/': result = operand1 / operand2; break;
        case '%': result = operand1 % operand2; break;
    }
    operand1 = result;
    printf("%d\n", result);
    }

[+\-*/%]  {
    operator = yytext[0];
    }

\n        { }

[ \t]     { }

.         { printf("Invalid character: %s\n", yytext); }

%%

int main() {
    printf("Do ./q1 < q1ip.txt ---------: \n");
    yylex();
    return 0;
}