#include "../include/gram-schmidt.h
#include <gmp.h>
#include <err.h>
#include <stdlib.h>

mpz_t* dot_product(mpz_t* a[],mpz_t* b[]){
    int size = sizeof(*a);
    if(size != sizeof(*b)){
        errx(EXIT_FAILURE, "a and b of two different sizes, cannot compute dot product\n");
    }

    mpz_t temp;
    mpz_t* product = (mpz_t*)malloc(sizeof(mpz_t));
    mpz_init2(temp, 1024UL);
    mpz_init2(*product, 1024UL);

    for(int i=0; i!=size; i++){
        mpz_mul(temp, *a[i], *b[i]);
        mpz_add(*product, *product, temp);
    }

    mpz_clear(temp);

    return product;
}

void vector_projection(mpz_t* u[], mpz_t* v[], mpz_t* proj[]){
    mpz_t scalar;
    mpz_init2(scalar, 1024UL);

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

void gram_schmidt(mpz_t* v[], mpz_t* u[]){

}
