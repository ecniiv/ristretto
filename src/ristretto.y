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

  #define STACK_CAPACITY 50
  static int stack[STACK_CAPACITY];
  static size_t stack_size = 0;
%}
%union {
  int integer;
  char string[MAX_BUFFER];
  type_synth type;
}
%token NUMBER
%token SYMB_END
%token S_BRACKET E_BRACKET

%token ADD SUB MUL DIV DECL EQ

%token V_INT V_BOOL V_VOID V_STRING
%token TRUE FALSE
%token AND OR
%token IF ELSE

%token STRING ID MAIN_VOID PRINT PRINTLN

%type<integer> NUMBER
%type<string> STRING
%type<string> ID
%type<type> expr

%left AND OR
%left DECL
%left EQ

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

expr:
V_VOID MAIN_VOID S_BRACKET {
  ristretto_write_super_class(ris);
  ristretto_write_main(ris);
} | E_BRACKET {
  unsigned char end[] = "\xb1\x00\x00\x00\x00\x00\x00";
  fwrite(end, sizeof(end) - 1, 1, ris -> out);
} | V_BOOL ID DECL expr SYMB_END {
  if ($4 == INT_TYPE) {
    char *id = malloc(sizeof(MAX_SIZE_ID));
    strcpy(id, $2);
    if (linked_list_insert(ll, ris -> index, id, INT_TYPE) == -1) {
      fprintf(stderr, "%s already exists.\n", id);
      return -1;
    }
    stack_size--;
    ristretto_add_integer(ris, stack[stack_size]);
  }
} | V_INT ID DECL expr SYMB_END {
  if ($4 == INT_TYPE) {
    char *id = malloc(sizeof(MAX_SIZE_ID));
    strcpy(id, $2);
    if (linked_list_insert(ll, ris -> index, id, INT_TYPE) == -1) {
      fprintf(stderr, "%s already exists.\n", id);
      return -1;
    }
    stack_size--;
    ristretto_add_integer(ris, stack[stack_size]);
  }
} | V_STRING ID DECL STRING SYMB_END {
  int d = strlen(yylval.string);
  char *s = malloc(sizeof(MAX_BUFFER));
  char *q = s;
  for (int i = 0; i < (d - 2); i++) {
    *q = yylval.string[i +1];
    q++;
  }
  *q = '\0';
  char *id = malloc(sizeof(MAX_SIZE_ID));
  strcpy(id, $2);
  if (linked_list_insert(ll, ris -> index, id, STRING_TYPE) == -1) {
    fprintf(stderr, "%s already exists.\n", id);
    return -1;
  }
  ristretto_add_string(ris, s);
} | PRINT expr SYMB_END {
  if ($2 == FALSE_STATMENT) {
    stack_size--;
  } else if ($2 == INT_TYPE || $2 == BOOL_TYPE) {
    if (yylval.integer < 0 || yylval.integer > 5) {
      fprintf(stderr, "Only number between 0 and 5.\n");
      $$ = ERR_TYPE;
    }
    ristretto_getstaticout(ris);
    stack_size--;
    ristretto_iconst(ris, stack[stack_size]);
    ristretto_print_int(ris);
  } else if ($2 == ID_TYPE) {
    char *id = malloc(sizeof(MAX_SIZE_ID));
    strcpy(id, yylval.string);
    ristretto_getstaticout(ris);
    int d = linked_list_search(ll, id);
    if (d == -1) {
      fprintf(stderr, "%s unknow.\n", id);
      return -1;
    }
    int type = linked_list_type_of_id(ll, id);
    ristretto_load_const(ris, d);
    if (type == STRING_TYPE) {
      ristretto_print(ris);
    } else if (type == INT_TYPE) {
      ristretto_print_int(ris);
    }
  }
} | PRINTLN expr SYMB_END {
  if ($2 == FALSE_STATMENT) {
    stack_size--;
  } else if ($2 == INT_TYPE || $2 == BOOL_TYPE) {
    if (yylval.integer < 0 || yylval.integer > 5) {
      fprintf(stderr, "Only number between 0 and 5.\n");
      $$ = ERR_TYPE;
    }
    ristretto_getstaticout(ris);
    stack_size--;
    ristretto_iconst(ris, stack[stack_size]);
    ristretto_println_int(ris);
  } else if ($2 == ID_TYPE) {
    char *id = malloc(sizeof(MAX_SIZE_ID));
    strcpy(id, yylval.string);
    ristretto_getstaticout(ris);
    int d = linked_list_search(ll, id);
    if (d == -1) {
      fprintf(stderr, "%s unknow.\n", id);
      return -1;
    }
    int type = linked_list_type_of_id(ll, id);
    ristretto_load_const(ris, d);
    if (type == STRING_TYPE) {
      ristretto_println(ris);
    } else if (type == INT_TYPE) {
      ristretto_println_int(ris);
    }
  }
} | '(' expr ')' {
  $$ = $2;
} | expr ADD expr {
  if ($1 == INT_TYPE && $3 == INT_TYPE) {
    int a = stack[stack_size - 1];
    stack_size--;
    int b = stack[stack_size - 1];
    stack[stack_size - 1] = b + a;
    $$ = INT_TYPE;
  } else if ($1 == ID_TYPE && $3 == ID_TYPE) {
    fprintf(stdout, "wait\n");
    $$ = INT_TYPE;
  }
} | expr SUB expr {
  if ($1 == BOOL_TYPE || $3 == BOOL_TYPE) {
    $$ = ERR_TYPE;
  } else if ($1 != INT_TYPE) {
    $$ = $1;
  } else if ($3 != INT_TYPE) {
    $$ = $3;
  } else if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    int a = stack[stack_size - 1];
    stack_size--;
    int b = stack[stack_size - 1];
    stack[stack_size - 1] = b - a;
    $$ = INT_TYPE;
  }
} | expr MUL expr {
  if ($1 == BOOL_TYPE || $3 == BOOL_TYPE) {
    $$ = ERR_TYPE;
  } else if ($1 != INT_TYPE) {
    $$ = $1;
  } else if ($3 != INT_TYPE) {
    $$ = $3;
  } else if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    int a = stack[stack_size - 1];
    stack_size--;
    int b = stack[stack_size - 1];
    stack[stack_size - 1] = a * b;
    $$ = INT_TYPE;
  }
} | expr DIV expr {
  if ($1 == BOOL_TYPE || $3 == BOOL_TYPE) {
    $$ = ERR_TYPE;
  } else if ($1 != INT_TYPE) {
    $$ = $1;
  } else if ($3 != INT_TYPE) {
    $$ = $3;
  } else if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    int a = stack[stack_size - 1];
    stack_size--;
    int b = stack[stack_size - 1];
    stack[stack_size - 1] = b / a;
    $$ = INT_TYPE;
  }
} | expr EQ expr {
  if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    int a = stack[stack_size - 1];
    stack_size--;
    int b = stack[stack_size - 1];
    stack[stack_size  - 1] = (b == a);
    $$ = INT_TYPE;
  }
} | expr AND expr {
  if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    if ($1 == INT_TYPE && $3 == INT_TYPE) {
      int a = stack[stack_size - 1];
      stack_size--;
      int b = stack[stack_size - 1];
      stack[stack_size  - 1] = (b && a);
      $$ = INT_TYPE;
    }
  }
} | expr OR expr {
  if (stack_size < 2) {
    $$ = ERR_STACK;
  } else {
    if ($1 == INT_TYPE && $3 == INT_TYPE) {
      int a = stack[stack_size - 1];
      stack_size--;
      int b = stack[stack_size - 1];
      stack[stack_size  - 1] = (b || a);
      $$ = INT_TYPE;
    }
  }
} | NUMBER {
  if (stack_size == STACK_CAPACITY) {
    fprintf(stderr, "ERR_STACK");
    $$ = ERR_STACK;
  } else {
    stack[stack_size] = $1;
    ++stack_size;
    $$ = INT_TYPE;
  }
} | ID {
  $$ = ID_TYPE;
} | STRING {
  $$ = STRING_TYPE;
} | TRUE {
  if (stack_size == STACK_CAPACITY) {
    fprintf(stderr, "ERR_STACK");
    $$ = ERR_STACK;
  } else {
    stack[stack_size] = 1;
    ++stack_size;
    $$ = INT_TYPE;
  }
} | FALSE {
  if (stack_size == STACK_CAPACITY) {
    fprintf(stderr, "ERR_STACK");
    $$ = ERR_STACK;
  } else {
    stack[stack_size] = 0;
    ++stack_size;
    $$ = INT_TYPE;
  }
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

  ristretto_add_class(ris);
  char *class = ris_to_name(argv[1]);
  ristretto_add_entry(ris, class);

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/lang/Object");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "<init>");
  ristretto_add_entry(ris, "()V");

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/lang/System");
  ristretto_add_reference(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "out");
  ristretto_add_entry(ris, "Ljava/io/PrintStream;");

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/io/PrintStream");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "println");
  ristretto_add_entry(ris, "(Ljava/lang/String;)V");

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/io/PrintStream");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "print");
  ristretto_add_entry(ris, "(Ljava/lang/String;)V");

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/io/PrintStream");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "println");
  ristretto_add_entry(ris, "(I)V");

  ristretto_add_class(ris);
  ristretto_add_entry(ris, "java/io/PrintStream");
  ristretto_add_method(ris);
  ristretto_add_name_type(ris);
  ristretto_add_entry(ris, "print");
  ristretto_add_entry(ris, "(I)V");

  ristretto_add_entry(ris, "main");
  ristretto_add_entry(ris, "([Ljava/lang/String;)V");
  ristretto_add_entry(ris, "Code");

  yyparse();

  ristretto_update_size_constant_pool(ris);

  linked_list_dispose(&ll);
  free(ris);
  fclose(yyin);
  fclose(dest);
  return 0;
}
