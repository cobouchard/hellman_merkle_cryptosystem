#include <gmp.h>
#include <stdlib.h>
#include <err.h>

#include "../include/attack.h"

void lll(struct Vector* v[], int number_of_vectors){
    struct Vector* u[number_of_vectors]; //containing the orthonormalized basis
    for(int i=0; i!=number_of_vectors; i++){
        u[i] = (struct Vector*)malloc(sizeof(struct Vector));
        init_vector(u[i], number_of_vectors);
    }
    gram_schmidt(v,u,number_of_vectors);


    mpf_t temp, temp2;
    mpf_init(temp); mpf_init(temp2);

    for(int k=1; k<number_of_vectors; ){
        for(int j=k-1; j>=0; j--){
            get_u_ij(v,u,k,j,temp,number_of_vectors);
            mpf_abs(temp2, temp);

            if(mpf_cmp_d(temp2, 0.5)>0){

                mpz_t converted_int;
                mpz_init (converted_int);
                nearest_integer(temp,converted_int);
                mpf_set_z(temp2, converted_int);

                for(int i=0; i!=number_of_vectors; i++){
                    mpf_mul(temp, v[j]->coefficients[i], temp2);
                    mpf_sub(v[k]->coefficients[i], v[k]->coefficients[i], temp);
                }
                gram_schmidt(v,u,number_of_vectors);
            }
        }

        mpf_t right_side, left_side;
        mpf_init(right_side); mpf_init(left_side);

        dot_product(u[k],u[k],left_side,number_of_vectors);
        dot_product(u[k-1],u[k-1],right_side,number_of_vectors);

        get_u_ij(v,u,k,k-1,temp,number_of_vectors);
        mpf_pow_ui(temp, temp, 2);
        mpf_set_d(temp2, DELTA);
        mpf_sub(temp, temp2, temp);

        mpf_mul(right_side, temp, right_side);


        if(mpf_cmp(left_side, right_side)>0){ //left_side > right_side
            k=k+1;
        }
        else{
            for(int i=0; i!=number_of_vectors; i++){
                mpf_swap(v[k]->coefficients[i], v[k-1]->coefficients[i]);
            }
            gram_schmidt(v,u,number_of_vectors);
            k = (( (k-1) > 2 ) ? k - 1 : 2) -1;
        }

        mpf_clear(left_side);
        mpf_clear(right_side);
    }

    for(int i = 0; i < number_of_vectors; i++) {
        clear_vector(u[i],number_of_vectors);
        free(u[i]);
    }

    mpf_clear(temp); mpf_clear(temp2);
}

void attack(mpz_t public_key[], int key_size, mpz_t cipher, mpz_t message){
    struct Vector* columns[key_size+1];

    for(int i=0; i!=key_size+1; i++){
        columns[i] = (struct Vector *)malloc(sizeof(struct Vector));
        if(columns[i]==NULL)
            errx(EXIT_FAILURE, "Failed allocation of matrix[%d] in attack()\n", i);
        init_vector(columns[i], key_size+1);

        for(int coeff=0; coeff!=key_size+1; coeff++){
            if(coeff==key_size){
                if(i==key_size)
                    mpf_set_z(columns[i]->coefficients[coeff], cipher);
                else
                    mpf_set_z(columns[i]->coefficients[coeff], public_key[i]);
            }
            else if(coeff==i)
                mpf_set_d(columns[i]->coefficients[coeff], 1);
            else{
                mpf_set_d(columns[i]->coefficients[coeff], 0);
            }

        }
    }

    lll(columns, key_size+1);

    //we now have to find the correct column giving the message
    int correct_column=-1;
    for(int column=0; column!=key_size+1; column++){
        int correct=1;
        for(int i=0; i!=key_size+1; i++){
            int temp = mpf_get_d(columns[column]->coefficients[i]);
            if(temp!=0 && temp!=1){
                correct=0;
                break;
            }
        }
        if(correct){
            correct_column=column;
            break;
        }
    }

    if(correct_column==-1){
        errx(EXIT_FAILURE, "Couldn't find a column corresponding to the message in attack()\n");
    }

    //we calculate the message
    for(int i=0; i!=key_size; i++){
        if(mpf_cmp_d(columns[correct_column]->coefficients[i],1)==0){
            mpz_t temp; mpz_init(temp);
            mpz_setbit(temp, key_size-i-1);
            mpz_add(message, message, temp);
        }
    }

    for(int i=0; i!=key_size+1; i++){
        clear_vector(columns[i], key_size+1);
        free(columns[i]);
    }
}

void test_attack(){
    mpz_t public_key[9];
    mpz_t cipher; mpz_init(cipher);
    for(int i=0; i!=9 ; i++)
        mpz_init(public_key[i]);

    mpz_set_d(public_key[0], 575);
    mpz_set_d(public_key[1], 436);
    mpz_set_d(public_key[2], 1586);
    mpz_set_d(public_key[3], 1030);
    mpz_set_d(public_key[4], 1921);
    mpz_set_d(public_key[5], 569);
    mpz_set_d(public_key[6], 721);
    mpz_set_d(public_key[7], 1183);
    mpz_set_d(public_key[8], 1570);

    mpz_set_d(cipher, -6665);

    mpz_t result; mpz_init(result);
    attack(public_key, 9, cipher, result);

    gmp_printf("%Zd \n", result);

    for(int i=0; i!=9 ; i++)
        mpz_clear(public_key[i]);
    mpz_clear(cipher); mpz_clear(result);
}