
#define R_NO_REMAP

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include "kissat.h"

// From kitten.c
#define KIS_SATISFIABLE 10
#define KIS_UNSATISFIABLE 20



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Version
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP kis_version_(void) {
  const char *version = kissat_version();
  return Rf_mkString(version);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Solver
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP kis_solve_(SEXP literals_, SEXP limit_, SEXP max_literal_) {
  
  int nprotect = 0;
  kissat *kis = kissat_init();
  kissat_set_option(kis, "quiet", 1);
  
  int *literals = INTEGER(literals_);
  int n = Rf_length(literals_);
  
  for (int i = 0; i < n; i++) {
    kissat_add(kis, literals[i]);
  }
  
  // Insert a '0' at the end if there wasn't one already
  if (literals[n - 1] != 0) {
    kissat_add(kis, 0);
  }
  
  // solve it
  int res = kissat_solve(kis);
  // Rprintf("SAT?: %i\n", res);
  
  
  SEXP res_ = R_NilValue;
  
  if (res == KIS_SATISFIABLE) {
    
    int nlits = Rf_asInteger(max_literal_);
    res_ = PROTECT(Rf_allocVector(INTSXP, nlits)); nprotect++;
    int *resp = INTEGER(res_);
    
    for (int i = 1; i <= nlits; i++) {
      resp[i - 1] = kissat_value(kis, i);
    }
  } else if (res == KIS_UNSATISFIABLE) {
    // Unsatisfiable is a valid answer. return NULL
    res_ = R_NilValue;
  } else {
    Rf_error("kis_solve_(): SAT Unknown or Interrupted. [%i]", res);
  }
  
  
  kissat_release(kis);
  UNPROTECT(nprotect);
  return res_;
}





// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// // Called when a 'kis' external pointer is garbage collected
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void kis_finalizer(SEXP kis_) {
//   kissat *kis = R_ExternalPtrAddr(kis_);
//   if (kis != NULL) {
//     kissat_release(kis);
//     R_ClearExternalPtr(kis_);
//   }
// }
// 
// 
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// // 'kis' external pointer to C pointer
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// kissat *kis_extptr_to_cptr(SEXP kis_) {
//   if (!Rf_inherits(kis_, "kis")) {
//     Rf_error("Not a 'kis' context");
//   }
//   kissat *kis = TYPEOF(kis_) != EXTPTRSXP ? NULL : (kissat *)R_ExternalPtrAddr(kis_);
//   if (kis == NULL) Rf_error("'kis' pointer is invalid/NULL");
//   return kis;
// }
// 
// 





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Initialise a kissat session
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SEXP kis_init_(void) {
//   int nprotect = 0;
//   kissat *kis = kissat_init();
//   kissat_set_option(kis, "quiet", 1);
//   
//   
//   // Create external pointer
//   SEXP kis_ = PROTECT(R_MakeExternalPtr(kis, R_NilValue, R_NilValue)); nprotect++;
//   SEXP cls_ = PROTECT(Rf_mkString("kis")); nprotect++;
//   SET_CLASS(kis_, cls_);
//   R_RegisterCFinalizer(kis_, kis_finalizer);
//   
//   UNPROTECT(nprotect);
//   return kis_;
// }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Add a literal in CNF form
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SEXP kis_add_literals_(SEXP kis_, SEXP literals_) {
//   kissat *kis = kis_extptr_to_cptr(kis_);
//   
//   int *literals = INTEGER(literals_);
//   int n = Rf_length(literals_);
//   
//   for (int i = 0; i < n; i++) {
//     kissat_add(kis, literals[i]);
//   }
//   
//   // Insert a '0' at the end if there wasn't one already
//   if (literals[n - 1] != 0) {
//     kissat_add(kis, 0);
//   }
//   
//   return kis_;
// }



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Solve the current system
// @param kis_ kis object
// @param integer vector of literals
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SEXP kis_solve_(SEXP kis_, SEXP literals_) {
//   int nprotect = 0;
//   kissat *kis = kis_extptr_to_cptr(kis_);
//   
//   // solve it
//   int res = kissat_solve(kis);
//   // Rprintf("SAT?: %i\n", res);
//   
//   
//   SEXP res_ = R_NilValue;
//   
//   if (res == KIS_SATISFIABLE) {
//     
//     int *lits = INTEGER(literals_);
//     int nlits = Rf_length(literals_); 
//     
//     res_ = PROTECT(Rf_allocVector(INTSXP, nlits)); nprotect++;
//     int *resp = INTEGER(res_);
//     
//     for (int i = 0; i < nlits; i++) {
//       int lit = lits[i];
//       resp[i] = kissat_value(kis, lit);
//     }
//   } else if (res == KIS_UNSATISFIABLE) {
//     // Unsatisfiable is a valid answer. return NULL
//   } else {
//     Rf_error("kis_solve_(): SAT Unknown or Interrupted. [%i]", res);
//   }
//   
//   
//   UNPROTECT(nprotect);
//   return res_;
// }







