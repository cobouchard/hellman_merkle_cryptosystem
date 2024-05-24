#include <gmp.h>
#include <stdlib.h>
#include <err.h>
#include "../include/gram-schmidt.h"
#include "../include/main.h"

void nearest_integer(mpf_t* floaty, mpz_t* result){
    mpf_t floor, sub, temp;
    mpf_init(floor); mpf_init(sub); mpf_init(temp);

    mpf_floor(floor, *floaty);
    mpf_sub(sub, *floaty, floor);

    if(mpf_cmp_d(sub, 0.5)>0){ //sub>0.5
        mpf_ceil(temp, *floaty);
        mpz_set_f(*result, temp);
    }
    else{
        mpz_set_f(*result, floor);
    }

    mpf_clear(sub);
    mpf_clear(floor);
    mpf_clear(temp);
}

void norm2(struct Vector* vector, mpf_t* result){
    mpf_t temp;
    mpf_init(temp);

    for(int i=0; i!=BASE_SIZE; i++){
        mpf_pow_ui(temp, vector->coefficients[i], 2);
        mpf_add(*result,*result,temp);
    }

    mpf_sqrt(*result, *result);

    mpf_clear(temp);
}

void dot_product(struct Vector* a,struct Vector* b, mpf_t* result){
    mpf_t temp;
    mpf_init(temp);

    for(int i=0; i!=BASE_SIZE; i++){
        mpf_mul(temp, a->coefficients[i], b->coefficients[i]);
        mpf_add(*result, *result, temp);
    }

    mpf_clear(temp);
}

void vector_projection(struct Vector* u, struct Vector* v, struct Vector* proj){
    mpf_t scalar, temp;
    mpf_init(scalar);
    mpf_init(temp);


    dot_product(v, u, &scalar);
    dot_product(u, u, &temp);
    mpf_div(scalar, scalar, temp);


    for(int i=0; i!=BASE_SIZE; i++){
        mpf_mul(proj->coefficients[i], u->coefficients[i], scalar);
    }

    mpf_clear(temp);
    mpf_clear(scalar);
}

void get_u_ij(struct Vector* b[], struct Vector* b_prime[], int i, int j, mpf_t* result){
    mpf_t top, bottom;
    mpf_init(top); mpf_init(bottom);

    dot_product(b[i],b_prime[j],&top);
    dot_product(b_prime[j], b_prime[j], &bottom);

    mpf_div(*result,top,bottom);

    mpf_clear(top); mpf_clear(bottom);
}

void gram_schmidt(struct Vector* v[], struct Vector* u[], int number_of_vectors){
    for(int i=0; i!=BASE_SIZE; i++){
        mpf_set(u[0]->coefficients[i], v[0]->coefficients[i]);
    }

    for(int vector_index=1; vector_index!=number_of_vectors; vector_index++){
        for(int coeff=0; coeff!=BASE_SIZE; coeff++){
            mpf_set(u[vector_index]->coefficients[coeff], v[vector_index]->coefficients[coeff]);
        }

        //subtract projections

        for(int sum_index=0; sum_index!=vector_index; sum_index++){
            struct Vector proj_ui;
            init_vector(&proj_ui);
            vector_projection(u[sum_index], v[vector_index], &proj_ui);
            for(int i=0; i!= BASE_SIZE; i++){
                mpf_sub(u[vector_index]->coefficients[i], u[vector_index]->coefficients[i], proj_ui.coefficients[i]);
            }
            clear_vector(&proj_ui);
        }
    }
}

void lll(struct Vector* v[], int number_of_vectors){
    struct Vector* u[number_of_vectors]; //containing the orthonormalized basis
    for(int i=0; i!=number_of_vectors; i++){
        u[i] = (struct Vector*)malloc(sizeof(struct Vector));
        init_vector(u[i]);
    }
    gram_schmidt(v,u,number_of_vectors);

    mpf_t temp, temp2;
    mpf_init(temp); mpf_init(temp2);


    for(int k=2; k<=number_of_vectors; k++){
        for(int j=k-1; j!=0; j--){
            get_u_ij(v,u,k,j,&temp);
            mpf_abs(temp, temp);
            if(mpf_cmp_d(temp, 0.5)>0){ //temp>0.5
                mpz_t converted_int;
                mpz_init (converted_int);
                nearest_integer(&temp,&converted_int);
                mpf_set_z(temp2, converted_int);
                for(int i=0; i!=BASE_SIZE; i++){
                    mpf_mul(temp, v[j]->coefficients[i], temp2);
                    mpf_sub(v[k]->coefficients[i], v[k]->coefficients[i], temp);
                }
                gram_schmidt(v,u,number_of_vectors);
            }
        }

        mpf_t right_side, left_side;
        mpf_init(right_side); mpf_init(left_side);

        dot_product(u[k],u[k],&left_side);
        dot_product(u[k-1],u[k-1],&right_side);

        get_u_ij(v,u,k,k-1,&temp);
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
            k = k-1>2 ? k-1 : 2;
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


void init_vector(struct Vector* vector){
    for(int i=0; i!=BASE_SIZE; i++){
        mpf_init(vector->coefficients[i]);
    }
}

void clear_vector(struct Vector* v){
    for (int i = 0; i!=BASE_SIZE; i++) {
        mpf_clear(v->coefficients[i]);
    }
}

void print_vector(struct Vector* vector){
    for(int i=0; i!=BASE_SIZE; i++){
        //gmp_printf("\nq = %Zd\n", q);
        if(i!=BASE_SIZE-1)
            gmp_printf("%. *Ff,", 10, vector->coefficients[i]);
        else
            gmp_printf("%.* Ff\n", 10, vector->coefficients[i]);
    }
}


void test_gram_schmidt(){
    mpf_set_default_prec(DEFAULT_SIZE);
    struct Vector b1, b2, b3;
    struct Vector c1, c2, c3;
    init_vector(&b1);init_vector(&b2);init_vector(&b3);
    init_vector(&c1);init_vector(&c2);init_vector(&c3);
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
    for(int i=0; i!=BASE_SIZE; i++)
        print_vector(v[i]);

    clear_vector(&b1); clear_vector(&b2); clear_vector(&b3);
    clear_vector(&c1); clear_vector(&c2); clear_vector(&c3);
}