import csv
import sys


def main():

    # TODO: Check for command-line usage
    header = []
    sequence = {}

    if len(sys.argv) != 3:
        sys.exit()

    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        header = reader.fieldnames

    rows = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            rows.append(row)

    readData = ""
    with open(sys.argv[2]) as file:
        readData = file.read()

    for key in header:
        if key != "name":
            sequence[key] = longest_match(readData, key)

    for row in rows:
        isPerson = True
        for key in sequence:
            if int(row[key]) != sequence[key]:
                isPerson = False
        if isPerson:
            print(row["name"])
            break
    else:
        print("No match")

    # TODO: Read database file into a variable

    # TODO: Read DNA sequence file into a variable

    # TODO: Find longest match of each STR in DNA sequence

    # TODO: Check database for matching profiles

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
