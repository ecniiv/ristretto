%{
  #include "utils.h"
  #include "linkedList.h"
  #include <ctype.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "typesynth.h"
  int yylex(void);
  void yyerror(char const *);
  ristretto *ris;
  linked_list *ll;
  extern FILE *yyin;
%}
%union {
  int integer;
  float floatant;
  char string[BUFFER_SIZE];
  types t;
  type_synth type;
}
%token NUMBER
%token V_STRING
%token V_INT
%token V_VOID
%token V_BOOL

%token S_BRACKET E_BRACKET
%left S_BRACKET E_BRACKET
%token NOT DECL EQ NEQ LEQ GEQ AND OR TRUE FALSE ID ADD SUB MUL DIV SYMB_END IF ELSE STRING MAIN_VOID PRINTLN RETURN
%left OR
%left AND
%left DECL
%left EQ NEQ
%left LEQ GEQ
%precedence NOT
%left ADD SUB
%left MUL DIV
%left MAIN_VOID
%left PRINTLN
%type<string> STRING
%type<string> ID

%start lignes
%%
lignes:
lignes expr error '\n'
| lignes error '\n'
| expr error '\n'
| error '\n'
| lignes expr '\n'
| lignes '\n'
| expr '\n'
| '\n'
;

expr :
| V_VOID MAIN_VOID S_BRACKET {
  ristretto_write_super_class(ris);
  ris -> code_size += 8;
  ristretto_write_main(ris);
}
| E_BRACKET {
  unsigned char end[] = "\xb1\x00\x00\x00\x00\x00\x00";
  fwrite(end, sizeof(end) - 1, 1, ris -> out);
}
| IF '(' expr ')' '{' expr '}' {
  fprintf(stdout, "IF expr THEN expr\n");
}
| IF '(' expr ')' '{' expr '}' ELSE '{' expr '}' {
  fprintf(stdout, "IF expr THEN expr ELSE expr\n");
}
| V_INT ID {
  fprintf(stdout, "decl int\n");
}
| V_BOOL ID {
  fprintf(stdout, "decl bool\n");
}
| expr EQ expr {
  fprintf(stdout, "expr == expr");
}
| NUMBER {
  printf("\n");
}
| TRUE {

}
| FALSE {

}
| V_STRING ID DECL STRING {
  int d = strlen(yylval.string);
  char *s = malloc(sizeof(d));
  char *q = s;
  for (int i = 0; i < (d - 2); i++) {
    *q = yylval.string[i +1];
    q++;
  }
  *q = '\0';
  char *id = malloc(sizeof(16));
  strcpy(id, $2);
  if (linked_list_insert(ll, ris -> index, id, s) == -1) {
    fprintf(stderr, "%s already exists.\n", id);
    return -1;
  }
  ristretto_add_constant_pool(ris, CONST);
  ristretto_add_entry(ris, s);
}
| PRINTLN ID {
  ristretto_getstaticout(ris);
  int d = linked_list_search(ll, "p");
  ristretto_println(ris, d);
}
| PRINTLN STRING {
  int d = strlen(yylval.string);
  char s[d - 2];
  for (int i = 0; i < (d - 2); i++) {
    s[i] = yylval.string[i + 1];
  }
  s[d - 2] = '\0';
  printf("%s\n", s);
}
;
%%

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}

char *ris_to_class(char *s) {
  char *sep = ".";
  char *filename = strtok(s, sep);
  int j = toupper(filename[0]);
  filename[0] = j;
  char *extension = ".class";
  char *r = strcat(filename, extension);
  return r;
}

char *ris_to_name(char *s) {
  char *sep = ".";
  char *filename = strtok(s, sep);
  int j = toupper(filename[0]);
  filename[0] = j;
  return filename;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "USAGE: ./ristretto FILENAME.ris\n");
    return EXIT_FAILURE;
  }
  yyin = fopen(argv[1], "r");
  char *classname = ris_to_class(argv[1]);
  if (classname == NULL) {
    fprintf(stderr, "You have to specify a ris file\n");
    fclose(yyin);
    return EXIT_FAILURE;
  }
  FILE *dest = fopen(classname, "wb");
  ris = new_ristretto();
  ristretto_set_output(ris, dest);

  ll = linked_list_empty();

  ristretto_add_constant_pool(ris, CLASS);
  char *class = ris_to_name(argv[1]);
  ristretto_add_entry(ris, class);

  ristretto_add_constant_pool(ris, CLASS);
  ristretto_add_entry(ris, "java/lang/Object");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "<init>");
  ristretto_add_entry(ris, "()V");

  ristretto_add_constant_pool(ris, CLASS);
  ristretto_add_entry(ris, "java/lang/System");
  ristretto_add_reference(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "out");
  ristretto_add_entry(ris, "Ljava/io/PrintStream;");

  ristretto_add_constant_pool(ris, CLASS);
  ristretto_add_entry(ris, "java/io/PrintStream");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "println");
  ristretto_add_entry(ris, "(Ljava/lang/String;)V");

  ristretto_add_entry(ris, "println");
  ristretto_add_entry(ris, "(I)V");

  ristretto_add_entry(ris, "main");
  ristretto_add_entry(ris, "([Ljava/lang/String;)V");
  ristretto_add_entry(ris, "Code");

  char *p = "Helloooo Woooorld ! :)";
  linked_list_insert(ll, ris -> index, "p", p);
  ristretto_add_constant_pool(ris, CONST);
  ristretto_add_entry(ris, p);

  yyparse();

  // call system.out.println on the id p -> must increase code_size + 8;

  ristretto_update_size_constal_pool(ris);

  free(ris);

  fclose(dest);
  return 0;
}
