#ifndef ATTACK_H
#define ATTACK_H

#include "gram-schmidt.h"

void lll(struct Vector* v[], int number_of_vectors);
void attack(mpz_t public_key[], int key_size, mpz_t cipher, mpz_t message);
void test_attack();

#endif
