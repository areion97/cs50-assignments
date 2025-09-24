#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);


int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    float L = (float)letters / words * 100;
    float S = (float)sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int roundedIndex = (int)round(index);

    if (roundedIndex < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    else if (roundedIndex >= 16)
    {
        printf("Grade 16+\n");
        return 0;
    }

    printf("Grade %d\n", roundedIndex);
    return 0;
}

int count_letters(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = tolower(text[i]);
        if ((int)letter - 97 >= 0 && (int)letter - 97 <= 25)
        {
            counter++;
        }
    }

    return counter;
}

int count_words(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = tolower(text[i]);
        if ((int)letter - 32 == 0)
        {
            counter++;
        }
    }

    return counter + 1;
}

int count_sentences(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        char letter = tolower(text[i]);
        if ((int)letter - 46 == 0 || (int)letter - 33 == 0 || (int)letter - 63 == 0)
        {
            counter++;
        }
    }

    return counter;
}