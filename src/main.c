#include "main.h"
#include "gram-schmidt.h"
#include "cryptosystem.h"

int main(int argc, char *argv[])
{

    // private sequence

    mpz_t sequence[MESSAGE_LENGTH], q, r, pub_sequence[MESSAGE_LENGTH];
    initialisation(sequence, q, r, pub_sequence);

   
    /* public key = pub_sequence
    * private (key = sequence, r, q) */
    
    char *message = "message plus long"; // blocks
    
    ecb_encryption(pub_sequence, message);
    ecb_decryption(sequence, "cipher.txt", q, r);

    mpz_clears(q, r, NULL);
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_clears(sequence[i], pub_sequence[i], NULL);   
    
    return 0;

}
