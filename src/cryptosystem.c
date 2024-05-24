#include "../include/cryptosystem.h"
#include "../include/main.h"
#include "../include/attack.h"

static gmp_randstate_t state;
static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence 
 * sequence must be initialized before */
void super_increasing_sequence(mpz_t *sequence, mpz_t q)
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
    mpz_add(q, temp, sum);

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

void initialisation(mpz_t *sequence, mpz_t q, mpz_t r, mpz_t *pub_sequence)
{
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(sequence[i], DEFAULT_SIZE);
    
    mpz_init2(q, DEFAULT_SIZE);
    super_increasing_sequence(sequence, q);

    mpz_init2(r, DEFAULT_SIZE);
    find_coprime(q, r);

    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(pub_sequence[i], DEFAULT_SIZE);

    public_sequence(sequence, pub_sequence, q, r); 
}

void read_private_key(char *filename, mpz_t *sequence, mpz_t q, mpz_t r)
{
    FILE *key_file = NULL;
    key_file = fopen(filename, "r");
    if(key_file == NULL)
        errx(EXIT_FAILURE, "error: private_key cannot be opened");
    
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(sequence[i], DEFAULT_SIZE);

    mpz_t z;  
    mpz_init2(z, DEFAULT_SIZE);    

    fscanf(key_file,"[");
    char buf[50];
    int i = 0;

    while ((gmp_fscanf(key_file, "%Zd%[^ \n]", z, buf) != EOF) && (i < MESSAGE_LENGTH))
    {
        mpz_set(sequence[i], z);
        i++;
    }
    
    mpz_init2(q, DEFAULT_SIZE);
    mpz_init2(r, DEFAULT_SIZE);
    gmp_fscanf(key_file, "q = %Zd\nr = %Zd", q, r);

    mpz_clear(z);
    fclose(key_file);
}

/* IMPORTANT : key_file MUST be formatted [key1, key2, ..., keyN] */
void read_public_key(char *filename, mpz_t *pub_sequence)
{
    FILE *key_file = NULL;
    key_file = fopen(filename, "r");
    if(key_file == NULL)
        errx(EXIT_FAILURE, "error: public_key cannot be opened");
    
    for (int i = 0; i < MESSAGE_LENGTH; i++)
        mpz_init2(pub_sequence[i], DEFAULT_SIZE);

    mpz_t z;  
    mpz_init2(z, DEFAULT_SIZE);    

    fscanf(key_file,"[");
    char buf[50];
    int i = 0;

    while ((gmp_fscanf(key_file, "%Zd%[^ ]", z, buf) != EOF) && (i < MESSAGE_LENGTH))
    {
        mpz_set(pub_sequence[i], z);
        i++;
    }

    mpz_clear(z);
    fclose(key_file);
}
    
void store_private_key(char *filename, mpz_t *sequence, mpz_t q, mpz_t r)
{
    FILE *prv_output;
    prv_output = fopen(filename,"a");
    if(!prv_output)
        errx(EXIT_FAILURE, "error: prv_output open");
    
    fprintf(prv_output,"[");
    for (int i = 0; i < MESSAGE_LENGTH - 1; i++)
        gmp_fprintf(prv_output,"%Zd, ", sequence[i]);
    gmp_fprintf(prv_output,"%Zd]\n", sequence[MESSAGE_LENGTH - 1]);

    gmp_fprintf(prv_output,"q = %Zd\nr = %Zd\n\n", q, r);
    fclose(prv_output);
}

void store_public_key(char *filename, mpz_t *pub_sequence)
{
    FILE *pub_output;
    pub_output = fopen(filename,"a");
    if(!pub_output)
        errx(EXIT_FAILURE, "error: pub_output open");

    fprintf(pub_output,"[");
    for (int i = 0; i < MESSAGE_LENGTH - 1; i++)
        gmp_fprintf(pub_output,"%Zd, ", pub_sequence[i]);
    gmp_fprintf(pub_output,"%Zd]\n\n", pub_sequence[MESSAGE_LENGTH - 1]);

    fclose(pub_output);

}

/* cipers the message, cipher must be initialized */
void one_block_encryption(mpz_t *pub_sequence, char *message, mpz_t cipher)
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

void convert_indexes(int indexes[], unsigned char* res){
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
}

void one_block_decryption(mpz_t cipher, mpz_t *sequence, const mpz_t q, const mpz_t r, unsigned char *res)
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

    convert_indexes(indexes, res);
}

void ecb_encryption(mpz_t *pub_sequence, char *full_message)
{
    FILE *output_file = NULL;
    output_file = fopen("cipher.txt", "w");
    if(output_file == NULL)
        errx(EXIT_FAILURE, "error: output_file cannot be opened");
    
    int block_length = MESSAGE_LENGTH/8;
    int i = 0;
    int plainmsg_length = strlen(full_message);
    char message_block[block_length + 1];

    while(i < plainmsg_length)
    {   
        for(int j = 0; j < block_length; j++)
            message_block[j] = full_message[i + j]; 

        i += block_length;
        mpz_t cipher;
        mpz_init2(cipher, DEFAULT_SIZE);
        one_block_encryption(pub_sequence, message_block, cipher);
        
        gmp_fprintf(output_file,"%Zd ", cipher);
        mpz_clear(cipher);
    }
    fclose(output_file);
}

void ecb_decryption(mpz_t *sequence, char *filename, mpz_t q, mpz_t r, crypto_mode mode, mpz_t *pub_sequence)
{
    FILE *cipher_file = NULL;
    cipher_file = fopen(filename, "r");
    if(cipher_file == NULL)
        errx(EXIT_FAILURE, "error: cipher_file cannot be opened");
    
    FILE *output_file = NULL;
    output_file = fopen("decipher.txt", "w");
    if(output_file == NULL)
        errx(EXIT_FAILURE, "error: output_file cannot be opened");

    mpz_t z;  
    mpz_init2(z, DEFAULT_SIZE);  

    while (gmp_fscanf(cipher_file, "%Zd ", z) != EOF)
    {
        unsigned char res[MESSAGE_LENGTH/8 + 1];

        if(mode==ATTACK)
            one_block_attack(z,pub_sequence,res);
        else
            one_block_decryption(z, sequence, q, r, res);


        fprintf(output_file,"%s", res);
    }

    mpz_clear(z);
    fclose(cipher_file);
    fclose(output_file);
}

void one_block_attack(mpz_t cipher, mpz_t* pub_sequence, unsigned char* res){
    mpz_t public_key[MESSAGE_LENGTH];
    for(int i=0; i!=MESSAGE_LENGTH; i++){
        mpz_init(public_key[i]);
        mpz_set(public_key[i], pub_sequence[i]);
    }

    mpz_t temp; mpz_init(temp);
    attack(public_key, MESSAGE_LENGTH, cipher, temp);
    int indexes[MESSAGE_LENGTH];
    for(int i=0; i!=MESSAGE_LENGTH; i++){

        if(mpz_tstbit(temp,i))
            indexes[i]=1;
        else
            indexes[i]=0;
    }

    convert_indexes(indexes, res);

    mpz_clear(temp);
    for(int i=0; i!=MESSAGE_LENGTH; i++){
        mpz_clear(public_key[i]);
    }
}
