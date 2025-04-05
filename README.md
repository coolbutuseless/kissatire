
<!-- README.md is generated from README.Rmd. Please edit that file -->

# kissatire

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
[![CRAN](https://www.r-pkg.org/badges/version/kissatire)](https://CRAN.R-project.org/package=kissatire)
[![R-CMD-check](https://github.com/coolbutuseless/kissatire/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coolbutuseless/kissatire/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

`{kissatire}` is a SAT solver for R using the
[Kissat](https://github.com/arminbiere/kissat) library.

See the [`{satire}` package](https://github.com/coolbutuseless/satire)
for a more comprehensive introduction to building SAT problems in R.

**Note:** The Kissat C library is currently not-compatible with Windows
and this package will not work on that platform. Try
[picosatire](https://github.com/coolbutuseless/picosatire) instead.

This packages includes Kissat v4.0.2.

### What’s in the box

- `kis_solve_literals()` solve problems which are presented as a
  sequence of literals (i.e. an integer vector)
- `kis_solve_satire()` solve problems developed as a `sat` object with
  the [`{satire}` package](https://github.com/coolbutuseless/satire)
- `kis_version()` returns version information about the Kissat C library
  included in this package.

## Installation

<!-- This package can be installed from CRAN -->

<!-- ``` r -->

<!-- install.packages('kissatire') -->

<!-- ``` -->

You can install the latest development version from
[GitHub](https://github.com/coolbutuseless/kissatire) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/kissatire')
```

Pre-built source/binary versions can also be installed from
[R-universe](https://r-universe.dev)

``` r
install.packages('kissatire', repos = c('https://coolbutuseless.r-universe.dev', 'https://cloud.r-project.org'))
```

### Example: `kis_solve_literals()`

Integer literals are a standard encoding for SAT solving.

Using literals involves developing Boolean expressions in conjunctive
normal form. Variables are then replaced by integers - the positive
version of the integer indicating `TRUE` and the negative version
indicates `FALSE`. Zeros are used to represent logical `AND`.

When is the following expression true?

`(a | b) & (a | !c)`

Let:

- a = 1
- b = 2
- c = 3

So expresion becomes: `(1 | 2) & (1 | -3)`.

Drop the logical `OR` statements, replace logical `AND` with `0`, and
add a trailing zero on the end. I.e.

`1 2 0 1 -3 0`

Asking PicoSAT to solve this expression:

``` r
kis_solve_literals(c(1L, 2L, 0L, 1L, -3L, 0L), max_solutions = 8)
```

    #> [[1]]
    #> [1] 1 2 3
    #> 
    #> [[2]]
    #> [1] -1  2 -3
    #> 
    #> [[3]]
    #> [1]  1 -2 -3
    #> 
    #> [[4]]
    #> [1]  1 -2  3
    #> 
    #> [[5]]
    #> [1]  1  2 -3

This indicates that there are 5 solutions to the problem. The second
solution `-1 2 -3` says that when `a = FALSE, b = TRUE and c = FALSE`
the original statement is `TRUE`

### Example: `kis_solve_satire()`

Using `{satire}` to define the problem makes creation easier and the
solution easier to interpret.

``` r
library(satire)
sat <- sat_new()
sat_add_exprs(sat, "(a | b) & (a | !c)")
sat
```

    #> <sat> vars = 3 , clauses = 2

``` r
kis_solve_satire(sat, max_solutions = 8)
```

    #>       a     b     c
    #> 1  TRUE  TRUE  TRUE
    #> 2 FALSE  TRUE FALSE
    #> 3  TRUE FALSE FALSE
    #> 4  TRUE FALSE  TRUE
    #> 5  TRUE  TRUE FALSE

### Example: Unsatisfiable problems

If the SAT problem is unsatisfiable, the returned solution will be
`NULL`

E.g.

``` r
library(satire)
sat <- sat_new()
sat_add_exprs(sat, "a & !a")
sat
```

    #> <sat> vars = 1 , clauses = 2

``` r
kis_solve_satire(sat, max_solutions = 8)
```

    #> NULL
