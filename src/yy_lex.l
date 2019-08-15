%option header-file="include/lexer.h"
%option outfile="src/lexer.c"
%option yylineno

%{
    #include "yy_syn.def.h"
    #include "yy_syn.tab.h"
    #include "lexDeal.h"
    #include <malloc.h>

    #define yyHEADER_H
    #include "util.h"
    #include "mibTreeGen.h"
    #undef yyHEADER_H

%}

%x SMI_FIELD
%s IMPORT_FIELD
%s STATUS_FIELD
%s SEQUENCE_FIELD

%%

<IMPORT_FIELD>FROM                  { return FROM_; }
<IMPORT_FIELD>([a-zA-Z0-9]|"-")+    { yylval.str = strdup(yytext); return IDENTIFIER; }
<IMPORT_FIELD>\;                    { BEGIN(INITIAL); return SEMICOLON; }

<STATUS_FIELD>current |
<STATUS_FIELD>deprecated |
<STATUS_FIELD>obsolete |
<STATUS_FIELD>mandatory              { BEGIN(INITIAL); return STATUS_VALUE; }

<SEQUENCE_FIELD>OBJECT[ \T]+IDENTIFIER { yylval.str = strdup(yytext); return TYPE_BUILT_IN; }
<SEQUENCE_FIELD>\}                  { BEGIN(INITIAL); return R_BRACE; }

ENTERPRISE                          { return ENTERPRISE_SPECIFIER; }

"--".*                              { /* empty */ }
[\n ]+                              { /* empty */ }
[\t ]+                              { /* empty */ }
OBJECT[ \t]+IDENTIFIER              { return OBJ_IDEN_; }
OBJECT-TYPE                         { return OBJ_SPECIFIER; }
OBJECTS                             { return OBJECTS_; }
OBJECT                              { return OBJECT_; }
NOTIFICATION-TYPE                   { return NOTIFY_TYPE_SPECIFIER; }
TRAP-TYPE                           { return TRAP_TYPE_SPECIFIER; }
SYNTAX                              { return SYNTAX_SPECIFIER; }

DEFVAL                              { return DEFVAL_SPECIFIER; }
UNITS                               { return UNITS_SPECIFIER;  }
AUGMENTS                            { return AUGMENTS_; }

\|                                  { return OR; }

\(                                  { return L_PAREN; }
\)                                  { return R_PAREN; }
\.\.                                { return TO; }
SIZE                                { return SIZE; }

\'[a-fA-F0-9]+\'(h|H)               { return HEX_STRING; }

INTEGER                                 |
Integer32                               |
IpAddress                               |
OCTET[ \t]+STRING[ \t]*([ \t]*\(.*\))*  |
SEQUENCE[ \t]+OF[ \t]*[a-zA-Z0-9]*         { yylval.str = strdup(yytext); return TYPE_BUILT_IN; }

SEQUENCE                            { BEGIN(SEQUENCE_FIELD); return SEQ; }

INDEX                               { return INDEX_; }
ACCESS                              { return ACCESS_SPECIFIER_SHORT; }
MAX-ACCESS                          { return ACCESS_SPECIFIER; }
MIN-ACCESS                          { return MIN_ACCESS_SPECIFIER; }
DEFINITIONS                         { return DEF; }
BEGIN                               { return BEGIN_; }
IMPORTS                             { BEGIN(IMPORT_FIELD); return IMPORTS_; }
FROM                                { return FROM_; }
\,                                  { return COMMA; }
\;                                  { return SEMICOLON; }
END                                 { return END_; }

TEXTUAL-CONVENTION                  { return TC_SPECIFIER; }
DISPLAY-HINT                        { return DISPLAY_HINT; }
REFERENCE                           { return REFERENCE; }

MODULE-IDENTITY                     { return MOD_SPECIFIER; }
LAST-UPDATED                        { return LAST_UPDATED; }
ORGANIZATION                        { return ORGANIZATION; }
REVISION                            { return REVISION; }
CONTACT-INFO                        { return CONTACT_INFO; }

OBJECT-GROUP                        { return OBJ_GRP_SPECIFIER; }

NOTIFICATION-GROUP                  { return NOTIFY_GRP_SPECIFIER; }
NOTIFICATIONS                       { return NOTIFY_SPECIFIER; }

MODULE-COMPLIANCE                   { return COMPLIANCE_SPECIFIER; }
GROUP                               { return GROUP_SPECIFIER; }
MANDATORY-GROUPS                    { return MANDATORY_SPECIFIER; }
WRITE-SYNTAX                        { return WRITE_SPECIFIER; }
MODULE                              { return MODULE_SPECIFIER; }

OBJECT-IDENTITY                     { return OBJ_IDENTITY_SPECIFIER; }

IMPLED                              { return IMPLED; }

read-only               |
read-write              |
read-create             |
accessible-for-notify   |
not-accessible                       { yylval.str = strdup(yytext); return ACCESS_VALUE; }

STATUS                              { BEGIN(STATUS_FIELD); return STATUS_SPECIFIER; }
DESCRIPTION                         { return DESC_SPECIFIER; }
\"[^\"]*\"{1}                       { yylval.str = strdup(yytext); return STRING; }
::=                                 { return ASSIGNED; }

\{                                  { return L_BRACE; }
\}                                  { return R_BRACE; }

(-?)[0-9]+                          { yylval.str = strdup(yytext); return NUM; }
SMI                                 { BEGIN(SMI_FIELD); return SMI_SPECIFIER; }

([a-zA-Z0-9]|"-"|"_")+    { yylval.str = strdup(yytext); return IDENTIFIER; }

<SMI_FIELD>([0-9a-zA-Z]|"-")+       { BEGIN(INITIAL); yylval.str = strdup(yytext); return SMI_VAL; }

<<EOF>>                             {
                                        /* First to do a type recheck for those fail in
                                           first pass type checking */
                                        int isValid;
                                        char *typeName;
                                        dispatchParam *current = pendingTypes;
                                        breakNode();
                                        while (current = dispatchParamNext(current)) {
                                            typeName = (char *)disParamGet(current);
                                            isValid = typeCheck_isValid(MIB_TYPE_TBL_R, typeName);

                                            if (isValid == FALSE) {
                                                abortWithMsg("Unknow type: %s at %s : %d\n",
                                                             typeName,
                                                             SW_CUR_FILE_NAME(&swState),
                                                             yylineno);
                                            }
                                        }

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
