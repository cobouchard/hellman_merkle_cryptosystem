#include "main.h"
#include "gram-schmidt.h"
#include "cryptosystem.h"


static crypto_mode mode = UNINITIALIZED;

int main(int argc, char *argv[])
{
    // private sequence

    mpz_t sequence[MESSAGE_LENGTH], q, r, pub_sequence[MESSAGE_LENGTH];
    /*initialisation(sequence, q, r, pub_sequence);

    int opt;
    while ((opt = getopt(argc, argv, "c:d:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed.");
            if (!optarg)
                errx(EXIT_FAILURE, "error: encryption mode needs an argument.");
            mode = ENCRYPTION;
            break;

        case 'd':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed.");
            if (!optarg)
                errx(EXIT_FAILURE, "error: decryption mode needs an argument.");
            mode = DECRYPTION;
            break;

        default:
            errx(EXIT_FAILURE, "error: option %c not allowed.",opt);
            break;
        }
    }

    char *message = (mode == ENCRYPTION)? argv[optind - 1]: "message plus long";
    char *input_file = (mode == DECRYPTION)? argv[optind -1]: "cipher.txt";

    switch (mode)
    {
    case UNINITIALIZED:
        printf("DEMO:\nm = \"message plus long\"\n");
        ecb_encryption(pub_sequence, message);
        ecb_decryption(sequence, input_file, q, r);
        printf("results written in files \'cipher.txt\' and \'decipher.txt\'\n");
        break;

    case DECRYPTION:
        ecb_decryption(sequence, "cipher.txt", q, r);
        break;

    case ENCRYPTION:
        ecb_encryption(pub_sequence, message);
        break;

    default:
        errx(EXIT_FAILURE, "error: mode has wrong value");
        break;
    }

*/
    read_private_key("private_key", sequence, q, r);
    read_public_key("public_key", pub_sequence);

    mpz_clears(q, r, NULL);
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_clears(sequence[i], pub_sequence[i], NULL);

    return 0;


}


/* ciphers the message, cipher must be initialized to 1 */
void encryption(mpz_t *pub_sequence, char *message, mpz_t cipher)
{
    int len = MIN(strlen(message), MESSAGE_LENGTH);
    for(int i = 0; i < len; i++)
    {
        if ((message[i / 8] >> (i % 8)) & 1)
            mpz_mul(cipher, cipher, pub_sequence[i]);
    }
}


