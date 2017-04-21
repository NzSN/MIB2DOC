%{
    #include "../include/type.h"
    #define YYSTYPE char*
    extern YYSTYPE yylval;
%}

%%
[\n ]+
[\t ]+
OBJECT[ \n\t\r]+IDENTIFIER          { return OBJ_IDEN_; }
OBJECT-TYPE                         { return OBJ_SPECIFIER; }
SYNTAX                              { return SYNTAX_SPECIFIER; }
INTEGER([ \n\t\r]*\(*.*\))* |
IpAddress |
MacAddress |
OCTET[ \n\t\r]+STRING([ \n\t\r]*\(.*\))* |
SEQUENCE[ \n\t\r]+OF[ \n\t\r]*[a-zA-Z]* |
RowStatus                           { printf("%s\n", "TYPE"); return TYPE; }
SEQUENCE                            { printf("%s\n", "SEQ"); return SEQ; }

INDEX                               { return INDEX_; }
MAX-ACCESS                          { return ACCESS_SPECIFIER; }
DEFINITIONS                         { return DEF; }
BEGIN                               { return BEGIN_; }
IMPORTS                             { printf("%s\n", "IMPORTS"); return IMPORTS_; }
FROM                                { return FROM_; }
\,                                  { printf("%s\n", "COMMA"); return COMMA; }
\;                                  { return SEMICOLON; }
"--".*                              { printf("%s:%s\n", "COMMENT", yytext); return COMMENT; }
END                                 { return END_; }

read-only |
write |
read-write |
accessible-for-notify |
not-accessible                      { return ACCESS_VALUE; }

STATUS                              { return STATUS_SPECIFIER; }
current                             { return STATUS_VALUE; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"[^\"]*\"{1}                       { return DESC_VALUE; }
::=                                 { printf("%s\n", "ASSIGNED"); return ASSIGNED; }

\{                                  { printf("%s\n", "LB"); return L_BRACE; }
\}                                  { printf("%s\n", "RB"); return R_BRACE; }

[0-9]+                              { printf("%s\n", "NUM"); return NUM; }

([a-zA-Z0-9]|"-")+                  { printf("%s\n", "IDENTIFIER"); return IDENTIFIER; }
%%
#define YYSTYPE char*
YYSTYPE yylval;
int yywrap() {
    return 1;
}
