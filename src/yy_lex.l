%option header-file="include/lexer.h"
%option outfile="src/lexer.c"

%{
    #include "yy_syn.def.h"
    #include "yy_syn.tab.h"
    #include "lexDeal.h"

    void * getCurSwInfo();
%}

%x SMI_FIELD

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
INTEGER32                   |
IpAddress                   | 
MacAddress                  | 
OCTET[ \n\t\r]+STRING[ \n\t\r]*([ \n\t\r]*\(.*\))*    | 
SEQUENCE[ \n\t\r]+OF[ \n\t\r]*[a-zA-Z]*     | 
RowStatus                           { yylval.str = strdup(yytext); return TYPE_BUILT_IN; }

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

TEXTUAL-CONVENTION                  { return TC_SPECIFIER; }
DISPLAY-HINT                        { return DISPLAY_HINT; }
REFERENCE                           { return REFERENCE; }

MODULE-IDENTITY                     { return MOD_SPECIFIER; }
LAST-UPDATED                        { return LAST_UPDATED; }
\"[0-9a-zA-Z]*\"[ \t]*--[ \t]*[0-9a-zA-Z\t ,]* { return REVISION_DATE; }
ORGANIZATION                        { return ORGANIZATION; }
REVISION                            { return REVISION; }
CONTACT-INFO                        { return CONTACT_INFO; }

read-only               |
write                   |
read-write              |
accessible-for-notify   |
not-accessible                      { yylval.str = strdup(yytext); return ACCESS_VALUE; }

STATUS                              { return STATUS_SPECIFIER; }
current                             { return STATUS_VALUE; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"[^\"]*\"{1}                       { yylval.str = strdup(yytext); return DESC_VALUE; }
::=                                 { return ASSIGNED; }

\{                                  { return L_BRACE; }
\}                                  { return R_BRACE; }

[0-9]+                              { yylval.str = strdup(yytext); return NUM; }

SMI                                 { BEGIN(SMI_FIELD); return SMI_SPECIFIER; }

([a-zA-Z0-9]|"-")+                  { yylval.str = strdup(yytext); return IDENTIFIER; }

<SMI_FIELD>([0-9a-zA-Z]|"-")+       { BEGIN(INITIAL); yylval.str = strdup(yytext); return SMI_VAL; }

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
