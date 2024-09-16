#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

float lws[] = {0, 0, 0};

void countLWS(string text);

int main(void)
{
    string text = get_string("Text: ");
    countLWS(text);

    float index = 0.0588 * ((lws[0]/lws[1]) * 100) - 0.296 * ((lws[2]/lws[1]) * 100) - 15.8;

    int grade = (int) round(index);

    if(grade < 0)
    {
        printf("Before Grade 1\n");
    } else if(grade > 16)
    {
        printf("Grade 16+\n");
    } else
    {
        printf("Grade %i\n", grade);
    }
}

void countLWS(string text)
{
    for(int i = 0, n = strlen(text); i < n; i++)
    {
        if(tolower(text[i]) >= 'a' && tolower(text[i]) <= 'z')
        {
            lws[0]++;
        } else if(text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            lws[2]++;
        } else if(text[i] == ' ')
        {
            lws[1]++;
        }
    }
    lws[1]++;
}
