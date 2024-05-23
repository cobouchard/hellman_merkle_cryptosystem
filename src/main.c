#include "main.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static gmp_randstate_t state;
static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence 
 * sequence must be initialized before */
void super_increasing_sequence(mpz_t *sequence, mpz_t *res)
{
    if (MESSAGE_LENGTH <= 0)
        return ;
    
    mpz_set_ui(sequence[0], rand()% 10 + 1);

    mpz_t sum;
    mpz_t temp;
    mpz_init_set(sum, sequence[0]);
    mpz_init2(temp, 1024UL);

    for (int i = 1; i < MESSAGE_LENGTH; i++)
    {   
        mpz_urandomm(temp, state, sequence[i - 1]);
        mpz_add_ui(temp, temp, 1);          // urandomm generates from 0
        mpz_add(sequence[i], sum, temp);
        mpz_add(sum, sum, sequence[i]);
    }

    mpz_urandomm(temp, state, sum);
    mpz_add((*res), temp, sum);

    mpz_clears(temp, sum, NULL);
    return;
}

/* finds a number < q coprime with q*/
void find_coprime(mpz_t q, mpz_t res)
{   
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

/* cipers the message, cipher must be initialized to 1 */
void encryption(mpz_t *pub_sequence, char *message, mpz_t cipher)
{
    int len = MIN(strlen(message), MESSAGE_LENGTH);
    for(int i = 0; i < len; i++)
    {
        if ((message[i / 8] >> (i % 8)) & 1)
            mpz_mul(cipher, cipher, pub_sequence[i]);
    }
}


int main(int argc, char *argv[])
{
    if(!randomized)
    {   
        gmp_randinit_default(state);
        randomized = true;
        gmp_randseed_ui(state, time(NULL));
    }

    // private sequence

    mpz_t sequence[MESSAGE_LENGTH];
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(sequence[i], 1024UL);

    mpz_t q;
    mpz_init2(q, 1024UL);
    super_increasing_sequence(sequence, &q);
    
    for (int i = 0; i < 100; i++)
        gmp_printf ("%Zd/ ", sequence[i]);
    gmp_printf("\nq = %Zd\n", q);
    
    // r and public sequence computation 
    mpz_t r;
    mpz_init2(r, 1024UL);
    find_coprime(q, r);
    gmp_printf("r = %Zd\n", r);
    
    mpz_t pub_sequence[MESSAGE_LENGTH];
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(pub_sequence[i], 1024UL);

    public_sequence(sequence, pub_sequence, q, r); 

    printf("-------------------------------------\npublic key:\n");
    for (int i = 0; i < 100; i++)
        gmp_printf ("%Zd/ ", pub_sequence[i]);
    printf("\n-------------------------------------\n");

/* public key = pub_sequence
 * private (key = sequence, r, q) */

    
    char *message = "message";
    char binary[MESSAGE_LENGTH];
    str_to_binary(message, binary);
    
    mpz_t cipher;
    mpz_init2(cipher, 1024UL);
    encryption(pub_sequence, message, cipher);

    
    mpz_clears(q, r, NULL);
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_clears(sequence[i], pub_sequence[i], NULL);   
    
    return 0;
}