#include <stdio.h>
#include <stdlib.h>

#define MAGIC_NUMBER      "\xCA\xFE\xBA\xBE"
#define VERSION           "\x00\x00\x00\x34"

#define ACC_PUBLIC        "\x00\x01"
#define ACC_PUBLIC_STATIC "\x00\x09"

#define POOL_ENTRY        "\x01\x00"
#define CLASS             "\x07\x00"
#define CONST             "\x08\x00"
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

int ristretto_add_constant_pool(ristretto *ris, char *type);

int ristretto_add_entry(ristretto *ris, char *s);

int ristretto_add_reference(ristretto *ris);

int ristretto_add_method(ristretto *ris);

int ristretto_add_name_type(ristretto *ris);

int ristretto_load_const(ristretto *ris, int i);

int ristretto_getstaticout(ristretto *ris);

int ristretto_println(ristretto *ris, unsigned int index);

int ristretto_println_int(ristretto *ris, unsigned int i);

int ristretto_write_super_class(ristretto *ris);

int ristretto_write_main(ristretto *ris);

int ristretto_update_size_constal_pool(ristretto *ris);
