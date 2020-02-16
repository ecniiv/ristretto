#include <stdio.h>
#include <stdlib.h>

#define MAGIC_NUMBER      "\xCA\xFE\xBA\xBE"
// Java 8
#define VERSION           "\x00\x00\x00\x34"

#define ACC_PUBLIC        "\x00\x01"
#define ACC_PUBLIC_STATIC "\x00\x09"

#define POOL_ENTRY        "\x01\x00"
#define INTEGER_CONST     "\x03\x00"
#define CLASS             "\x07\x00"
#define STRING_CONST      "\x08\x00"
#define REFER             "\x09\x00"
#define METHD             "\x0a\x00"
#define NA_TY             "\x0c\x00"

typedef struct {
  FILE *out;
  char *bytecode;
  int size_index;
  char *magic_number;
  char *version;
  int index;
  int main_index;
  int n_class;
  int n_const;
  int code_size;
  long int offset_code_size;
} ristretto;

//  new_ristretto: Renvoie un pointeur sur une structure ristretto vide, NULL en
//    cas d'échec.
ristretto *new_ristretto();

//  ristretto_set_output: Initialise la sortie où le bytecode est écrit à out.
extern int ristretto_set_output(ristretto *ris, FILE *out);

//  ristretto_add_entry: Ajoute une entrée UTF8 s au constant pool.
extern int ristretto_add_entry(ristretto *ris, char *s);

//  ristretto_add_string: Ajoute une string s au constant pool.
extern int ristretto_add_string(ristretto *ris, char *s);

//  ristretto_add_integer: Ajoute un entier i au constant pool
extern int ristretto_add_integer(ristretto *ris, unsigned int i);

//  ristretto_add_x: Ajoute une (classe|référence|méthode|nom & type) au
//    constant pool.
extern int ristretto_add_class(ristretto *ris);
extern int ristretto_add_reference(ristretto *ris);
extern int ristretto_add_method(ristretto *ris);
extern int ristretto_add_name_type(ristretto *ris);

//  ristretto_load_const: Charge la constante du constant pool à l'index index.
extern int ristretto_load_const(ristretto *ris, unsigned int index);

//  ristretto_iconst: Fait appel à iconst_i en Java.
extern int ristretto_iconst(ristretto *ris, unsigned int i);

//  ristretto_getstaticout: Fait appel getstaticout en Java.
extern int ristretto_getstaticout(ristretto *ris);

//  ristretto_print*: Ces 4 fonctions font appel aux fonctions :
//    System.out.println(String), System.out.print(String),
//    System.out.println(Integer) et System.out.print(Integer)
extern int ristretto_println(ristretto *ris);
extern int ristretto_print(ristretto *ris);
extern int ristretto_println_int(ristretto *ris);
extern int ristretto_print_int(ristretto *ris);

//  ristretto_write_super_class: Fait appel à super() en java.
extern int ristretto_write_super_class(ristretto *ris);

//  ristretto_write_main: Initialise la fonction main ainsi que l'attribut "Code"
extern int ristretto_write_main(ristretto *ris);

//  ristretto_update_size_constant_pool: Modifie la taille du constant pool
//    selon le nombre d'index enregistré dans ris.
extern int ristretto_update_size_constant_pool(ristretto *ris);
