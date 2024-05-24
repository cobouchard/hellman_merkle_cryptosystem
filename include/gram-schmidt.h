#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include <gmp.h>

#define BASE_SIZE 10
#define DELTA 0.5

struct Vector{
    mpf_t coefficients[BASE_SIZE];
};

void init_vector(struct Vector* vector, int size);
void clear_vector(struct Vector* v, int size);
void print_vector(struct Vector* vector, int size);

void nearest_integer(mpf_t floaty, mpz_t result);
void norm2(struct Vector* vector, mpf_t result, int size);
void dot_product(struct Vector* a,struct Vector* b, mpf_t result, int size);
void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj, int size);
void get_u_ij(struct Vector* b[], struct Vector* b_prime[], int i, int j, mpf_t result, int size);

void gram_schmidt(struct Vector* v[], struct Vector* u[], int number_of_vectors);



void test_gram_schmidt();

#endif
