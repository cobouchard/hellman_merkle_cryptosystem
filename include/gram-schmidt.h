#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include <gmp.h>

#define BASE_SIZE 10

struct Vector{
    int size;
    mpz_t coefficients[BASE_SIZE];
};

mpz_t* dot_product(struct Vector* a,struct Vector* b);
void vector_projection(mpz_t* u[], mpz_t* v[], mpz_t* proj[]);
void gram_schmidt(struct Vector* v[], struct Vector* u[]);
void init_vector(struct Vector* vector);


#endif