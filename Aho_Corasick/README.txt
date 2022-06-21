I have used C++ as a programming language in this assignment.
The program is implementation of Aho-Corasick algorithm.
To implement Aho-Corasick first I have implemented Trie data structure by using struct Nodes
After implementing and making sure that my keyword tree is working properly I have imlemented a function
to set Fail State of each node. To set Fail State of each node I traverse through tree by using Bradth First
Search and call setFailureState function for each node. After setting all failiure state of nodes I have implemented
Aho Corasick Algorith as it is tought in the course.
