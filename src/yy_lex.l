%{
    #include "../include/type.h"
    #define YYSTYPE char*
    extern YYSTYPE yylval;
%}

%%
[\t ]+
[\n]+
OBJECT\sIDENTIFIER
OBJECT-TYPE                         { return OBJ_SPECIFIER; }
SYNTAX                              { return SYNTAX_SPECIFIER; }
INTEGER.*                           { return SYNTAX_VALUE; }
MAX-ACCESS                          { return ACCESS_SPECIFIER; }

read |
write |
read-write |
not-accessible                      { return ACCESS_VALUE; }

STATUS                              { return STATUS_SPECIFIER; }
current                             { return STATUS_VALUE; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"(\n|.)*\"                         { return DESC_VALUE; }
::=                                 { return ASSIGNED; }
\{.*\}                              { yylval = yytext; return MOUNT_POINT; }
[a-zA-Z]+                           { return IDENTIFIER; }
%%

#define YYSTYPE char*
YYSTYPE yylval;
int yywrap() {
    return 1;
}
