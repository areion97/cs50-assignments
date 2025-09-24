#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHA_LENGTH 26

bool validate_key(string key);

int main(int argc, string argv[])
{
    if (argc <= 1 || argc >= 3)
    {
        printf("Please only supply the cipher\nUsage: ./substitution key");
        return 1;
    }

    string cipher = argv[1];

    if (!validate_key(cipher))
    {
        return 1;
    }


    string plaintext = get_string("plaintext:  ");
    char ciphertext[strlen(plaintext)];

    for (int i = 0; i < strlen(plaintext); i++)
    {

        bool isLetterUpper = isupper(plaintext[i]);

        char letter = tolower(plaintext[i]);

        if ((int)letter - 97 <= 25 && (int)letter - 97 >= 0)
        {
            char alphaletter;

            // Scorre l'alfabeto per trovare la lettera
            for (int j = 0; j <= (int)letter - 97; j++)
            {
                alphaletter = cipher[j];
            }

            if (isLetterUpper)
            {
                alphaletter = toupper(alphaletter);
                ciphertext[i] = alphaletter;
            }
            else
            {
                alphaletter = tolower(alphaletter);
                ciphertext[i] = alphaletter;
            }

        }
        else
        {
            ciphertext[i] = letter;
        }

    }

    printf("ciphertext:  ");
    for (int i = 0; i < strlen(plaintext); i++)
    {
        printf("%c", ciphertext[i]);
    }
    printf("\n");

    return 0;

}

bool validate_key(string key)
{
    if (strlen(key) != 26)
    {
        printf("Not containing 26 characters\n");
        return false;
    }

    for (int i = 0; i < strlen(key) - 1; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key is not valid: containing one or more characters not alphabetic\n");
            return false;
        }
        for (int j = i + 1; j < strlen(key); j++)
        {
            if ((int)key[j] - (int)key[i] == 0)
            {
                printf("Key is not valid: containing duplicate\n");
                return false;
            }
        }
    }
    return true;

}
