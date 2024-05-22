#include "main.h"

static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence */
bignumber super_increasing_sequence(int n, bignumber *sequence)
{
    if (n <= 0)
        return 0;

    if(!randomized)
    {
        srand(time(NULL));
        randomized = true;
    }

    sequence[0] = rand() % 10 + 1;
    bignumber sum = sequence[0];
    for (int i = 1; i < n; i++)
    {
        sequence[i] = sum + rand() % sequence[i - 1] + 1;
        sum += sequence[i];
    }

    return (rand() % 2 * sum + sum);
}

/* calculates pcgd between 2 integers */
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

/* return an integer coprime with q with value > minValue
 * minValue isn't necessary but we it's safer than having a small r */
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


int main(int argc, char *argv[])
{
    // test sequence 
    bignumber sequence[20];
    bignumber q = super_increasing_sequence(20, sequence);
    
    for (int i = 0; i < 20; i++)
        printf("%lld /", sequence[i]);
    printf("\nq = %lld\n",q);

    // la suite ressemble un peu a la suite de fibonacci 
    // elle augmente en taille de façon expo

    bignumber r = find_coprime(q, q/2);
    printf("r = %lld\n",r);
    printf("gcd = %lld\n",gcd(r,q));


    return 0;

}