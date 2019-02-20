%option header-file="include/lexer.h"
%option outfile="src/lexer.c"

%{
    #include "yy_syn.tab.h"
    #include "lexDeal.h"
%}

%%
[\n ]+
[\t ]+
"--".*
OBJECT[ \n\t\r]+IDENTIFIER          { return OBJ_IDEN_; }
OBJECT-TYPE                         { return OBJ_SPECIFIER; }
OBJECTS                             { return OBJECTS_; }
NOTIFICATION-TYPE                   { return TRAP_SPECIFIER; }
SYNTAX                              { return SYNTAX_SPECIFIER; }

INTEGER([ \n\t\r]*\(*.*\))* | 
IpAddress                   | 
MacAddress                  | 
OCTET[ \n\t\r]+STRING([ \n\t\r]*\(.*\))*    | 
SEQUENCE[ \n\t\r]+OF[ \n\t\r]*[a-zA-Z]*     | 
RowStatus                           { yylval = strDuplicate(yytext); return TYPE; }

SEQUENCE                            { return SEQ; }

INDEX                               { return INDEX_; }
MAX-ACCESS                          { return ACCESS_SPECIFIER; }
DEFINITIONS                         { return DEF; }
BEGIN                               { return BEGIN_; }
IMPORTS                             { return IMPORTS_; }
FROM                                { return FROM_; }
\,                                  { return COMMA; }
\;                                  { return SEMICOLON; }
END                                 { return END_; }

read-only               |
write                   |
read-write              |
accessible-for-notify   |
not-accessible                      { yylval = strDuplicate(yytext); return ACCESS_VALUE; }

STATUS                              { return STATUS_SPECIFIER; }
current                             { return STATUS_VALUE; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"[^\"]*\"{1}                       { yylval = strDuplicate(yytext); return DESC_VALUE; }
::=                                 { return ASSIGNED; }

\{                                  { return L_BRACE; }
\}                                  { return R_BRACE; }

[0-9]+                              { yylval = strDuplicate(yytext); return NUM; }

([a-zA-Z0-9]|"-")+                  { yylval = strDuplicate(yytext); return IDENTIFIER; }

<<EOF>>                             {
                                        if (switchToPrevModule(getCurSwInfo()) == -1) {
                                            yyterminate();
                                        }
                                    }

%%
int yywrap() {
    return 1;
}

YY_BUFFER_STATE getCurrentBufferState() {
    return YY_CURRENT_BUFFER;
}
