%{
#include <stdio.h>
#include <stdlib.h>
%}

%%
[0-9]+  {
           int n = atoi(yytext);
           if(n%2==0) printf("Even\n");
           else printf("Odd\n");
        }
.|\n    ;
%%

int main(){ yylex(); return 0; }
