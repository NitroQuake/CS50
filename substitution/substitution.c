#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    else if (strlen(argv[1]) < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                printf("Key must contain alphabetic characters.\n");
                return 1;
            }
            for (int j = 0, o = strlen(argv[1]); j < o; j++)
            {
                if (!isalpha(argv[1][j]))
                {
                    printf("Key must contain alphabetic characters.\n");
                    return 1;
                }
                else if (tolower(argv[1][i]) == tolower(argv[1][j]) && i != j)
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
        }
    }

    string input = get_string("plaintext: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isalpha(input[i]))
        {
            if (isupper(input[i]))
            {
                input[i] = toupper(argv[1][input[i] - 'A']);
            }
            else if (islower(input[i]))
            {
                input[i] = tolower(argv[1][input[i] - 'a']);
            }
        }
    }

    printf("ciphertext: %s\n", input);
}
