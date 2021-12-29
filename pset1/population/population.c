#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int current_size = get_int("What is the population starting size?\n");
    while (current_size < 9){
        current_size = get_int("Please enter a starting size greater or equal to 9.\n");
    }
    // Prompt for end size
    int end_size = get_int("What is the population ending size?\n");
    while(end_size < current_size){
        end_size = get_int("Please enter a end size greater than starting size.\n");
    }
    // Calculating the number of years until we reach threshold
    int years = 0;
    while (current_size < end_size){
        current_size = current_size - current_size/4 + current_size/3;
        years++;
    }
    // Print number of years
    printf("Years: %i\n", years);
}