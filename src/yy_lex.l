%{
    #include "../include/type.h"
    #define YYSTYPE char*
    extern YYSTYPE yylval;
%}

%%
[\n ]+
[\t ]+
[\s ]+
OBJECT_IDENTIFIER                   { return OBJ_IDEN_; }
OBJECT-TYPE                         { return OBJ_SPECIFIER; }
SYNTAX                              { return SYNTAX_SPECIFIER; }
INTEGER.*                           { yylval = yytext; return SYNTAX_VALUE; }
MAX-ACCESS                          { return ACCESS_SPECIFIER; }

DEFINITIONS                         { return DEF; }

BEGIN                               { return BEGIN_; }
IMPORTS                             { return IMPORTS_; }
FROM                                { return FROM_; }
\,                                  { return COMMA; }
\;                                  { return SEMICOLON; }
END                                 { return END_; }

read |
write |
read-write |
not-accessible                      { yylval = yytext; return ACCESS_VALUE; }

STATUS                              { return STATUS_SPECIFIER; }
current                             { yylval = yytext; return STATUS_VALUE; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"[^\"]*\"{1}                       { yylval = yytext; return DESC_VALUE; }
::=                                 { return ASSIGNED; }

\{.*\}                              { yylval = yytext; return MOUNT_POINT; }
([a-zA-Z0-9]|"-")+                  { yylval = yytext; return IDENTIFIER; }
%%
#define YYSTYPE char*
YYSTYPE yylval;
int yywrap() {
    return 1;
}
