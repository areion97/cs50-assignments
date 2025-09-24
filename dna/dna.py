import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return

    csvFilePath = sys.argv[1]
    dnaFilePath = sys.argv[2]
    dna = ''

    with open(dnaFilePath, "r") as dnaFile:

        for row in dnaFile:
            if (row != '\n' and row != ''):
                dna = row

    with open(csvFilePath, newline='\n') as csvFile:

        csvFileReader = csv.DictReader(csvFile)

        for row in csvFileReader:
            count = 0
            for key, numOfTimesRepeated in row.items():
                if key != 'name':
                    if longest_match(dna, key) == int(numOfTimesRepeated):
                        count += 1
                        if count == len(row.items()) - 1:
                            print(row['name'])
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
