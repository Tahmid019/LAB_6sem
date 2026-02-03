%{
#include <stdio.h>
%}

%%
[aeiouAEIOU]   { printf("%s : Vowel\n", yytext); }
[a-zA-Z]       { printf("%s : Consonant\n", yytext); }
[ \t\n]+       ;
.              ;
%%

int main() {
    printf("Enter a sentence:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}