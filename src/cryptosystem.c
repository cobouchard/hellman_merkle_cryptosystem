#include "cryptosystem.h"



#define MIN(a, b) ((a) < (b) ? (a) : (b))

static gmp_randstate_t state;
static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence 
 * sequence must be initialized before */
void super_increasing_sequence(mpz_t *sequence, mpz_t *q)
{
    if(!randomized)
    {   
        gmp_randinit_default(state);
        randomized = true;
        gmp_randseed_ui(state, time(NULL));
    }

    if (MESSAGE_LENGTH <= 0)
        return ;
    
    mpz_set_ui(sequence[0], rand()% 10 + 1);

    mpz_t sum;
    mpz_t temp;
    mpz_init_set(sum, sequence[0]);
    mpz_init2(temp, DEFAULT_SIZE);

    for (int i = 1; i < MESSAGE_LENGTH; i++)
    {   
        mpz_urandomm(temp, state, sequence[i - 1]);
        mpz_add_ui(temp, temp, 1);          // urandomm generates from 0
        mpz_add(sequence[i], sum, temp);
        mpz_add(sum, sum, sequence[i]);
    }

    mpz_urandomm(temp, state, sum);
    mpz_add((*q), temp, sum);

    mpz_clears(temp, sum, NULL);
    return;
}

/* finds a number < q coprime with q*/
void find_coprime(mpz_t q, mpz_t res)
{   
    if(!randomized)
    {   
        gmp_randinit_default(state);
        randomized = true;
        gmp_randseed_ui(state, time(NULL));
    }
    
    mpz_t gcd;
    mpz_init(gcd);
    mpz_urandomm(res, state, q);
    mpz_gcd(gcd, q, res);

    while (mpz_cmp_ui(gcd, 1) != 0)
    {
        mpz_urandomm(res, state, q);
        mpz_gcd(gcd, q, res);
    }

    mpz_clear(gcd);
    return;
}

/* calculate the sequence B so that bi = r*ai % q 
 * sequence_b must be initialized before */
void public_sequence(mpz_t *sequence_a, mpz_t *sequence_b, const mpz_t q, const mpz_t r)
{
    for (int i = 0; i < MESSAGE_LENGTH; i++)
    {
        mpz_mul(sequence_b[i], sequence_a[i], r);
        mpz_mod(sequence_b[i], sequence_b[i], q);
    }
    return;
}

void str_to_binary(char *message, char* binary)
{
    int len = strlen(message);
    for(int i = 0; i < MESSAGE_LENGTH; i++)
    {   
        if (i/8 < len)
            binary[i] = ((message[i / 8] >> (i % 8)) & 1) ? '1' : '0';
        else
            binary[i] = '0';
    }
}

/* cipers the message, cipher must be initialized */
void encryption(mpz_t *pub_sequence, char *message, mpz_t cipher)
{
    mpz_set_ui(cipher, 0);
    int len = MIN(strlen(message) * 8, MESSAGE_LENGTH);
    for(int i = 0; i < len; i++)
    {
        if ((message[i / 8] >> (i % 8)) & 1)
        {
            mpz_add(cipher, cipher, pub_sequence[i]);
        }
    }
}

void subset_sum_solver(mpz_t c, int *indexes, mpz_t *sequence)
{
    while (mpz_cmp_ui(c, 0) > 0)
    {
        int i = 0;
        while ((mpz_cmp(sequence[i], c) <= 0) && (i < MESSAGE_LENGTH))
            i++;
        indexes[i - 1] = 1;
        mpz_sub(c, c, sequence[i - 1]);
    }
    
}

void decryption(mpz_t cipher, mpz_t *sequence, const mpz_t q, const mpz_t r, unsigned char *res)
{
    mpz_t invert;
    mpz_init2(invert, DEFAULT_SIZE);
    mpz_invert(invert, r, q);       // computation of r independant to the message
                                    // can be done once and work for all messages
    mpz_t c;
    mpz_init2(c, DEFAULT_SIZE);
    mpz_mul(c, cipher, invert);
    mpz_mod(c, c, q);

    int indexes[MESSAGE_LENGTH];
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        indexes[i] = 0;
    subset_sum_solver(c, indexes, sequence);

    int pow;
    int temp_int;
    for (int i = 0; i < MESSAGE_LENGTH; i++)
    {   
        if (i % 8 == 0)
        {
            pow = 1;
            temp_int = 0;
        }

        temp_int += indexes[i] * pow;
        pow *= 2;

        if ((i + 1) % 8 == 0)
            res[i/8] = temp_int;
        
    }
    return;
}
