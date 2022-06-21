I have used Python as a programming language in this assignment.
The program is an implementation of an algorithm that is very similar to BLAST


Classes:

-> hashItem:
	variables:
		index, k_mer, queryIndex
	This class's objects are used in hash table.
	They represent the hash table's items.

-> HSPItem
	variables:
		score, queryPair, sequencePair, queryIndex, sequenceIndex, queryNo, k_mer
	This class's objects represents high-segment-score pairs

Functions: 
findMatches(): Finds and returns all exact k_mer matches

simpleAlign(): returns alignment puts letter if match, puts '-' if mismatch

getBlank(index):  Returns blank space with the length of given str

getAlignment(): this function makes the final alignment. However, as I commented in the hw4.py file
               """SINCE IT IS NOT WANTED 
	       IN THE OUTPUT I COMMENTED OUT THE PRINT STATEMENTS IN ALIGNMENT FUNCTION,
               ONE CAN UNCOMMENT THEM TO SEE THE ALIGNMENT RESULTS"""
	       Users can uncomment those lines in this function to see the results of alignments.


insertionSort(): Sorts the given list of HSPItems concerning 'sequenceIndex'(Index of pair in the reference file)



And also there is the main function. In this function, the program first gets variable from arg parsing, then 
reads genome and queries files.

After that, it creates the hashtable and finds all matches for k_mers in queries.


After, it starts to apply the seed-and-extend strategy. Once the seed-and-extend strategy has finished the
the program gives a list of HSPPairs. 
After getting the list, the program first groups HSPPPairs by their query number(i.e., sequence number)
and sorts the groups concerning sequenceIndex(index of pair in reference file)

Then it aligns all HSPPairs by calling getAlignment() function and prints the starting location of each alignment.

NOTE: Since I've tried with different queries.fasta and genome.fasta files (because it takes a lot of time to process original file) in my makefile
k = 5 and s = 10
