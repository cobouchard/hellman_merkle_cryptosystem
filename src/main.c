#include "main.h"
// key generation
static bool randomized = false;

/* initialize a super increasing sequence of n integers 
 * and return q an integer > sum of all elements of the sequence */
unsigned long long super_increasing_sequence(int n, unsigned long long *sequence)
{
    if (n <= 0)
        return 0;

    if(!randomized)
    {
        srand(time(NULL));
        randomized = true;
    }

    sequence[0] = rand() % 10 + 1;
    unsigned long long sum = sequence[0];
    for (int i = 1; i < n; i++)
    {
        sequence[i] = sum + rand() % sequence[i - 1] + 1;
        sum += sequence[i];
    }

    return (rand() % 2 * sum + sum);
}



int main(int argc, char *argv[])
{
    // test sequence 
    unsigned long long sequence[20];
    unsigned long long q = super_increasing_sequence(20, sequence);
    
    for (int i = 0; i < 20; i++)
        printf("%lld /", sequence[i]);
    printf("\nq = %lld\n",q);

    // la suite ressemble un peu a la suite de fibonacci 
    // elle augmente en taille de façon expo
    // donc pas sur de pouvoir choisir des tailles de blocs super grandes.

    return 0;
}