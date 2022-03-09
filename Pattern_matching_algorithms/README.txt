I have used C++ as a programming language.
The program consists of 3 different exact string matching algorithms: Brute Force, Knuth-Morris-Pratt, and Boyer-Moore.
The program reads text and pattern files in FATSA format and uses an argument parser to get their names. Also, it takes which algorithm to run
from the argument parser. The program calculates specified algorithms' run times and prints them.
Possible algorithm options are:

BF: Runs Brute Force algorithm, prints total comparison count, its runtime.
KMP: Runs Knuth-Morris-Pratt algorithm, prints total comparison count, its runtime.
BM: Runs Boyer-Moore algorithm, prints total comparison count, its runtime.
A: Runs all three algorithms, prints their total comparison counts, and compares their runtimes to find the fastest one.

