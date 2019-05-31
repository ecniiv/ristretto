#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

ristretto *new_ristretto() {
  ristretto *ris = malloc(768);
  ris -> out = NULL;
  ris -> size_index = 0;
  ris -> magic_number = MAGIC_NUMBER;
  ris -> version = VERSION;
  ris -> index = 2;
  ris -> main_index = 0;
  ris -> n_class = 0;
  ris -> n_const = 0;
  ris -> code_size = 1;
  return ris;
}

int ristretto_set_output(ristretto *ris, FILE *out) {
  ris -> out = out;
  fwrite(ris -> magic_number, 4, 1, ris -> out);
  fwrite(ris -> version, 4, 1, ris -> out);
  unsigned char constant_pool[] = "\x00\x00";
  fwrite(constant_pool, 2, 1, ris -> out);
  return 0;
}

int ristretto_add_constant_pool(ristretto *ris, char *type) {
  if (strcmp(type, CLASS) == 0) {
    ris -> n_class += 1;
  } else if (strcmp(type, CONST) == 0) {
    ris -> n_const += 1;
  }
  fwrite(type, 2, 1, ris -> out);
  fwrite(&(ris -> index), 1, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_entry(ristretto *ris, char *s) {
  if (strcmp("main", s) == 0) {
    ris -> main_index = ris -> index;
  }
  fwrite(POOL_ENTRY, 2, 1, ris -> out);
  unsigned int size = strlen(s);
  fwrite(&size, 1, 1, ris -> out);
  fwrite(s, size, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_reference(ristretto *ris) {
  fwrite(REFER, 2, 1, ris -> out);
  unsigned int d = ris -> index - 3;
  fwrite(&d, 2, 1, ris -> out);
  fwrite(&(ris -> index), 1, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_method(ristretto *ris) {
  fwrite(METHD, 2, 1, ris -> out);
  unsigned int d = ris -> index - 3;
  fwrite(&d, 2, 1, ris -> out);
  fwrite(&(ris -> index), 1, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_add_name_type(ristretto *ris) {
  int d = ris -> index;
  int d2 = ris -> index + 1;
  fwrite(NA_TY, 2, 1, ris -> out);
  fwrite(&d, 2, 1, ris -> out);
  fwrite(&d2, 1, 1, ris -> out);
  ris -> index += 1;
  return 0;
}

int ristretto_update_size_constal_pool(ristretto *ris) {
  fseek(ris -> out, 9, SEEK_SET);
  int d2 = ris -> index - 1;
  fwrite(&d2, 1, 1, ris -> out);
  fseek(ris -> out, 0, SEEK_END);
  return 0;
}

int ristretto_write_super_class(ristretto *ris) {
  unsigned char e[] = "\x00\x21\x00\x01\x00\x03\x00\x00\x00\x00\x00\x01";
  fwrite(e, 12, 1, ris -> out);
  return 0;
}

int ristretto_getstaticout(ristretto *ris) {
  unsigned char static_out[] = "\xb2\x00\x0b";
  fwrite(static_out, 3, 1, ris -> out);
  return 0;
}

int ristretto_println(ristretto *ris, unsigned int index) {
  unsigned char ldc[] = "\x12";
  unsigned char invoke[] = "\xb6\x00\x11";
  fwrite(ldc, 1, 1, ris -> out);
  unsigned int d = index - 1;
  fwrite(&d, 1, 1, ris -> out);
  fwrite(invoke, 3, 1, ris -> out);
  return 0;
}

int ristretto_write_main(ristretto *ris) {
  fwrite(ACC_PUBLIC_STATIC, 3, 1, ris -> out);
  int main1 = ris -> main_index - 1;
  int main2 = ris -> main_index;
  fwrite(&main1, 2, 1, ris -> out);
  fwrite(&main2, 2, 1, ris -> out);
  unsigned char attr_size[] = "\x01\x00";
  int code_attr = ris -> main_index + 1;
  fwrite(attr_size, sizeof(attr_size) - 1, 1, ris -> out);
  fwrite(&code_attr, 1, 1, ris -> out);
  unsigned char s[] = "\x00\x00\x00";
  fwrite(s, sizeof(s) - 1, 1, ris -> out);

  unsigned int code_attr_size = ris -> code_size + 12;
  fwrite(&code_attr_size, 1, 1, ris -> out);
  unsigned char p[] = "\x00\x02\x00\x01\x00\x00\x00";
  fwrite(p, sizeof(p) - 1, 1, ris -> out);

  fwrite(&(ris -> code_size), 1, 1, ris -> out);
  return 0;
}
