#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //get input
    int n;
    do
    {
        n = get_int("Size: ");
    }
    while (n < 1 || n > 8);

    //construct loops
    for (int i = 0; i < n; i++)
    {
        //Starting Spaces
        for (int l = 0; l < n - i - 1; l++)
        {
            printf(" ");
        }
        //Staring #s
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        //Middle Spaces
        printf("  ");
        //Ending #s
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        //new line
        printf("\n");
    }

}