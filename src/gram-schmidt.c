#include <gmp.h>

#include "../include/gram-schmidt.h"
#include "../include/attack.h"
#include "../include/cryptosystem.h"


void nearest_integer(mpf_t floaty, mpz_t result){
    mpf_t floor, sub, temp;
    mpf_init(floor); mpf_init(sub); mpf_init(temp);

    mpf_floor(floor, floaty);
    mpf_sub(sub, floaty, floor);

    if(mpf_cmp_d(sub, 0.5)>0){ //sub>0.5
        mpf_ceil(temp, floaty);
        mpz_set_f(result, temp);
    }
    else{
        mpz_set_f(result, floor);
    }

    mpf_clear(sub);
    mpf_clear(floor);
    mpf_clear(temp);
}

void norm2(struct Vector* vector, mpf_t result, int size){
    mpf_t temp;
    mpf_init(temp);

    for(int i=0; i!=size; i++){
        mpf_pow_ui(temp, vector->coefficients[i], 2);
        mpf_add(result,result,temp);
    }

    mpf_sqrt(result, result);

    mpf_clear(temp);
}

void dot_product(struct Vector* a,struct Vector* b, mpf_t result, int size){
    mpf_t temp;
    mpf_init(temp);

    for(int i=0; i!=size; i++){
        mpf_mul(temp, a->coefficients[i], b->coefficients[i]);
        mpf_add(result, result, temp);
    }

    mpf_clear(temp);
}

void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj, int size){
    mpf_t scalar, temp;
    mpf_init(scalar);
    mpf_init(temp);

    dot_product(v, u, scalar,size);
    dot_product(u, u, temp,size);
    mpf_div(scalar, scalar, temp);

    for(int i=0; i!=size; i++){
        mpf_mul(proj->coefficients[i], u->coefficients[i], scalar);
    }

    mpf_clear(temp);
    mpf_clear(scalar);
}

void get_u_ij(struct Vector* b[], struct Vector* b_prime[], int i, int j, mpf_t result, int size){
    mpf_t top, bottom;
    mpf_init(top); mpf_init(bottom);

    dot_product(b[i],b_prime[j],top, size);
    dot_product(b_prime[j], b_prime[j], bottom,size);

    mpf_div(result,top,bottom);

    mpf_clear(top); mpf_clear(bottom);
}

void gram_schmidt(struct Vector* v[], struct Vector* u[], int number_of_vectors){
    for(int i=0; i!=number_of_vectors; i++){
        mpf_set(u[0]->coefficients[i], v[0]->coefficients[i]);
    }
    
    for(int vector_index=1; vector_index!=number_of_vectors; vector_index++){
        for(int coeff=0; coeff!=number_of_vectors; coeff++){
            mpf_set(u[vector_index]->coefficients[coeff], v[vector_index]->coefficients[coeff]);
        }
        
        //subtract projections
        for(int sum_index=0; sum_index!=vector_index; sum_index++){
            struct Vector proj_ui;
            init_vector(&proj_ui, number_of_vectors);
            vector_projection(u[sum_index], v[vector_index], &proj_ui, number_of_vectors);
            for(int i=0; i!= number_of_vectors; i++){
                mpf_sub(u[vector_index]->coefficients[i], u[vector_index]->coefficients[i], proj_ui.coefficients[i]);
            }
            clear_vector(&proj_ui, number_of_vectors);
        }
    }
}


void init_vector(struct Vector* vector, int size){
    for(int i=0; i!=size; i++){
        mpf_init(vector->coefficients[i]);
    }
}

void clear_vector(struct Vector* v, int size){
    for (int i = 0; i!=size; i++) {
        mpf_clear(v->coefficients[i]);
    }
}

void print_vector(struct Vector* vector, int size){
    for(int i=0; i!=size; i++){
        if(i!=size-1)
            gmp_printf("%. *Ff,", 10, vector->coefficients[i]);
        else
            gmp_printf("%.* Ff\n", 10, vector->coefficients[i]);
    }
}


void test_gram_schmidt(){
    mpf_set_default_prec(DEFAULT_SIZE);
    struct Vector b1, b2, b3;
    struct Vector c1, c2, c3;
    int size = 3;
    init_vector(&b1, size);init_vector(&b2, size);init_vector(&b3, size);
    init_vector(&c1, size);init_vector(&c2, size);init_vector(&c3, size);
    struct Vector* v[3] = {&b1,&b2,&b3};
    struct Vector* u[3] = {&c1,&c2,&c3};

    mpf_set_si(b1.coefficients[0], 1);
    mpf_set_si(b1.coefficients[1], 1);
    mpf_set_si(b1.coefficients[2], 1);

    mpf_set_si(b2.coefficients[0], -1);
    mpf_set_si(b2.coefficients[1], 0);
    mpf_set_si(b2.coefficients[2], 2);

    mpf_set_si(b3.coefficients[0], 3);
    mpf_set_si(b3.coefficients[1], 5);
    mpf_set_si(b3.coefficients[2], 6);

    mpf_t temp;
    mpf_init(temp);

    lll(v,3);
    for(int i=0; i!=3; i++){
        print_vector(v[i], size);
    }


    clear_vector(&b1, size); clear_vector(&b2, size); clear_vector(&b3, size);
    clear_vector(&c1, size); clear_vector(&c2, size); clear_vector(&c3, size);
}