%{
#include <stdio.h>
%}

%%
Hello    { printf("Bye\n"); }
.|\n     ;
%%

int main(){ yylex(); return 0; }
