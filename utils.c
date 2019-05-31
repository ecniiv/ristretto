#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define U4_SIZE   4
#define U2_SIZE   2
#define U1_SIZE   1

ristretto *new_ristretto() {
  ristretto *ris = malloc(sizeof(ristretto));
  ris -> out = NULL;
  ris -> size_index = 0;
  ris -> magic_number = MAGIC_NUMBER;
  ris -> version = VERSION;
  ris -> index = 2;
  ris -> main_index = 0;
  ris -> n_class = 0;
  ris -> n_const = 0;
  ris -> code_size = 1;
  ris -> offset_code_size = 0;
  return ris;
}

int ristretto_set_output(ristretto *ris, FILE *out) {
  ris -> out = out;
  fwrite(ris -> magic_number, U4_SIZE, 1, ris -> out);
  fwrite(ris -> version, U4_SIZE, 1, ris -> out);
  unsigned char constant_pool[] = "\x00\x00";
  fwrite(constant_pool, U2_SIZE, 1, ris -> out);
  return 0;
}

int ristretto_add_constant_pool(ristretto *ris, char *type) {
  if (strcmp(type, CLASS) == 0) {
    ris -> n_class += 1;
  } else if (strcmp(type, CONST) == 0) {
    ris -> n_const += 1;
  }
  fwrite(type, U2_SIZE, 1, ris -> out);
  fwrite(&(ris -> index), U1_SIZE, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_entry(ristretto *ris, char *s) {
  if (strcmp("main", s) == 0) {
    ris -> main_index = ris -> index;
  }
  fwrite(POOL_ENTRY, U2_SIZE, 1, ris -> out);
  unsigned int size = strlen(s);
  fwrite(&size, U1_SIZE, 1, ris -> out);
  fwrite(s, size, U1_SIZE, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_reference(ristretto *ris) {
  fwrite(REFER, U2_SIZE, 1, ris -> out);
  unsigned int d = ris -> index - 3;
  fwrite(&d, U2_SIZE, 1, ris -> out);
  fwrite(&(ris -> index), U1_SIZE, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_method(ristretto *ris) {
  fwrite(METHD, U2_SIZE, 1, ris -> out);
  unsigned int d = ris -> index - 3;
  fwrite(&d, U2_SIZE, 1, ris -> out);
  fwrite(&(ris -> index), U1_SIZE, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_name_type(ristretto *ris) {
  int d = ris -> index;
  int d2 = ris -> index + 1;
  fwrite(NA_TY, U2_SIZE, 1, ris -> out);
  fwrite(&d, U2_SIZE, 1, ris -> out);
  fwrite(&d2, U1_SIZE, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_getstaticout(ristretto *ris) {
  unsigned char static_out[] = "\xb2\x00\x0b";
  fwrite(static_out, U2_SIZE + U1_SIZE, 1, ris -> out);
  ris -> code_size += U2_SIZE + U1_SIZE;
  return 0;
}

int ristretto_println(ristretto *ris, unsigned int index) {
  unsigned char ldc[] = "\x12";
  unsigned char invoke[] = "\xb6\x00\x11";
  fwrite(ldc, U1_SIZE, 1, ris -> out);
  unsigned int d = index - 1;
  fwrite(&d, U1_SIZE, 1, ris -> out);
  fwrite(invoke, U2_SIZE + U1_SIZE, 1, ris -> out);
  ris -> code_size += U1_SIZE + U1_SIZE + U2_SIZE + U1_SIZE;
  return 0;
}

int ristretto_print(ristretto *ris, unsigned int index) {
  unsigned char ldc[] = "\x12";
  unsigned char invoke[] = "\xb6\x00\x17";
  fwrite(ldc, U1_SIZE, 1, ris -> out);
  unsigned int d = index - 1;
  fwrite(&d, U1_SIZE, 1, ris -> out);
  fwrite(invoke, U2_SIZE + U1_SIZE, 1, ris -> out);
  ris -> code_size += U1_SIZE + U1_SIZE + U2_SIZE + U1_SIZE;
  return 0;
}

int ristretto_println_int(ristretto *ris, unsigned int i) {
  unsigned int d = i + 3;
  unsigned char invoke[] = "\xb6\x00\x1d";
  fwrite(&d, U1_SIZE, 1, ris -> out);
  fwrite(invoke, U2_SIZE + U1_SIZE, 1, ris -> out);
  ris -> code_size += U1_SIZE + U2_SIZE + U1_SIZE;
  return 0;
}

int ristretto_print_int(ristretto *ris, unsigned int i) {
  unsigned int d = i + 3;
  unsigned char invoke[] = "\xb6\x00\x23";
  fwrite(&d, U1_SIZE, 1, ris -> out);
  fwrite(invoke, U2_SIZE + U1_SIZE, 1, ris -> out);
  ris -> code_size += U1_SIZE + U2_SIZE + U1_SIZE;
  return 0;
}

int ristretto_write_super_class(ristretto *ris) {
  unsigned char e[] = "\x00\x21\x00\x01\x00\x03\x00\x00\x00\x00\x00\x01";
  fwrite(e, U4_SIZE * 3, 1, ris -> out);
  return 0;
}

int ristretto_write_main(ristretto *ris) {
  fwrite(ACC_PUBLIC_STATIC, 3, 1, ris -> out);
  int main1 = ris -> main_index - 1;
  int main2 = ris -> main_index;
  fwrite(&main1, U2_SIZE, 1, ris -> out);
  fwrite(&main2, U2_SIZE, 1, ris -> out);
  unsigned char attr_size[] = "\x01\x00";
  int code_attr = ris -> main_index + 1;
  fwrite(attr_size, U2_SIZE, 1, ris -> out);
  fwrite(&code_attr, U1_SIZE, 1, ris -> out);
  unsigned char s[] = "\x00\x00\x00";
  fwrite(s, U2_SIZE + U1_SIZE, 1, ris -> out);

  ris -> offset_code_size = ftell(ris -> out);
  unsigned char null[] = "\x00";
  fwrite(null, U1_SIZE, 1, ris -> out);
  unsigned char p[] = "\x00\x02\x00\x01\x00\x00\x00";
  fwrite(p, U4_SIZE + U2_SIZE + U1_SIZE, 1, ris -> out);

  fwrite(null, U1_SIZE, 1, ris -> out);
  return 0;
}

int ristretto_update_size_constal_pool(ristretto *ris) {
  fseek(ris -> out, 9, SEEK_SET);
  int d2 = ris -> index - 1;
  fwrite(&d2, U1_SIZE, 1, ris -> out);
  unsigned int code_attr_size = ris -> code_size + 12;
  fseek(ris -> out, ris -> offset_code_size, SEEK_SET);
  fwrite(&code_attr_size, U1_SIZE, 1, ris -> out);
  fseek(ris -> out, ris -> offset_code_size + 8, SEEK_SET);
  fwrite(&(ris -> code_size), U1_SIZE, 1, ris -> out);
  fseek(ris -> out, 0, SEEK_END);
  return 0;
}
