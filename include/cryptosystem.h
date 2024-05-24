#ifndef CRYPTOSYSTEM_H
#define CRYPTOSYSTEM_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "../include/main.h"

#define MESSAGE_LENGTH 96 // or 104
#define DEFAULT_SIZE 1024UL
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void super_increasing_sequence(mpz_t *sequence, mpz_t q);
void find_coprime(mpz_t q, mpz_t res);
void public_sequence(mpz_t *sequence_a, mpz_t *sequence_b, const mpz_t q, const mpz_t r);
void initialisation(mpz_t *sequence, mpz_t q, mpz_t r, mpz_t *pub_sequence);

void read_private_key(char *filename, mpz_t *sequence, mpz_t q, mpz_t r);
void read_public_key(char *filename, mpz_t *pub_sequence);
void store_private_key(char *filename, mpz_t *sequence, mpz_t q, mpz_t r);
void store_public_key(char *filename, mpz_t *pub_sequence);

void one_block_encryption(mpz_t *pub_sequence, char *message, mpz_t cipher);
void subset_sum_solver(mpz_t c, int *indexes, mpz_t *sequence);
void convert_indexes(int* indexes, unsigned char* res);
void one_block_decryption(mpz_t cipher, mpz_t *sequence, const mpz_t q, const mpz_t r, unsigned char *res);
void one_block_attack(mpz_t cipher, mpz_t* pub_sequence, unsigned char* res);

void ecb_encryption(mpz_t *pub_sequence, char *full_message);
void ecb_decryption(mpz_t *sequence, char *filename, mpz_t q, mpz_t r, crypto_mode mode, mpz_t *pub_sequence);

#endif /* CRYPTOSYSTEM_H */
