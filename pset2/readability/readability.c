//printf("Sentences: %i\n",sentences);
#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
//printf("Sentences: %i\n",sentences);
int get_letters(string text)
{
    int num = 0;

    for (int i = 0, a = strlen(text); i < a ; i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            num++;
        }
    }

    return num;
}
//printf("Sentences: %i\n",sentences);
int get_words(string text)
{
    int num = 1;

    for (int i = 0, a = strlen(text); i < a ; i++)
    {
        if (isspace(text[i]))
        {
            num++;
        }
    }

    return num;
}
//printf("Sentences: %i\n",sentences);
int get_sentences(string text)
{
    int num = 0;

    for (int i = 0, a = strlen(text); i < a ; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            num++;
        }
    }

    return num;
}

int main(void)
{
    string text = get_string("Text: ");
    //printf("Sentences: %i\n",sentences);
    int letters = get_letters(text);
    float words = get_words(text);
    int sentences = get_sentences(text);
    printf("%f", L)
    printf("%f", W)
    printf("%f", S)
    float L = 100 * (letters / words);
    float S = 100 * (sentences / words);
    //printf("Letters: %i\n",letters);
    //printf("Words: %f\n",words);

    //printf("L is %i\n",L);
    //printf("S is %i\n",S);
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //printf("Index: %i\n",index);
    //printf("Sentences: %i\n",sentences);
    if (index <= 1)
    {
        printf("%s\n", "Before Grade 1");
    }
    else if (index >= 16)
    {
        printf("%s\n", "Grade 16+");
    }
    else
    {
        printf("%s%i\n", "Grade ", index);
    }
}

