#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include <gmp.h>

#define BASE_SIZE 3

struct Vector{
    int size;
    mpz_t coefficients[BASE_SIZE];
};

void dot_product(struct Vector* a,struct Vector* b, mpz_t* result);
void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj);
void gram_schmidt(struct Vector* v[], struct Vector* u[]);
void init_vector(struct Vector* vector);
void print_vector(struct Vector* vector);
void test_gram_schmidt();

#endif