#include "main.h"
#include "gram-schmidt.h"
#include "cryptosystem.h"


static crypto_mode mode = UNINITIALIZED;

int main(int argc, char *argv[])
{
    mpz_t sequence[MESSAGE_LENGTH], q, r, pub_sequence[MESSAGE_LENGTH];
    //read_private_key("private_key", sequence, q, r);
    //read_public_key("public_key", pub_sequence);

    int opt;
    while ((opt = getopt(argc, argv, "c:d:ga")) != -1)
    {
        switch (opt)
        {
        case 'c':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed\n");
            if (!optarg)
                errx(EXIT_FAILURE, "error: encryption mode needs an argument\n");
            mode = ENCRYPTION;
            break;

        case 'd':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed\n");
            if (!optarg)
                errx(EXIT_FAILURE, "error: decryption mode needs an argument\n");
            mode = DECRYPTION;
            break;

        case 'g':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed\n");
            mode = GENERATION;
            break;

        case 'a':
            if (mode != UNINITIALIZED)
                errx(EXIT_FAILURE, "error: only one cryptography mode allowed\n");
            mode = ATTACK;
            break;

        default:
            errx(EXIT_FAILURE, "error: option %c not allowed\n",opt);
            break;
        }
    }

    char *message = (mode == ENCRYPTION)? argv[optind - 1]: DEFAULT_MESSAGE;
    char *input_file = (mode == DECRYPTION)? argv[optind -1]: CIPHERTXT;

    optind++;
        read_private_key(PRIVATEKEY, sequence, q, r);
        read_public_key(PUBLICKEY, pub_sequence);

    switch (mode)
    {
    case UNINITIALIZED:
        printf("DEMO:\nm = \"message plus long\"\n");
        ecb_encryption(pub_sequence, message);
        ecb_decryption(sequence, input_file, q, r, mode, pub_sequence);
        printf("results written in files \'%s\' and \'%s\'\n", CIPHERTXT, DECIPHERTXT);
        break;

    case ATTACK:
    case DECRYPTION:
        if(optind <= argc)
            read_private_key(argv[optind - 1], sequence, q, r);
        else
        {
            printf("%s not given, generating one...\n", PRIVATEKEY);
            initialisation(sequence, q, r, pub_sequence);
        }

        ecb_decryption(sequence, CIPHERTXT, q, r, mode, pub_sequence);

        break;

    case ENCRYPTION:
        if(optind <= argc)
            read_public_key(argv[optind - 1], pub_sequence);
        else
        {
            printf("%s not given, generating one...\n", PUBLICKEY);
            initialisation(sequence, q, r, pub_sequence);
        }
        ecb_encryption(pub_sequence, message);
        break;
        
    case GENERATION:
        initialisation(sequence, q, r, pub_sequence);
        store_private_key(PRIVATEKEY, sequence, q, r);
        store_public_key(PUBLICKEY, pub_sequence);
        break;

    default:
        errx(EXIT_FAILURE, "error: mode has wrong value");
        break;
    }   


    mpz_clears(q, r, NULL);
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_clears(sequence[i], pub_sequence[i], NULL);

    return 0;


}



