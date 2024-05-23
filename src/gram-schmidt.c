#include <gmp.h>
#include <err.h>
#include <stdlib.h>
#include "../include/gram-schmidt.h"
#include "../include/main.h"

mpz_t* dot_product(struct Vector* a,struct Vector* b){
    mpz_t temp;
    mpz_t* product = (mpz_t*)malloc(sizeof(mpz_t));
    mpz_init2(temp, DEFAULT_SIZE);
    mpz_init2(*product, DEFAULT_SIZE);

    for(int i=0; i!=a->size; i++){
        mpz_mul(temp, a->coefficients[i], b->coefficients[i]);
        mpz_add(*product, *product, temp);
    }

    mpz_clear(temp);
    return product;
}

void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj){
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
        mpz_mul(proj->coefficients[i], u->coefficients[i], scalar);
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
            vector_projection(u[sum_index], v[vector_index], &proj_ui);
            for(int i=0; i!= proj_ui.size; i++){
                mpz_sub(u[vector_index]->coefficients[i], u[vector_index]->coefficients[i], proj_ui.coefficients[i]);
            }
        }
    }
}


void init_vector(struct Vector* vector){
    for(int i=0; i!=BASE_SIZE; i++){
        mpz_init2(vector->coefficients[i], DEFAULT_SIZE);
    }
}