#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include <gmp.h>

mpz_t* dot_product(mpz_t* a[],mpz_t* b[]);
void vector_projection(mpz_t* u[], mpz_t* v[], mpz_t* proj[]);
void gram_schmidt(mpz_t* v[], mpz_t* u[]);

#endif