#include <stdio.h>
void draw(int n, int t);

int main(void)
{
    // Declare height
    int height;

    // Create while loop -> if input not satisfactory, repeat
    bool correct = false;
    while (!correct)
    {
        printf("Height: ");
        int error = scanf("%i", &height);

        // Is it within the requisites?
        if (height >= 1 && height <= 8 && error == 1)
        {
            correct = true;
        }
    }

    // Call draw with height between 1 and 8
    draw(height, height);
}

// Recursive function taking height twice as parameter, where n will be subtracted and t will remain height to keep track of the levels
void draw(int n, int t)
{
    // If not 0 and not negative
    if (n >= 2)
    {
        draw(n - 1, t);
    }

    // Subtraction to calculate the white spaces
    int spaces = t - n;

    // Make the spaces
    for (int j = 0; j < spaces; j++)
    {
        printf(" ");
    }

    // Make the pyramid of the left hand side
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }

    // Space in the middle
    printf("  ");

    // Make right hand side pyramid
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }

    // Finish level
    printf("\n");
}