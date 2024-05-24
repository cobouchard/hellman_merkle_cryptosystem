#ifndef HME_H
#define HME_H

#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <gmp.h>

#define MESSAGE_LENGTH 100
#define DEFAULT_SIZE 1024

typedef enum {
    DECRYPTION,
    ENCRYPTION,
    UNINITIALIZED
}  crypto_mode;


#endif /* HME_H*/
