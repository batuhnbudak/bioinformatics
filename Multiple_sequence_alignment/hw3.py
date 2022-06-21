import argparse

def getRegularScore(char1, char2):
    if(char1 == '-' or char2 == '-'):
        return gap
    elif(char1 == char2):
        return match
    else:
        return mismatch

def getScore(profile,char, col):
    
    # A
    score = getRegularScore('A',char)*profile[0][col]
    # C
    score = score  + getRegularScore('C',char)*profile[1][col]
    # G
    score = score  + getRegularScore('G',char)*profile[2][col]
    # T
    score = score  + getRegularScore('T',char)*profile[3][col]
    # -
    score = score  + getRegularScore('-',char)*profile[4][col]

    return score


def createProfile(sequences):
    row, col = len(sequences), len(sequences[0])
    
    profil = [[0 for i in range(col)] for j in range(row)]

    for j in range(0, col):
        aCount = 0
        cCount = 0
        tCount = 0
        gCount = 0
        deletionCount = 0
        for i in range(0,row):
            if(sequences[i][j] == 'A'):
                aCount = aCount + 1
            elif(sequences[i][j] == 'C'):
                cCount = cCount + 1
            elif(sequences[i][j] == 'T'):
                tCount = tCount + 1
            elif(sequences[i][j] == 'G'):
                gCount = gCount + 1
            elif(sequences[i][j] == '-'):
                deletionCount = deletionCount + 1
            else:
                print("Error not valid sequence: ")

        profil[0][j] = aCount / row
        profil[1][j] = cCount / row
        profil[2][j] = gCount / row
        profil[3][j] = tCount / row
        profil[4][j] = deletionCount / row
    
    #for rows in profil:
     #   print(rows)
    return profil

def getMax(num1,num2,num3):
    backTrack = '-'
    if (num1 >= num2) and (num1 >= num3):
        largest = num1
        backTrack = 'd'
    elif (num2 >= num1) and (num2 >= num3):
        largest = num2
        backTrack = 'u'
    else:
        largest = num3
        backTrack = 'l'
    return largest, backTrack

def multipleAllignment(seq, sequences):#row 18
    # Preprocess
    profile = createProfile(sequences)
    allignment = ""
    row = len(seq) + 1
    col = len(profile[0]) + 1
    
    V = [[0 for i in range(col)] for j in range(row)]
    bcTable = [[0 for i in range(col)] for j in range(row)]
    V[0][0] = 0
    bcTable[0][0] = '-'
    for j in range(1,col):
        V[0][j] = V[0][j-1] + getScore(profile,'-',j-1)
        bcTable[0][j] = '-'
    for i in range(1, row):
        V[i][0] = V[i-1][0] + getRegularScore(seq[i-1],'-')
        bcTable[i][0] = '-'
    returnVal = []
    for i in range(1, row):
        for j in range(1, col):
            num1 = V[i-1][j-1] + getScore(profile,seq[i-1],j-1) # Diagonal
            num2 = V[i-1][j] + getRegularScore(seq[i-1],'-') # up
            num3 = V[i][j-1] + getScore(profile,'-',j-1) # left
            returnVal = getMax(num1,num2,num3)
            V[i][j] = returnVal[0]
            bcTable[i][j] = returnVal[1]

    # Backtrace
    i = row - 1
    j = col - 1
    while(i >= 1):
        while(j >= 1):
            if(bcTable[i][j] == 'd'):
                allignment = seq[i - 1] + allignment
                i = i - 1
                j = j - 1
            elif(bcTable[i][j] == 'u'):
                allignment = '-' + allignment
                i = i - 1
            elif(bcTable[i][j] == 'l'):
                allignment = '-' + allignment
                j = j - 1

    return(allignment)       

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--fasta")
    parser.add_argument("--aln")
    parser.add_argument("--out")
    parser.add_argument("--match")
    parser.add_argument("--mismatch")
    parser.add_argument("--gap")
    
    args = parser.parse_args()
    fastaFileName = args.fasta
    alnFileName = args.aln
    outFileName = args.out
    global match
    match  = int(args.match)
    global mismatch
    mismatch  =  int(args.mismatch)
    global gap 
    gap = int(args.gap)
    sequences = []
        
    # Read and save seqs.aln file
    seqsFile = open(alnFileName)
    with open(alnFileName) as f:
        lines = f.readlines()

    
    dummyList = []
    for line in lines:
        dummyList = line.split()
        sequences.append(dummyList[1])
     
    # Read seq.fasta file
    with open(fastaFileName) as f:
        lines = f.readlines()
    
    seq = lines[1]

    allignment = multipleAllignment(seq, sequences)
    f = open(outFileName, "w")

    count = 1
    for seq in sequences:
        f.write("sequence" + str(count) + "\t" + seq + "\n")
        count += 1
    f.write("sequence\t" + allignment)
    f.close()

    print(allignment)