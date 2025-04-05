






#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Solve using literals
#' 
#' @param literals integer literals
#' @param max_solutions limit number of solutions
#' @return Return NULL is problem is unsatisfiable.  Otherwise return a list
#'         of integer sequences of literals - each sequence is a solution.
#' @examples
#' kis_solve_literals(c(1, 2, 0, 2, -3, 0))
#' @export
#' @import satire
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kis_solve_literals <- function(literals, max_solutions = 1) {
  
  sat <- satire::sat_new()
  satire::sat_add_literals(sat, as.integer(literals))
  
  all_res <- list()
  nsolns  <- 0L
  while(nsolns < max_solutions) {
    res <- .Call(kis_solve_, sat$literals, max_solutions, max_literal = max(abs(literals)))
    if (is.null(res)) break;
    nsolns <- nsolns + 1L
    all_res[[nsolns]] <- res
    
    ## Block solution
    if (max_solutions > 1) {
      satire::sat_block_solution(sat, res)  
    }
  }
  
  if (nsolns == 0) {
    return(NULL)
  } else if (max_solutions == 1 && length(all_res) == 1) {
    all_res[[1]]
  } else {
    all_res
  }
  
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Solve using 'satire' 
#' 
#' @inheritParams kis_solve_literals
#' @param sat SAT problem definition as created by \code{satire::sat_new()} or
#'        \code{satire::read_dimacs()}
#' @param remove regular expression for variables to remove when blocking solutions
#'        and assembling values to return. Default: "^dummy" will block all
#'        variables starting with the word "dummy" (as this is how the 'satire' 
#'        package automatically creates dummy variables.)
#'        If NULL no variables will be removed.
#' @return Return NULL is problem is unsatisfiable.  Otherwise return a data.frame
#'         of solutions where each column represents a named variable in the 
#'         problem and each row is a solution.
#' @examples
#' sat <- satire::sat_new()
#' satire::sat_add_exprs(sat, "a -> (b & c)")
#' kis_solve_satire(sat, max_solutions = 10)
#' satire::sat_solve_naive(sat)
#' satire::sat_solve_dpll(sat, max_solutions = 10)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kis_solve_satire <- function(sat, max_solutions = 1, remove = "^dummy") {
  
  # Within this function, operate on a copy of the SAT problem as 
  # we are going to iteratively block solutinos to find more of them -
  # thus changing the sat object.
  stopifnot(inherits(sat, "sat_prob"))
  sat <- satire::sat_copy(sat)
  
  if (is.null(remove) || nchar(remove) == 0) {
    keep_idxs <- seq_along(sat$names)
  } else {
    keep_idxs <- which(!grepl(remove, sat$names)) 
  }
  
  if (length(keep_idxs) == 0) {
    stop("No variable names to return. Empty problem, or 'remove' too aggressive")
  }
  
  max_literal <- max(abs(sat$literals))
  
  
  all_res <- list()
  nsolns <- 0L
  
  while (nsolns < max_solutions) {
    res <-.Call(kis_solve_, sat$literals, max_solutions, max_literal)
    if (is.null(res)) break;
    
    ## Block solution of the non-dummy variables
    satire::sat_block_solution(sat, res[keep_idxs])

    # Not entirely sure why, but kissat_value() sometimes returns 0
    # after doing a solve.  This may be values which are redundant and not needed
    # for solving?  Set these values to TRUE
    zero <- which(res == 0)
    if (length(zero) > 0) {
      res[zero] <- zero
    }    
        
    nsolns <- nsolns + 1L
    all_res[[nsolns]] <- res
  }  
  
  if (nsolns == 0) {
    # Unsatisfiable
    return(NULL)
  }
  
  
  # Map to named logicals
  all_res <- lapply(all_res, function(soln) {
    satire::sat_literals_to_lgl(sat, soln, remove = remove)
  })
  
  # Convert to data.frrame
  all_res <- as.data.frame(do.call(rbind, all_res))
  all_res <- all_res[!duplicated(all_res), , drop = FALSE]
  rownames(all_res) <- NULL
  
  all_res
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Return a list of version information of the kisSAT library
#' 
#' @return Named list of version information. \code{kisSAT_API_VERSION} is the
#'         internal C API version, and \code{release} is the numbering of the 
#'         software release tarball.
#' @examples
#' kis_version()
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kis_version <- function() {
  .Call(kis_version_);
}








