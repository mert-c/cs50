#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>

//Define methods
int test(int ,string []);
int subs(string, string);

//Main function to check inputs and print out result
int main(int argc, string argv[])
{
    //check if the key is valid - if not return 1 and error message
    if (test(argc, argv))
    {
        return 1;
    }
    else
    {
        //cypher the message using subs
        string key = argv[1];
        string text = get_string("plaintext: ");
        return subs(text, key);
    }

}

//Input testing function
int test(int args, string key[])
{
    //Testing argument numbers
    if (args != 2)
    {
        printf("Usage: ./substitution key.\n");
        return 1;
    }
    //Testing lenght of the key
    else if (strlen(key[1]) != 26)
    {
        printf("The key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        for (int i = 0, n = strlen(key[1]); i < n; i++)
        {
            //Testing if the key is alphabetical or not
            char ith = key[1][i];
            if (!isalpha(ith))
            {
                printf("The key must contain only alphabetic characters\n");
                return 1;
            }
            for (int j = 0; j < n; j++)
            {
                //Testing if the key includes repeated characters or not
                int jth = key[1][j];
                if (toupper(ith) == toupper(jth) && i != j)
                {
                    printf("The key must not contain repeated characters\n");
                    return 1;
                }
            }
        }
    }

    return 0;
}

//Substitution function
int subs(string text, string key)
{
    int val;
    int n = strlen(text);
    char cth;
    char cypher[n];

    for (int i = 0; i < n; i++)
    {
        char c = text[i];
        bool alpha = isalpha(text[i]);

        if (alpha && islower(c))
        {
            val = c - 97;
            cth = tolower(key[val]);
        }
        else if (alpha && isupper(c))
        {
            val = c - 65;
            cth = toupper(key[val]);
        }
        else
        {
            cth = c;
        }
        cypher[i] = cth;
    }
    printf("ciphertext: %s\n", cypher);
    return 0;
}