#ifndef CRYPTOSYSTEM_H
#define CRYPTOSYSTEM_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#include <gmp.h>

#define MESSAGE_LENGTH 96 // or 104
#define DEFAULT_SIZE 1024UL

void super_increasing_sequence(mpz_t *sequence, mpz_t q);
void find_coprime(mpz_t q, mpz_t res);
void public_sequence(mpz_t *sequence_a, mpz_t *sequence_b, const mpz_t q, const mpz_t r);
void initialisation(mpz_t *sequence, mpz_t q, mpz_t r, mpz_t *pub_sequence);
void read_public_key(char *filename, mpz_t *pub_sequence);
void one_block_encryption(mpz_t *pub_sequence, char *message, mpz_t cipher);
void subset_sum_solver(mpz_t c, int *indexes, mpz_t *sequence);
void one_block_decryption(mpz_t cipher, mpz_t *sequence, const mpz_t q, const mpz_t r, unsigned char *res);
void ecb_encryption(mpz_t *pub_sequence, char *full_message);
void ecb_decryption(mpz_t *sequence, char *filename, mpz_t q, mpz_t r);

#endif /* CRYPTOSYSTEM_H */