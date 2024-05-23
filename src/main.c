#include "main.h"
#include "gram-schmidt.h"

static gmp_randstate_t state;
static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence */
void super_increasing_sequence(int n, mpz_t *sequence, mpz_t *res)
{
    if (n <= 0)
        return ;

    if(!randomized)
    {   
        gmp_randinit_default(state);
        srand(time(NULL));
        randomized = true;
    }
    
    mpz_set_ui(sequence[0], rand()% 10 + 1);

    mpz_t sum;
    mpz_t temp;
    mpz_init2(sum, 1024UL);
    mpz_init2(temp, 1024UL);

    for (int i = 1; i < n; i++)
    {   
        mpz_urandomm(temp, state, sequence[i - 1]);
        mpz_add_ui(temp, temp, 1);          // urandomm generates from 0
        mpz_add(sequence[i], sum, temp);
        mpz_add(sum, sum, sequence[i]);
    }

    mpz_urandomm(temp, state, sum);
    mpz_add((*res), temp, sum);

    mpz_clear(temp);
    mpz_clear(sum);
}

/* calculates pcgd between 2 integers */
/*
bignumber gcd(bignumber  a, bignumber  b)
{
    while(b != 0)
    {
        bignumber temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
*/

/* return an integer coprime with q with value > minValue
 * minValue isn't necessary but we it's safer than having a small r */
/*
bignumber find_coprime(bignumber q, bignumber minValue)
{
    int smallPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int n = 25;
    bignumber r = 1;

    for (int i = 0; i < n ; i++)
    {
        if (gcd(smallPrimes[i], q) == 1)
        {   
            r *= smallPrimes[i];
            if(r > minValue)
                return r;
        }
    }
    return r;
}
*/

int main(int argc, char *argv[])
{
    // test sequence 
    int n = 100;
    mpz_t sequence[n];
    for (int i = 0; i < n; i++)
        mpz_init2(sequence[i], 1024UL);

    mpz_t q;
    mpz_init2(q, 1024UL);
    super_increasing_sequence(n, sequence, &q);
    
    
    for (int i = 0; i < 100; i++)
    {
        gmp_printf ("%Zd /", sequence[i]);
    }
    gmp_printf("\n q = %Zd\n", q);

    
    // la suite ressemble un peu a la suite de fibonacci 
    // elle augmente en taille de façon expo
    /*
    bignumber r = find_coprime(q, q/2);
    printf("r = %lld\n",r);
    printf("gcd = %lld\n",gcd(r,q));
    */

    mpz_clear(q);
    for (int i = 0; i < n; i++)
        mpz_clear(sequence[i]);
    return 0;

}
