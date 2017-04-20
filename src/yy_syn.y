%token IDENTIFIER OBJ_SPECIFIER
%token SYNTAX_SPECIFIER SYNTAX_VALUE
%token ACCESS_SPECIFIER ACCESS_VALUE
%token STATUS_SPECIFIER STATUS_VALUE
%token DESC_SPECIFIER DESC_VALUE
%token MOUNT_POINT ASSIGNED

%{
    #include <stdio.h>

    extern int yylex (void);
    #define YYSTYPE char *
    void yyerror(char const *s) {
        fprintf(stderr, "%s\n", s);
    }
%}
%%
OBJ :  HEAD BODY                        { printf("%s\n", "reduce to OBJ"); } ;
HEAD : IDENTIFIER OBJ_SPECIFIER         { printf("%s\n", "reduce to HEAD"); } ;
BODY : PROPERTY                         { printf("%s\n", "reduce to BODY"); };
PROPERTY :  SYNTAX PROPERTY |
            ACCESS PROPERTY |
            STATUS PROPERTY |
            DESCRIPTION PROPERTY |
            MOUNT PROPERTY |
            /* empty */                 { printf("%s\n", "reduce to PROPERTY"); };
SYNTAX : SYNTAX_SPECIFIER SYNTAX_VALUE  { printf("%s\n", "reduce to SYNTAX"); };
ACCESS : ACCESS_SPECIFIER ACCESS_VALUE  { printf("%s\n", "reduce to ACCESS"); };
STATUS : STATUS_SPECIFIER STATUS_VALUE  { printf("%s\n", "reduce to STATUS"); };
DESCRIPTION : DESC_SPECIFIER DESC_VALUE { printf("%s\n", "reduce to DESCRIPTION"); };
MOUNT : ASSIGNED MOUNT_POINT            { printf("%s\n", "reduce to MOUNT"); };
%%