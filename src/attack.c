#include "../include/attack.h"
#include "../include/gram-schmidt.h"
#include <gmp.h>
#include <stdlib.h>

void lll(struct Vector* v[], int number_of_vectors){
    struct Vector* u[number_of_vectors]; //containing the orthonormalized basis
    for(int i=0; i!=number_of_vectors; i++){
        u[i] = (struct Vector*)malloc(sizeof(struct Vector));
        init_vector(u[i]);
    }
    gram_schmidt(v,u,number_of_vectors);


    mpf_t temp, temp2;
    mpf_init(temp); mpf_init(temp2);


    for(int k=1; k<number_of_vectors; ){

        for(int j=k-1; j>=0; j--){
            get_u_ij(v,u,k,j,temp);
            mpf_abs(temp, temp);

            mpz_t converted_int;
            mpz_init (converted_int);
            nearest_integer(temp,converted_int);
            mpf_set_z(temp2, converted_int);
            for(int i=0; i!=BASE_SIZE; i++){
                mpf_mul(temp, v[j]->coefficients[i], temp2);
                mpf_sub(v[k]->coefficients[i], v[k]->coefficients[i], temp);
            }
            gram_schmidt(v,u,number_of_vectors);

        }
        mpf_t right_side, left_side;
        mpf_init(right_side); mpf_init(left_side);

        dot_product(u[k],u[k],left_side);
        dot_product(u[k-1],u[k-1],right_side);

        get_u_ij(v,u,k,k-1,temp);
        mpf_pow_ui(temp, temp, 2);
        mpf_ui_sub(temp, DELTA, temp);

        mpf_mul(right_side, temp, right_side);

        if(mpf_cmp(left_side, right_side)>0){ //left_side > right_side
            k=k+1;
        }
        else{
            for(int i=0; i!=BASE_SIZE; i++){
                mpf_swap(v[k]->coefficients[i], v[k-1]->coefficients[i]);
            }
            gram_schmidt(v,u,number_of_vectors);
            k = ((k-1 > 2) ? k - 1 : 2)-1;
        }

        mpf_clear(left_side);
        mpf_clear(right_side);

    }

    for(int i = 0; i < number_of_vectors; i++) {
        clear_vector(u[i]);
        free(u[i]);
    }

    mpf_clear(temp); mpf_clear(temp2);
}