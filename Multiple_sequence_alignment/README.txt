I have used Python as a programming language in this assignment.
The program is the implementation of Multiple Sequence Alignment(sequence to profile alignment)
To implement sequence-to-profile alignment, I have created a method createProfile() that creates the profile
according to the sequence list. Also, I have implemented another method named getScore(). This function takes profile,
char, and column number as input and returns the total score of the char given according to the column number.
After implementing these methods, I created a method named multipleAllignment() which takes sequences list and sequence.

In this method, the program first calls createProfile() method to create the sequences' profile; then, it creates a dynamic programming and backtracking
table. After creating these tables, it backtraces the backtrack table and forms alignment. Then the program writes this alignment into
the .txt file whose name is taken by arg parsing.

You can use seq.fasta and seqs.aln files to test.
