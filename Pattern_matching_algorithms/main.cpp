#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include "argh.h"
using namespace std;
using namespace std::chrono;

// Brute force
bool bruteForce(string T, string P, int* compCount, int* pos)
{
    // Vars
    int n = T.length();
    int m = P.length();
    int i = 0;
    int count = 0;
    string toCompare;
    bool flag = false;

    while(i < n - m + 1)
    {
        flag = true;
        toCompare = T.substr(i,m);
        for(int j = 0; j < m; j++)
        {
            count++;
            if(P[j] != toCompare[j])
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            *compCount = count;
            *pos = i + 1;
            return true;
        }
        else
        {
            i++;
        }
    }
    return false;
}

void failureFunction(string P, int* F)
{
    int i, j, m;
    F[0] = 0;

    i = 1;
    j = 0;
    m = P.length();
    while(i < m)
    {
        if(P[i] == P[j])
        {
            F[i] = j + 1;
            i = i + 1;
            j = j + 1;
        }
        else if( j > 0)
        {
            j = F[j - 1];
        }
        else
        {
            F[i] = 0;
            i = i + 1;
        }
    }
}

// KMP
bool KMP(string T, string P, int* compCount, int* pos, int* F)
{
    int i, j, n, m;
    int count = 0;
    failureFunction(P, F);
    i = 0;
    j = 0;
    n = T.length();
    m = P.length();
    while(i < n)
    {
        count++;
        if(T[i] == P[j])
        {
            if(j == m - 1)
            {
                *compCount = count;
                *pos = i - j + 1;
                return true;
            }
            else
            {
                i++;
                j++;
            }
        }
        else
        {
            if(j > 0)
            {
                j = F[j - 1];
            }
            else
            {
                i++;
                j = 0;
            }
        }
    }
    return false;
}

// Check if string s1 is a suffix of string s2
bool isSuffix(string s1, string s2)
{
    int len1 = s1.length();
    int len2 = s2.length();

    if(len1 > len2)
    {
        return false;
    }
    else
    {
        for(int i = 0; i < len1; i++)
        {
            if(s1[len1 - i - 1] != s2[len2 - i - 1])
                return false;
        }
        return true;
    }
}
void goodSuffix1(string P, int* gs1)
{
    int k;
    int m = P.length();
    string s1, s2;
    bool flag;
    for(int j = 0; j < m; j++)
    {
        flag = true;
        k = m - 1;
        s1 = P.substr(j+1);
        while(k > 0 && flag)
        {
            s2 = P.substr(0,k);

            if((unsigned) k < s1.length() || s1.length() == 0)
            {
                gs1[j] = 0;
                break;
            }
            if((isSuffix(s1,s2)) && (k-m+j) >= 0 && (P[k-m+j] != P[j]) && (m-j+1 <=k) && (k < m))
            {
                gs1[j] = k;
                flag = false;
            }
            else
            {
                k--;
            }
            if(k == 0)
            {
                gs1[j] = 0;
                flag = false;
            }
        }
    }
}

void goodSuffix2(string P, int* gs2)
{
    int k;
    string s1, s2;
    int m = P.length();
    bool flag;
    for(int j = 0; j < m; j++)
    {
        flag = true;
        k = m - j;
        s2 = P.substr(j+1);
        while(k >= 1 && flag)
        {
            s1 = P.substr(0,k);
            if(isSuffix(s1,s2))
            {
                gs2[j] = k;
                flag = false;
            }
            else
            {
                k--;
            }
            if(k == 0)
            {
                gs2[j] = 0;
            }
        }
    }
}

void formGS(int* gs1, int* gs2, int m, int* gs)
{
    for(int i = 0; i < m; i++)
    {
        if(gs1[i] > gs2[i])
        {
            gs[i] = m - gs1[i];
        }
        else
        {
            gs[i] = m - gs2[i];
        }
    }
    gs[m - 1] = 1;
}
void badCharMatrix(string P, int** bArr)
{
    int m = P.length();
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            bArr[i][j] = -1;
        }
    }

    bool A, C, G, T;

    for(int i = m - 1; i >= 0; i--)
    {
        A = true;
        C = true;
        G = true;
        T = true;
        if(P[i] == 'A')
        {
            bArr[i][0] = i;
        }
        else if(P[i] =='C')
        {
            bArr[i][1] = i;
        }
        else if(P[i] =='G')
        {
            bArr[i][2] = i;
        }
        else if(P[i] =='T')
        {
            bArr[i][3] = i;
        }

        for(int j = i; j >= 0; j--)
        {
            if(P[j] == 'A' && A)
            {
                bArr[i][0] = j;
                A = false;
            }
            else if(P[j] =='C' && C)
            {
                bArr[i][1] = j;
                C = false;
            }
            else if(P[j] =='G' && G)
            {
                bArr[i][2] = j;
                G = false;
            }
            else if(P[j] =='T' && T)
            {
                bArr[i][3] = j;
                T = false;
            }
        }
    }
}
bool boyerMoore(string T, string P, int* compCount, int* pos, int* gs1, int* gs2, int* gs, int** bcr)
{
    int m, n;
    m = P.length();
    n = T.length();
    string toCompare;
    goodSuffix1(P, gs1);
    goodSuffix2(P, gs2);
    formGS(gs1, gs2, m, gs);
    badCharMatrix(P, bcr);

    int i = 0;
    int mismatchIndex;
    char mismatchChar;
    int bcrShift;
    int gsShift;
    int count = 0;
    bool flag = true;
    while(i < n - m + 1)
    {
        flag = true;
        toCompare = T.substr(i,m);

        for(int j = m -1; j >= 0; j--)
        {
            count++;
            if(P[j] != toCompare[j])
            {
                mismatchIndex = j;
                mismatchChar = toCompare[j];
                flag = false;
                break;
            }
        }
        if(flag)
        {
            *compCount = count;
            *pos = i + 1;
            return true;
        }
        else
        {
            if(mismatchChar == 'A')
            {
                bcrShift = mismatchIndex - bcr[mismatchIndex][0];
            }
            else if(mismatchChar == 'C')
            {
                bcrShift = mismatchIndex - bcr[mismatchIndex][1];
            }
            else if(mismatchChar == 'G')
            {
                bcrShift = mismatchIndex - bcr[mismatchIndex][2];
            }
            else if(mismatchChar == 'T')
            {
                bcrShift = mismatchIndex - bcr[mismatchIndex][3];
            }
            gsShift = gs[mismatchIndex];
            if(gsShift > bcrShift)
            {
                i = i + gs[mismatchIndex];
            }
            else
            {
                i = i + bcrShift;
            }
        }
    }
    return false;
}
int main(int argc, char *argv[])
{
    // Variables
    int* gs1;
    int* gs2;
    int* gs;
    int** bcr;
    int* F;

    string patternFileDir;
    string textFileDir;
    string algorithmSpecifier;
    string T;
    string P;
    string line;

    bool check;

    ifstream textFile;
    ifstream pattFile;

    argh::parser cmdl;

    cmdl.parse(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

    cmdl("-i") >> textFileDir;
    cmdl("-o") >> patternFileDir;
    cmdl("-a") >> algorithmSpecifier;

    // Reading files.
    textFile.open(textFileDir);
    pattFile.open(patternFileDir);

    // Reading TEXT file ( T )
    // Get rid of dummy ">text" at the first line of the FATSA file.
    getline(textFile, line);

    T = "";
    while(getline(textFile, line))
    {
        T = T + line;
    }
    textFile.close();
    // Reading PATT file ( P )

    // Get rid of dummy ">pattern" at the first line of the FATSA file.
    getline(pattFile, line);
    P = "";
    while(getline(pattFile, line))
    {
        P = P + line;
    }
    pattFile.close();

    int* compCount;
    int* pos;
    compCount = new int;
    pos = new int;
    gs = new int[P.length()];
    gs1 = new int[P.length()];
    gs2 = new int[P.length()];
    F = new int[P.length()];
    bcr = new int*[P.length()];
    for(int i = 0; i < P.length(); i++)
    {
        bcr[i] = new int[4];
    }
    if(algorithmSpecifier == "BF") // Brute force
    {
        auto start = std::chrono::high_resolution_clock::now();
        check = bruteForce(T,P,compCount,pos);
        auto finish = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
    }
    else if(algorithmSpecifier == "KMP") // Knuth-Morris-Pratt
    {
        auto start = std::chrono::high_resolution_clock::now();
        check = KMP(T,P,compCount,pos,F);
        auto finish = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
    }
    else if(algorithmSpecifier == "BM") // Boyer-boyerMoore
    {
        auto start = std::chrono::high_resolution_clock::now();
        check = boyerMoore(T,P,compCount,pos,gs1,gs2,gs,bcr);
        auto finish = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << duration.count() << " ms." << endl;
        }
    }
    else if(algorithmSpecifier == "A") // All of them
    {
        // BF first
        cout << "Brute Force:" << endl;
        auto startBF = std::chrono::high_resolution_clock::now();
        check = bruteForce(T,P,compCount,pos);
        auto finishBF = std::chrono::high_resolution_clock::now();

        auto durationBF = std::chrono::duration_cast<std::chrono::microseconds>(finishBF - startBF);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << durationBF.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << durationBF.count() << " ms." << endl;
        }
        cout << endl;

        // KMP second
        cout << "Knuth-Morris-Pratt:" << endl;
        auto startKMP = std::chrono::high_resolution_clock::now();
        check = KMP(T,P,compCount,pos,F);
        auto finishKMP = std::chrono::high_resolution_clock::now();

        auto durationKMP = std::chrono::duration_cast<std::chrono::microseconds>(finishKMP - startKMP);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << durationKMP.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << durationKMP.count() << " ms." << endl;
        }
        cout << endl;

        // BM third
        cout << "Boyer-Moore:" << endl;
        auto startBM = std::chrono::high_resolution_clock::now();
        check = boyerMoore(T,P,compCount,pos,gs1,gs2,gs,bcr);
        auto finishBM = std::chrono::high_resolution_clock::now();

        auto durationBM = std::chrono::duration_cast<std::chrono::microseconds>(finishBM - startBM);
        if(check)
        {
            cout << "pattern was found in query file at position " << *pos << "." << endl;
            cout << *compCount << " character comparisons performed." << endl;
            cout << "Run time was " << durationBM.count() << " ms." << endl;
        }
        else
        {
            cout << "Couldn't find the pattern in the query." << endl;
            cout << "Run time was " << durationBM.count() << " ms." << endl;
        }

        // Find best
        cout << endl;
        if(durationBF.count() < durationKMP.count())
        {
            if(durationBF.count() < durationBM.count())
            {
                cout << "The best algorithm was Brute-Force" << endl;
            }
            else
            {
                cout << "The best algorithm was Boyer-Moore" << endl;
            }
        }
        else if(durationKMP.count() < durationBM.count())
        {
            cout << "The best algorithm was Knuthh-Morris-Pratt" << endl;
        }
        else
        {
            cout << "The best algorithm was Boyer-Moore" << endl;
        }
    }
    else
    {
        cout << "Error. " << algorithmSpecifier << " is not an option. Program will be terminated..." << endl;
        exit(0);
    }

    if(compCount != NULL)
    {
        delete compCount;
    }
    if(pos != NULL)
    {
        delete pos;
    }

    if(gs2 != NULL)
    {
        delete[] gs2;
    }
    if(gs1 != NULL)
    {
        delete[] gs1;
    }

    if(gs != NULL)
    {
        delete[] gs;
    }
    if(F != NULL)
    {
        delete[] F;
    }
    for(int i = 0; i < P.length(); i++)
    {
        delete[] bcr[i];
    }
    delete[] bcr;

    return 0;

}
