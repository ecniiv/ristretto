%{
  #include <limits.h>
  #include "typesynth.h"
  #include "ristretto.tab.h"
  void string_to_int(int *v, const char *s);
%}
%option noyywrap
%%
[[:digit:]]+                  {string_to_int(&(yylval.integer), yytext); return NUMBER;}
\;                            {return SYMB_END;}
\{                            {return S_BRACKET;}
\}                            {return E_BRACKET;}
\+                            {return ADD;}
\-                            {return SUB;}
\*                            {return MUL;}
\/                            {return DIV;}
\=                            {return DECL;}
\=\=                          {return EQ;}
"int"                         {return V_INT;}
"void"                        {return V_VOID;}
"string"                      {return V_STRING;}
"true"                        {return TRUE;}
"false"                       {return FALSE;}
"if"                          {return IF;}
"else"                        {return ELSE;}
\".*\"                        {strcpy(yylval.string, yytext); return STRING;}
"main(void)"                  {return MAIN_VOID;}
"print"                       {return PRINT;}
"println"                     {return PRINTLN;}
[a-zA-Z][a-zA-Z0-9_]*         {
    if (strlen(yytext) + 1 < MAX_BUFFER) {
      strcpy(yylval.string, yytext);
    } else {
      fprintf(stderr, "erreur string\n");
    }
    return ID;
}
[[:blank:]]
.|\n                          {return yytext[0];}
%%

void string_to_int(int *r, const char *s) {
  char *p;
  long v;
  errno = 0;
  v = strtol(s, &p, 10);
  if ((*p != '\0' || (errno == ERANGE && (v == LONG_MIN || v == LONG_MAX)))
      || (v < INT_MIN || v > INT_MAX)) {
    fprintf(stderr, "Error converting string to int\n");
    exit(EXIT_FAILURE);
  }
  *r = v;
}
