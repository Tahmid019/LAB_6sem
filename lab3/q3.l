%{
    #include <stdio.h>
%}

%%
[0-2]*[0-9]*[0-9]+\.[0-2]*[0-9]*[0-9]+\.[0-2]*[0-9]*[0-9]+\.[0-2]*[0-9]*[0-9]+ {
                                                                                printf("IP Valid: %s\n", yytext);
                                                                            }

[0-9 ]* {
    printf("IP Invalid: %s\n", yytext);
}   

. {
    printf("Invalid Input: %s\n", yytext);
}
%%

int main(int argc, char *argv){
    yylex();
    return 0;
}