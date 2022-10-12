#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Prototypes
void usage(string program_name);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    string program_name = argv[0];

    if (argc == 2) // exactly 1 argument was passed as intended
    {
        // Error handling
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                usage(program_name);
                return 1;
            } 
        }

        // Convert string to int
        int n = atoi(argv[1]);

        if (n < 1) // Error, print help and quit
        {
            usage(program_name);
            return 1;
        }
        else // Rot word using 'n' rotations
        {
            // Do the prep
            string plaintext = get_string("plaintext: ");
            int len = strlen(plaintext);
            // printf("Len: %d\n", len);
            string ciphertext = malloc(sizeof(char) * len);
            
            // Do the magic
            for (int i = 0; i < len; i++)
            {
                if (plaintext[i] >= 65 && plaintext[i] <= 122)
                {
                    // Get letter and figure out the rotation
                    char *letter = &plaintext[i];
                    int pos = 0;
                    int rot = 0;
                    int new_letter = 0;

                    if (*letter >= 65 && *letter <= 90) // Mayus
                    {
                        pos = *letter - 64;
                        // printf("pos: %d\n", pos);

                        rot = n + pos;
                        if (rot > 26)
                        {
                            rot = rot % 26;
                        }

                        new_letter = rot + 64;

                        // printf("rot: %d\n", rot);
                        // printf("new_letter: %d\n", new_letter);
                        ciphertext[i] = (char) new_letter;

                        // printf("%c\n", ciphertext[i]);

                    }
                    else if (*letter >= 97 && *letter <= 122) // Minus
                    {
                        pos = *letter - 96;
                        // printf("pos: %d\n", pos);
                        
                        rot = n + pos;
                        if (rot > 26)
                        {
                            rot = rot % 26;
                        }

                        new_letter = rot + 96;

                        // printf("rot: %d\n", rot);
                        // printf("new_letter: %d\n", new_letter);
                        ciphertext[i] = (char) new_letter;

                        // printf("%c\n", ciphertext[i]);
                    }
                }
                else
                {
                    ciphertext[i] = plaintext[i];
                    continue;
                }
            }

            // Print result
            printf("ciphertext: %s\n", ciphertext);
            free(ciphertext);
        }
        
        return 0;
    }
    else // no valid arguments were passed. Print help and exit
    {
        usage(program_name);
        return 1;
    }
}

char rotate(char c, int n)
{
    return '0';
}

// Print help
void usage(string program_name)
{
    printf("Usage: %s <positive ROT number>\n", program_name);
}