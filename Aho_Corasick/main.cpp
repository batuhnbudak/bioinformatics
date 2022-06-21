#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <bits/stdc++.h>
#include "argh.h"

using namespace std;

// Node for keyword tree
struct Node{
    Node* children[128];
    bool isEndOfWord;
    Node* n;
    Node* parent;
    int id;
    vector<int> nextState;
    char c;
    string output;
    int height;
};

// sets node's vars, returns new node
Node* createNewNode()
{
    Node* newNode = new Node;
    newNode->isEndOfWord = false;

    for(int i = 0; i < 128; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Insert a pattern into the keyword tree
int insert(Node* root, string pattern, int id)
{
    Node* temp = root;
    int index;
    for(unsigned int i = 0; i < pattern.length(); i++)
    {
        index = pattern[i] - '\0';
        if(temp->children[index] == NULL)
        {
            temp->children[index] = createNewNode();
            temp->children[index]->parent = temp;
            temp->children[index]->height = temp->height + 1;
            temp->children[index]->c = index + '\0';
            temp->children[index]->id = id;
            temp->nextState.push_back(id);
            id++;
            if(temp == root)
            {
                temp->children[index]->n = root;
            }
        }
        temp = temp->children[index];
    }
    temp->isEndOfWord = true;
    temp->output = pattern;
    return id;
}

// Sets the failure state of the given node v
void setFailureState(Node* v, Node* root)
{
    Node* vPrime = v->parent;
    char x = v->c;
    int index = x - '\0';
    Node* w = vPrime->n;

    while(!w->children[index] && w != root)
    {
        w = w->n;
    }
    if(w->children[index])
    {
        v->n = w->children[index];
    }
    else
    {
        v->n = root;
    }
}

// Traverse through tree by Breadth First Search and call setFailureState function to set its failure state
vector<Node*> failFunction(struct Node* node, struct Node* root)
{
    queue<Node*> q;
    vector<Node*> bfsVec;
    q.push(root);
    bfsVec.push_back(root);
    while(q.size() != 0)
    {
        Node* curr = q.front();
        q.pop();

        for(int i = 0; i < 128; i++)
        {
            if(curr->children[i])
            {
                q.push(curr->children[i]);
                bfsVec.push_back(curr->children[i]);
            }
        }
    }

    for(unsigned int i = 0; i < bfsVec.size(); i++)
    {
        if(bfsVec[i] != root && bfsVec[i]->parent != root)
        {
            setFailureState(bfsVec[i], root);
        }
    }
    return bfsVec;
}

// Prints tree by adding order(id)
void printTree(vector<Node*> vec)
{
    Node* node;
    for(unsigned int i = 0; i < vec.size(); i++)
    {
        for(unsigned int j = 0; j < vec.size(); j++)
        {
            node = vec[j];
            if(node->id == (int)i)
            {
                cout << "char: " << node->c << " ";
                cout << "next states: [";
                for(unsigned int i = 0; i < node->nextState.size(); i++)
                {
                    if(i != node->nextState.size() - 1)
                        cout << node->nextState[i] << " ,";
                    else
                        cout << node->nextState[i];
                }
                cout << "]";
                cout << " fail state: " << node->n->id;
                if(node->isEndOfWord)
                {
                    cout << " output: ['" << node->output << "']" << endl;
                }
                else
                {
                    cout << " output: []" << endl;
                }
            }
        }
    }
}

// Builds keyword tree(preprcess) and prints it (returns root of the tree)
Node* buildTree(vector<string> patternSet)
{
    vector<Node*> vec;
    Node* root = createNewNode();
    root->parent = NULL;
    root->n = root;
    root->id = 0;
    root->height = -1;
    root->c = '\0';
    int count = 1;

    for(unsigned int i = 0; i < patternSet.size(); i++)
    {
        count = insert(root,patternSet[i], count);
    }

    vec = failFunction(root, root);

    cout << "Build tree\n" << "----------------------------------------------------------------------" << endl;
    printTree(vec);

    cout << endl;
    return root;
}
// Aho- Corasick algorithm
void ahoCorasickAlgorithm(vector<string> patternSet, string text)
{
    // Variables
    int l, c, index, m;
    Node* root, *w;

    // Preprocess
    root = buildTree(patternSet);
    cout << "Search\n" << "----------------------------------------------------------------------" << endl;
    m = text.length();
    w = root;
    l = 1;
    c = 0;

    while(c < m)
    {
        index = text[c] - '\0';
        while(w->children[index])
        {
            w = w->children[index];
            c++;
            index = text[c] - '\0';
            if(w->isEndOfWord)
            {
                cout << "keyword: " << w->output << " index: " << l << endl;
            }
            //cout << "C is: " << c << endl;
        }
        w = w->n;
        l = c - w->height;
        c++;
    }
}
int main(int argc, char *argv[])
{
    ifstream file;
    string pattern;
    string line;
    string text = "";
    string textFileDir;
    vector<string> patternSet;

    argh::parser cmdl;

    cmdl.parse(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

    cmdl("-i") >> textFileDir;

    file.open(textFileDir);

    getline(file, pattern);
    getline(file, text);
    file.close();

    // Tokenizing the pattern
    stringstream check(pattern);
    while(getline(check, pattern, ' '))
    {
        patternSet.push_back(pattern);
    }

    ahoCorasickAlgorithm(patternSet, text);
    return 0;
}
