#include "witness.h"
#include "allocate.h"
#include "internal.h"

#include <stdio.h>
#include <string.h>

static void flush_buffer (chars *buffer) {
  // fputs ("v", stdout);
  Rprintf("v");
  for (all_stack (char, ch, *buffer)) {
    // fputc (ch, stdout);
    Rprintf("%c", ch);
  }
  // fputc ('\n', stdout);
  Rprintf("\n");
  CLEAR_STACK (*buffer);
}

static void print_int (kissat *solver, chars *buffer, int i) {
  char tmp[16];
  snprintf (tmp, sizeof(tmp), " %d", i);
  size_t tmp_len = strlen (tmp);
  size_t buf_len = SIZE_STACK (*buffer);
  if (buf_len + tmp_len > 77)
    flush_buffer (buffer);
  for (const char *p = tmp; *p; p++)
    PUSH_STACK (*buffer, *p);
}

void kissat_print_witness (kissat *solver, int max_var, bool partial) {
  chars buffer;
  INIT_STACK (buffer);
  for (int eidx = 1; eidx <= max_var; eidx++) {
    int tmp = kissat_value (solver, eidx);
    if (!tmp && !partial)
      tmp = eidx;
    if (tmp)
      print_int (solver, &buffer, tmp);
  }
  print_int (solver, &buffer, 0);
  assert (!EMPTY_STACK (buffer));
  flush_buffer (&buffer);
  RELEASE_STACK (buffer);
}
