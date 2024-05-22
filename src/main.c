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
        sequence[i] = sum + rand() % 10 + 1;
        sum += sequence[i];
    }

    return sequence;
}


int main(int argc, char *argv[])
{
    printf("yo\n");
}