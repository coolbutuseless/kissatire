#ifndef _cover_h_INCLUDED
#define _cover_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define COVER(COND) \
  ((COND) ? \
\
          (\
           Rprintf ("%s:%ld: %s: Coverage goal `%s' reached.\n", \
                    __FILE__, (long) __LINE__, __func__, #COND), \
           Rf_error ("cover"), (void) 0) \
          : (void) 0)

#ifdef COVERAGE
#define FLUSH_COVERAGE() \
  do { \
    void __gcov_dump (void); \
    __gcov_dump (); \
  } while (0)
#else
#define FLUSH_COVERAGE() \
  do { \
  } while (0)
#endif

#endif
