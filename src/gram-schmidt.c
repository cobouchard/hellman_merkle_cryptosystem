#include <gmp.h>
#include <stdlib.h>
#include <err.h>
#include "../include/gram-schmidt.h"
#include "../include/main.h"

void dot_product(struct Vector* a,struct Vector* b, mpz_t* result){
    mpz_t temp;
    mpz_init2(temp, DEFAULT_SIZE);
    mpz_init2(*result, DEFAULT_SIZE);

    for(int i=0; i!=BASE_SIZE; i++){
        mpz_mul(temp, a->coefficients[i], b->coefficients[i]);
        mpz_add(*result, *result, temp);
    }

    mpz_clear(temp);
}

void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj){
    mpz_t scalar;
    mpz_init2(scalar, DEFAULT_SIZE);

    mpz_t* temp = (mpz_t*)malloc(sizeof (mpz_t*));
    if(temp==NULL){
        errx(EXIT_FAILURE, "Failed allocation of temp in vector_projection()\n");
    }

    dot_product(v,u, temp);
    mpz_set(scalar, *temp);

    dot_product(u,u,temp);
    mpz_cdiv_q(scalar, scalar, *temp);


    for(int i=0; i!=BASE_SIZE; i++){
        mpz_mul(proj->coefficients[i], u->coefficients[i], scalar);
    }

    mpz_clear(*temp);
    mpz_clear(scalar);
    free(temp);
}

void gram_schmidt(struct Vector* v[], struct Vector* u[]){
    for(int i=0; i!=BASE_SIZE; i++){
        mpz_set(u[0]->coefficients[i], v[0]->coefficients[i]);
    }

    size_t number_of_vectors = sizeof(*v)/sizeof(v[0]);
    for(int vector_index=1; vector_index!=number_of_vectors; vector_index++){
        for(int coeff=0; coeff!=BASE_SIZE; coeff++){
            mpz_set(u[vector_index]->coefficients[coeff], v[vector_index]->coefficients[coeff]);
        }

        //subtract projections

        for(int sum_index=1; sum_index!=vector_index+1; sum_index++){
            struct Vector proj_ui;
            init_vector(&proj_ui);
            vector_projection(u[sum_index], v[vector_index], &proj_ui);
            for(int i=0; i!= BASE_SIZE; i++){
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

void print_vector(struct Vector* vector){
    for(int i=0; i!=BASE_SIZE; i++){
        //gmp_printf("\nq = %Zd\n", q);
        if(i!=BASE_SIZE-1)
            gmp_printf("%Zd,", vector->coefficients[i]);
        else
            gmp_printf("%Zd\n", vector->coefficients[i]);
    }
}


void test_gram_schmidt(){
    struct Vector b1, b2, b3;
    struct Vector c1, c2, c3;
    init_vector(&b1);init_vector(&b2);init_vector(&b3);
    init_vector(&c1);init_vector(&c2);init_vector(&c3);
    struct Vector* v[3] = {&b1,&b2,&b3};
    struct Vector* u[3] = {&c1,&c2,&c3};

    mpz_set_ui(b1.coefficients[0], 1);
    mpz_set_ui(b1.coefficients[1], 1);
    mpz_set_ui(b1.coefficients[2], 1);

    mpz_set_ui(b2.coefficients[0], -1);
    mpz_set_ui(b2.coefficients[1], 0);
    mpz_set_ui(b2.coefficients[2], 3);

    mpz_set_ui(b3.coefficients[0], 3);
    mpz_set_ui(b3.coefficients[1], 5);
    mpz_set_ui(b3.coefficients[2], 6);

    mpz_t temp;
    mpz_init2(temp, DEFAULT_SIZE);

    gram_schmidt(v, u);
    for(int i=0; i!=BASE_SIZE; i++)
        print_vector(u[i]);

}