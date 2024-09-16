#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int i);
bool check_cycle_util(int i, bool visited[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j && (preferences[i][j] > preferences[j][i]))
            {
                pair pairIJ;
                pairIJ.winner = i;
                pairIJ.loser = j;
                pairs[pair_count] = pairIJ;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] <
                preferences[pairs[j].winner][pairs[j].loser])
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (check_cycle(i))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

bool check_cycle(int i)
{
    bool visited[candidate_count];

    for (int j = 0; j < candidate_count; j++)
    {
        visited[j] = false;
    }
    // starting from the first node
    return check_cycle_util(pairs[i].winner, visited);
}

bool check_cycle_util(int i, bool visited[])
{
    if (visited[i])
    {
        return true;
    }

    visited[i] = true;

    for (int j = 0; j < candidate_count; j++)
    {
        // going to check the next node, that is pointed to
        if (locked[i][j] && check_cycle_util(j, visited))
        {
            return true;
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    bool winner = true;
    int can_index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            // checking the candidate columm with i
            if (locked[j][i] == true)
            {
                winner = false;
                break;
            }
        }
        if (winner == true)
        {
            can_index = i;
            break;
        }
    }
    if (winner == true)
    {
        printf("%s\n", candidates[can_index]);
    }
}