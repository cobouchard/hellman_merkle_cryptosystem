#ifndef CRYPTOSYSTEM_H
#define CRYPTOSYSTEM_H

#include <gmp.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>


#define MESSAGE_LENGTH 100
#define DEFAULT_SIZE 1024UL

void super_increasing_sequence(mpz_t *sequence, mpz_t *q);
void find_coprime(mpz_t q, mpz_t res);
void public_sequence(mpz_t *sequence_a, mpz_t *sequence_b, const mpz_t q, const mpz_t r);
void str_to_binary(char *message, char* binary);
void encryption(mpz_t *pub_sequence, char *message, mpz_t cipher);
void subset_sum_solver(mpz_t c, int *indexes, mpz_t *sequence);
void decryption(mpz_t cipher, mpz_t *sequence, const mpz_t q, const mpz_t r, unsigned char *res);


#endif /* CRYPTOSYSTEM_H */