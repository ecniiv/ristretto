#ifndef TYPE_SYNTH_H
#define TYPE_SYNTH_H

#include <stdbool.h>

#define MAX_SIZE_ID   32
#define MAX_BUFFER    128

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
  char string_value[MAX_BUFFER];
} types;

#endif
