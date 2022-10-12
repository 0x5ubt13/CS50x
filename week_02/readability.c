#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int calculate_grade(string text);

// index = 0.0588 * L - 0.296 * S - 15.8
// where L is the average number of letters per 100 words in the text,
// and S is the average number of sentences per 100 words in the text.

int main(void)
{
    // Get text
    string text = get_string("Please enter your text: ");

    // Calculate grade
    int grade = calculate_grade(text);

    // Print grade, max 16, min 1
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", (int) grade);
    }
}

// Count the total number of letters in the text
int count_letters(string text, int len)
{
    int letters_count = 0;

    for (int ch = 0; ch < len; ch++)
    {
        if (isalpha(text[ch]))
        {
            letters_count++;
        }
    }

    return letters_count;
}

// Count the total number of words in the text
int count_words(string text, int len)
{
    int words_count = 1;

    for (int ch = 0; ch < len; ch++)
    {
        if (isspace(text[ch]))
        {
            words_count++;
        }
    }

    return words_count;
}

// Count the total number of sentences in the text,
// Without having into account the contractions like Mr. or Mrs.
int count_sentences(string text, int len)
{
    int sentences_count = 0;

    for (int ch = 0; ch < len; ch++)
    {
        if (text[ch] == '!' || text[ch] == '?' || text[ch] == '.')
        {
            sentences_count++;
        }
    }

    return sentences_count;
}

int calculate_grade(string text)
{
    // Calculate length of text and pass it as arg
    int len = strlen(text);

    // Calculate number of letters, words and sentences
    double letters = count_letters(text, len);
    double words = count_words(text, len);
    double sentences = count_sentences(text, len);

    // printf("%d letters\n", letters);             // |
    // printf("%d words\n", words);                 //  } debug
    // printf("%d sentences\n", sentences);         // |

    // Calculate average of letters per 100 words
    double L = letters / words * 100.00;

    // Calculate average of sentences per 100 words
    double S = sentences  / words * 100;

    // Put everything together using Coleman-Liau's formula
    double grade = round((0.0588 * L) - (0.296 * S) - 15.8);


    return (int) grade;
}