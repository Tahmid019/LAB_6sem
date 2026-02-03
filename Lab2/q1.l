%{
#include <stdio.h>
int lines = 0;
int words = 0;
int letters = 0;
int specials = 0;
%}

%%

\n                  { lines++; }
[ \t]+              {}

[a-zA-Z0-9]+        {
                        words++;
                        letters += yyleng;
                    }

[^a-zA-Z0-9 \t\n]   { specials++; }
.                   {}

%%

int main() {
    yylex();

    printf("Results:\n");
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Letters: %d\n", letters);
    printf("Specials: %d\n", specials);

    return 0;
}

int yywrap(){
    return 1;
}
