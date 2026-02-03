%{
#include <stdio.h>
%}

%%
"int"|"float"|"if"|"else"|"while"   { printf("Keyword: %s\n", yytext); }
[a-zA-Z]                            { printf("Alphabet: %s\n", yytext); }
[+\-*/=]                            { printf("Operator: %s\n", yytext); }
[;,.(){}]                           { printf("Symbol: %s\n", yytext); }
.|\n                                 ;
%%

int main(){ yylex(); return 0; }
