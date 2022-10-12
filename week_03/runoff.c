#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define _CRT_INT_MAX 2147483647

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Checking name is correct
    {   
        // For candidate in candedates
        for (int i = 0; i < candidate_count; i++)
        {
            // If candidate.name == valid candidate name, update references table
            if (strcmp(candidates[i].name, name) == 0)
            {
                preferences[voter][rank] = i;
                return true;
            }
        } 
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    printf("Entering tabulate\n");
    // Check the references table and vote accordingly

    // Preferences[i][] loop
    for (int i = 0; i < voter_count; i++)
    {
        int j = 0;

        // Preferences[][j] loop
        int done = 0;
        while (!done)
        {
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes += 1;
                
                done = 1;
            }

            j++;
        }
    }

    // DEBUG:
    printf("Preferences table:\n");
    for (int i = 0; i < voter_count; i++)
    {
        // Preferences[][j] loop
        for (int j; j < candidate_count; j++)
        {   
            printf("Preferences[%i][%i] = %i\n", i, j, preferences[i][j]);
        }
    }

    printf("Votes:\n");
    for (int i = 0; i < voter_count; i++)
    {
        printf("candidates[%i].votes = %i\n", i, candidates[i].votes);
    }

    printf("Exiting tabulate\n");
    
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes > voter_count / 2)
            {
                printf("%s\n", candidates[i].name);
                return true;
            }
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = _CRT_INT_MAX;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int valid_candidates = 0;
    int min_candidates = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            // Take a note of how many of the candidates are not eliminated
            valid_candidates++;

            // Take a note of how many of the not eliminated candidates have the minimum number of votes
            if (candidates[i].votes == min)
            {
                min_candidates++;
            }
        }
    }

    // Tie
    if (valid_candidates == min_candidates)
    {
        return true;
    }

    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            // Take a note of how many of the not eliminated candidates have the minimum number of votes
            if (candidates[i].votes == min)
            {   
                candidates[i].votes = 0;
                candidates[i].eliminated = true;
            }
        }
    }

    return;
}