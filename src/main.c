#include "main.h"
#include "gram-schmidt.h"
#include "cryptosystem.h"

int main(int argc, char *argv[])
{

    // private sequence

    mpz_t sequence[MESSAGE_LENGTH];
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(sequence[i], DEFAULT_SIZE);

    mpz_t q;
    mpz_init2(q, DEFAULT_SIZE);
    super_increasing_sequence(sequence, &q);
    
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        gmp_printf ("%Zd/ ", sequence[i]);
    gmp_printf("\nq = %Zd\n", q);
    
    // r and public sequence computation 
    mpz_t r;
    mpz_init2(r, DEFAULT_SIZE);
    find_coprime(q, r);
    gmp_printf("r = %Zd\n", r);
    
    mpz_t pub_sequence[MESSAGE_LENGTH];
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(pub_sequence[i], DEFAULT_SIZE);

    public_sequence(sequence, pub_sequence, q, r); 

    printf("-------------------------------------\npublic key:\n");
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        gmp_printf ("%Zd/ ", pub_sequence[i]);
    printf("\n-------------------------------------\n");

/* public key = pub_sequence
 * private (key = sequence, r, q) */
    
    char *message = "message";
    
    mpz_t cipher;
    mpz_init2(cipher, DEFAULT_SIZE);
    encryption(pub_sequence, message, cipher);

    unsigned char res[MESSAGE_LENGTH/8 + 1];
    decryption(cipher, sequence, q, r, res);
    printf("%s\n", res);


    mpz_clears(q, r, cipher, NULL);
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_clears(sequence[i], pub_sequence[i], NULL);   
    
    return 0;


}
