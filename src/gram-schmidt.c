#include <gmp.h>
#include <err.h>
#include <stdlib.h>
#include "../include/gram-schmidt.h"
#include "../include/main.h"

mpz_t* dot_product(mpz_t* a[],mpz_t* b[]){
    int size = sizeof(*a);
    if(size != sizeof(*b)){
        errx(EXIT_FAILURE, "a and b of two different sizes, cannot compute dot product\n");
    }

    mpz_t temp;
    mpz_t* product = (mpz_t*)malloc(sizeof(mpz_t));
    mpz_init2(temp, DEFAULT_SIZE);
    mpz_init2(*product, DEFAULT_SIZE);

    for(int i=0; i!=size; i++){
        mpz_mul(temp, *a[i], *b[i]);
        mpz_add(*product, *product, temp);
    }

    mpz_clear(temp);

    return product;
}

void vector_projection(mpz_t* u[], mpz_t* v[], mpz_t* proj[]){
    mpz_t scalar;
    mpz_init2(scalar, DEFAULT_SIZE);

    mpz_t* temp = dot_product(v,u);
    mpz_set(scalar, *temp);
    free(temp);

    temp = dot_product(u,u);
    mpz_cdiv_q(scalar, scalar, *temp);
    mpz_clear(*temp);
    free(temp);

    for(int i=0; i!=sizeof(*proj); i++){
        mpz_mul(*proj[i], *u[i], scalar);
    }

    mpz_clear(scalar);
}

void gram_schmidt(struct Vector* v[], struct Vector* u[]){
    for(int i=0; i!=sizeof(*v[0]); i++){
        mpz_set(u[0]->coefficients[i], v[0]->coefficients[i]);
    }

    for(int vector_index=1; vector_index!=BASE_SIZE; vector_index++){
        for(int coeff=0; coeff!=BASE_SIZE; coeff++){
            mpz_set(u[vector_index]->coefficients[coeff], v[vector_index]->coefficients[coeff]);
        }

        //subtract projections

        for(int sum_index=1; sum_index!=vector_index+1; sum_index++){
            struct Vector proj_ui;
            init_vector(&proj_ui);
            //vector_projection(u[sum_index]->coefficients, v[vector_index]->coefficients, proj_ui);
        }
    }
}


void init_vector(struct Vector* vector){
    for(int i=0; i!=BASE_SIZE; i++){
        mpz_init2(vector->coefficients[i], DEFAULT_SIZE);
    }
}