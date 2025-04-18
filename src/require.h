#ifndef _require_h_INCLUDED
#define _require_h_INCLUDED

#define kissat_require(COND, ...) \
  do { \
    if ((COND)) \
      break; \
    kissat_fatal_message_start (); \
    Rprintf ("calling '%s': ", __func__); \
    kissat_abort (); \
  } while (0)

#define kissat_require_initialized(SOLVER) \
  kissat_require (SOLVER, "uninitialized")

#define kissat_require_valid_external_internal(LIT) \
  do { \
    kissat_require ((LIT) != INT_MIN, "invalid literal '%d' (INT_MIN)", \
                    (LIT)); \
    const int TMP_IDX = ABS (LIT); \
    kissat_require (TMP_IDX <= EXTERNAL_MAX_VAR, \
                    "invalid literal '%d' (variable larger than %d)", \
                    (LIT), EXTERNAL_MAX_VAR); \
    assert (VALID_EXTERNAL_LITERAL (LIT)); \
  } while (0)

#endif
