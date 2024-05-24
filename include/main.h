#ifndef HME_H
#define HME_H

#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <gmp.h>

typedef enum {
    DECRYPTION,
    ENCRYPTION,
    GENERATION,
    UNINITIALIZED
}  crypto_mode;

#define DEFAULT_MESSAGE "message plus long"
#define CIPHERTXT "cipher.txt"
#define DECIPHERTXT "decipher.txt"
#define PRIVATEKEY "private_key"
#define PUBLICKEY "public_key"

#endif /* HME_H*/
