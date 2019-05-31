#ifndef TYPE_SYNTH_H
#define TYPE_SYNTH_H

#include <stdbool.h>

#define BUFFER_SIZE 32

typedef enum {ERR_ARITH,
              ERR_TYPE,
              INT_TYPE,
              BOOL_TYPE,
              FLOAT_TYPE,
              UNKNOWN,
              STRING_TYPE}
type_synth;

typedef union {
  int int_value;
  float float_value;
  bool bool_value;
  char string_value[BUFFER_SIZE];
} types;

#endif
