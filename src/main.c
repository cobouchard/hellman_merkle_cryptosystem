#include "main.h"
// key generation
static bool randomized = false;

void super_increasing_sequence(int n, int *sequence)
{
    if (n <= 0)
        return;

    if(!randomized)
    {
        srand(time(NULL));
        randomized = true;
    }

    sequence[0] = rand() % 10 + 1;
    int sum = sequence[0];
    for (int i = 1; i < n; i++)
    {
        sequence[i] = sum + rand() % sequence[i - 1] + 1;
        sum += sequence[i];
    }
}


int main(int argc, char *argv[])
{
    // test sequence 
    int sequence[20];
    super_increasing_sequence(20, sequence);
    
    for (int i = 0; i < 20; i++)
        printf("%d /", sequence[i]);
    printf("\n");

    // la suite ressemble un peu a la suite de fibonacci 
    // elle augmente en taille de façon expo
    // donc pas sur de pouvoir choisir des tailles de blocs super grandes.

    
}