#include <stdio.h>
#include <stdlib.h>

#define MAGIC_NUMBER      "\xCA\xFE\xBA\xBE"
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

ristretto *new_ristretto();

int ristretto_set_output(ristretto *ris, FILE *out);

int ristretto_write_all(ristretto *ris);

int ristretto_add_entry(ristretto *ris, char *s);

int ristretto_add_string(ristretto *ris, char *s);

int ristretto_add_integer(ristretto *ris, unsigned int i);

int ristretto_add_class(ristretto *ris);

int ristretto_add_reference(ristretto *ris);

int ristretto_add_method(ristretto *ris);

int ristretto_add_name_type(ristretto *ris);

int ristretto_load_const(ristretto *ris, unsigned int index);

int ristretto_iconst(ristretto *ris, unsigned int i);

int ristretto_getstaticout(ristretto *ris);

int ristretto_println(ristretto *ris);

int ristretto_print(ristretto *ris);

int ristretto_println_int(ristretto *ris);

int ristretto_print_int(ristretto *ris);

int ristretto_write_super_class(ristretto *ris);

int ristretto_write_main(ristretto *ris);

int ristretto_update_size_constal_pool(ristretto *ris);
