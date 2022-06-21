import argparse

# Hash Item class
class hashItem:
    def __init__(self, index, k_mer, queryIndex):
        self.index = index
        self.k_mer = k_mer
        self.queryIndex = queryIndex
    
    def __str__(self):
        return "| Reference index: " + str(self.index) + " | k_mer: " + str(self.k_mer) + " | queryIndex: " + str(self.queryIndex) + " |"

# HSPS Item class
class HSPItem:
    def __init__(self, score, queryPair, sequencePair, queryIndex, sequenceIndex, queryNo, k_mer): #HSPItem(score, queryPair, sequencePair, queryIndexStart + 1, sequenceIndexStart + 1, count))
        self.score = score
        self.queryPair = queryPair
        self.sequencePair = sequencePair
        self.queryIndex = queryIndex
        self.sequenceIndex = sequenceIndex
        self.queryNo = queryNo
        self.k_mer = k_mer
    
    def __str__(self):
        return  "Query Pair:    " + str(self.queryPair) + "\n" + "Sequence Pair: " + str(self.sequencePair)\
                + "\n" + "Score: " + str(self.score) + " Query Index: " + str(self.queryIndex) + " Sequence Index: " + str(self.sequenceIndex) + " Query No: " + str(self.queryNo)\
                + " K_Mer: " + str(self.k_mer)    

# Finds and returns matches for given checkWord in hashTable
def findMatches(hashTable, char, checkWord, queryIndex):
    hashRow = hashTable.get(char)
    list = []
    for item in hashRow:
        if item.k_mer == checkWord:
            newItem = hashItem(item.index, item.k_mer, queryIndex)
            list.append(newItem)
    return list

# Simple alignment ..... puts letter if match, puts '-' if mismatch
def simpleAlign(seq1, seq2):

    result = ""
    
    for i in range(0, len(seq1)):
        if(seq1[i] == seq2[i]):
            result += seq1[i]
        else:
            result += '-'
    return result
# Returns blank space with the length of given str
def getBlank(index):
    blank = ""
    for i in range(0, len(index)):
        blank += ' '
    return blank + '\t'

# Align and print for BLAST
def getAlignment(gruop):

    """ ..............SINCE IT IS NOT WANTED IN THE OUTPUT I COMMENTED OUT THE PRINT STATEMENTS IN ALIGNMENT FUNCTION,
        ONE CAN UNCOMMENT THEM TO SEE THE ALIGNMENT RESULTS.........."""
    #print("ALIGNMENTS")
    count = 1
    for group in groupList:

        blank = ""
        #print("------------------For Sequnece: " + str(count) + "------------------")
        count += 1
        for item in group:
            blank = getBlank(str(item.queryIndex))
            simAlign = simpleAlign(item.queryPair, item.sequencePair)
            """ 
            print("Query: " + str(item.queryIndex) + "\t" + item.queryPair + "\t" + str(item.queryIndex + len(item.queryPair) -1))
            print("       " + blank +  simAlign)
            print("Sbjct: " + str(item.sequenceIndex) + "\t" + item.sequencePair + "\t" + str(item.sequenceIndex + len(item.sequencePair) -1))
            print()
            print()"""

# Sorts HSSP list by sequence index
def insertionSort(list):
    for i in range(1, len(list)):

        key = list[i].sequenceIndex
        item = list[i]
        j = i - 1
        while j >= 0 and key < list[j].sequenceIndex :
            list[j + 1] = list[j]

            j -= 1
        list[j + 1] = item
# Main
if __name__ == "__main__":
    
    # ARGPARSE
    parser = argparse.ArgumentParser()
    parser.add_argument("--ref")
    parser.add_argument("--qry")
    parser.add_argument("--k")
    parser.add_argument("--s")
    
    args = parser.parse_args()
    referenceFileName = args.ref
    queryFileName = args.qry
    k = int(args.k)
    s = int(args.s)
    
    
    threshold = s
    genome = ""
    check = False

    # Read reference file
    with open(referenceFileName) as inFile:
        for line in inFile:
            if(check):
                genome = genome + line.strip()
            else:
                check = True

    # Read query file
    seq = ""
    check = False
    queries = []
    with open(queryFileName) as inFile:
        for line in inFile:
            if(line[0] == '>'):
                if(len(seq)):
                    queries.append(seq)
                seq = ""
            else:
                seq = seq + line.strip()
        queries.append(seq)

    # Create hash table
    hashTable = {'A': [], 'C': [], 'T': [], 'G': []}
    list = []
    for i in range(0, len(genome)-(k-1)):
        k_mer = genome[i:i+k]
        item = hashItem(i,k_mer, -1) # queryIndex is -1 since we will add this item to hash table not checking for query matces...
        
        if(k_mer[0] == 'A'):
            hashTable.get('A').append(item)
        elif(k_mer[0] == 'C'):
            hashTable.get('C').append(item)
        elif(k_mer[0] == 'T'):
            hashTable.get('T').append(item)
        elif(k_mer[0] == 'G'):
            hashTable.get('G').append(item)

    checkWord = ""
    matchesQuery = []

    # find all matches for k_mers in queries
    for query in queries:
        i = 0
        matches = []
        while i < len(query) - k +1:
            checkWord = query[i: i + k]
            list = findMatches(hashTable, checkWord[0], checkWord, i)
            matches.append(list)
            i = i + 1
        matchesQuery.append(matches)


    # print matches..
    """
    i = 1
    for matches in matchesQuery:
        print("-----------------------------------------Query: ", i, end = " -----------------------------------------\n")
        for list in matches:
            for item in list:
                print(item)
        i = i + 1
        print("--------------------------------------------------------------------------------------------")"""

    # SEED-AND-EXTEND
    qFinalIndex = -1
    sFinalIndex = -1
    count = 0

    queryPairs = []
    hspPairs = []

    addedIndexes = []
    for matches in matchesQuery:
        myQuery = queries[count]
        addedIndexes = []
        for list in matches:
            for item in list:
                qFinalIndex = item.queryIndex
                sFinalIndex = item.index

                checkLeft = True
                checkRight = True
                # Set pairs as k_mer at first
                sequencePair = item.k_mer
                queryPair = item.k_mer
                # Indexes that will go left
                sequenceIndexStart = item.index -1
                
                queryIndexStart = item.queryIndex-1      

                # Indexes that will go right
                queryIndexEnd = item.queryIndex + k 
                sequenceIndexEnd = item.index + k
                score = k   # At first score is k since there is k-matches  

                while(True):

                    # Check can extend to left or right
                    if(sequenceIndexStart < 0 or queryIndexStart < 0):
                        checkLeft = False
                    if(sequenceIndexEnd >= len(genome) or queryIndexEnd >= len(myQuery)):
                        checkRight = False
                    scoreTemp = score
                    sequencePairTemp = sequencePair
                    queryPairTemp = queryPair
                    
                    # Exten to left
                    if(checkLeft and genome[sequenceIndexStart] == myQuery[queryIndexStart]):
                        scoreTemp += 1
                        sequencePairTemp =   genome[sequenceIndexStart] + sequencePairTemp
                        queryPairTemp =  myQuery[queryIndexStart] + queryPairTemp

                    elif(checkLeft):
                        scoreTemp -= 1
                        sequencePairTemp =  genome[sequenceIndexStart] + sequencePairTemp
                        queryPairTemp =   myQuery[queryIndexStart] + queryPairTemp
                    
                    # Extend to right
                    if(checkRight and genome[sequenceIndexEnd] == myQuery[queryIndexEnd]):
                        scoreTemp += 1
                        sequencePairTemp =  sequencePairTemp + genome[sequenceIndexEnd]
                        queryPairTemp =   queryPairTemp + myQuery[queryIndexEnd]

                    elif(checkRight):
                        sequencePairTemp =  sequencePairTemp + genome[sequenceIndexEnd]
                        queryPairTemp =  queryPairTemp +  myQuery[queryIndexEnd]
                        scoreTemp -= 1

                    # Indexation
                    if(checkRight):    
                        sequenceIndexEnd += 1
                        queryIndexEnd += 1
                    if(checkLeft):
                        sequenceIndexStart -= 1
                        queryIndexStart -= 1

                    # If score not decreased then continue extending
                    if(scoreTemp >= score):     
                        sequencePair = sequencePairTemp
                        queryPair = queryPairTemp
                        score = scoreTemp
                        qFinalIndex = queryIndexStart + 1
                        sFinalIndex = sequenceIndexStart + 1
                    else:   # decreased... so break
                        # ADD HSPP pair
                        if(score >= threshold and (sFinalIndex not in addedIndexes)):
                            #addedIndexes.append(sFinalIndex)
                            hspPairs.append(HSPItem(score, queryPair, sequencePair, qFinalIndex, sFinalIndex, count, item.k_mer))
   
                        break
                    # cannot extend more.... break
                    if(checkLeft == False and checkRight == False):
                        # ADD HSPP pair if it meets the threshold
                        if(score >= threshold and (sFinalIndex not in addedIndexes)):
                            #addedIndexes.append(sFinalIndex)
                            hspPairs.append(HSPItem(score, queryPair, sequencePair, qFinalIndex, sFinalIndex, count, item.k_mer))
                        break
        count += 1

    """
    for item in hspPairs:
        print(item)                                                                                      
        print("--------------------------------------------------------------------------------------------")  """

    # Group HSSP by queryNo
    count = 0
    output = []
    groupList = []
    for count in range(0,len(queries)):
        this_group = []
        for item in hspPairs:
            if(item.queryNo == count):
                this_group.append(item)
        groupList.append(this_group)
    
    # sort every group with respect to sequence index
    for group in groupList:
        #print("***************NEXT GROUP*********************")   
        insertionSort(group)
        #for elem in group:
        #   print(elem)


    # Align
    getAlignment(group)

    # Print starting locations...
    output = []
    outTxt = ""
    count = 1
    for group in groupList:
        if(len(group)):
            outTxt = "> Seq" + str(count) + ": " + str(group[0].sequenceIndex)
            count += 1
        else:
            outTxt = "> Seq" + str(count) + ": -"
            count += 1
        output.append(outTxt)

    print()
    for row in output:
        print(row)