%{
#include <stdio.h>

int comment_count = 0;
%}

%%
"/*"([^*]|\*+[^*/])*\*+"/"   { comment_count++;}
"//".*                       { comment_count++;}
.                            { putchar(yytext[0]); }
\n                           { putchar('\n'); }    
%%

int main(int argc, char *argv[]) {
    yylex();
    printf("\ncomments: %d\n", comment_count);
    return 0;
}