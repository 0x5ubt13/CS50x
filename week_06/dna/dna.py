import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py <file.csv> <file.txt>")
        sys.exit(1)

    # Read database file into a variable
    with open(sys.argv[1], "r") as csv_file:
        db_reader = csv.DictReader(csv_file)

        # Read DNA sequence file into a variable
        with open(sys.argv[2], "r") as seq_file:
            seq = seq_file.readlines()

            # Find longest match of each STR in DNA sequence
            for row in db_reader:
                number_of_STR = 0
                matches = 0
                for k, v in row.items():                      
                    if k == "name":
                        name = v
                        continue
                    
                    number_of_STR += 1
                    longest_run = longest_match(seq[0], k)
                    if longest_run == int(v):
                        matches += 1

                # Check database for matching profiles
                if matches == number_of_STR:
                    print(name)
                    return
        
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
