
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>


extern SEXP kis_version_(void);
extern SEXP kis_solve_(SEXP literals_, SEXP limit_, SEXP max_literal);

static const R_CallMethodDef CEntries[] = {
  
  {"kis_version_"    , (DL_FUNC) &kis_version_   , 0},
  {"kis_solve_"      , (DL_FUNC) &kis_solve_     , 3},
  
  {NULL , NULL, 0}
};


void R_init_kissatire(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



