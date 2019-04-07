import json
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from nltk.stem import PorterStemmer
import operator

stop_words = set(stopwords.words('english'))

def parseInputFile():
    # Open JSON file
    with open('captions_val2017.json', "r") as jsonFile:
        data = json.load(jsonFile)

    # Create PorterStemmer
    ps = PorterStemmer()

    i = 0
    for ann in data['annotations']:
        # Get caption
        tmp = ann['caption']
        # To lowercase
        tmp = tmp.lower()
        # Tokenize
        word_tokens = word_tokenize(tmp)
        # Filter stopwords
        filtered_sentence = [w for w in word_tokens if not w in stop_words]

        # Keep only stem
        for i in range(len(filtered_sentence)):
            filtered_sentence[i] = ps.stem(filtered_sentence[i])

        # Join to create one string
        filtered_sentence = ' '.join(filtered_sentence)
        # Remove '.'
        filtered_sentence = filtered_sentence.replace(".", "")
        # Remove ','
        filtered_sentence = filtered_sentence.replace(",", "")

        #print(filtered_sentence)
        data['annotations'][i]['caption'] = filtered_sentence
        i += 1

    with open("captions_val2017.json", "w") as jsonFile:
        json.dump(data, jsonFile)

# Create output.json file
# contains raw hashmap
def getTopics():
    # Open JSON file
    with open('captions_val2017.json', "r") as jsonFile:
        data = json.load(jsonFile)

    # 'V' verbs
    # 'N' nouns
    # 'A' adjectives
    # 'R' adverbs

    map = {'V': [], 'N': [], 'J': [], 'R': []}

    cpt = 0
    size = len(data['annotations'])

    for ann in data['annotations']:
        print((100*cpt)/(size))
        cpt += 1
        capt = ann['caption'].split()
        id = ann['image_id']
        for i in range(len(capt)):
            tokenized = word_tokenize(capt[i])
            tagged = nltk.pos_tag(tokenized)

            if(tagged[0][1][0] == "V"):
                map['V'].append(tagged[0][0])
                map['V'].append(id)

            if (tagged[0][1][0] == "N"):
                map['N'].append(tagged[0][0])
                map['N'].append(id)
            if (tagged[0][1][0] == "J"):
                map['J'].append(tagged[0][0])
                map['J'].append(id)
            if (tagged[0][1][0] == "R"):
                map['R'].append(tagged[0][0])
                map['R'].append(id)
    with open("output.json", "w") as jsonFileOut:
        json.dump(map, jsonFileOut)


# Create result.json file
# Contains hashmap
#          key : [word]
#          value : [occurrence, rank]
def createIndex():
    # Open JSON file
    with open('output.json', "r") as jsonFile:
        data = json.load(jsonFile)

    dictR = {}
    dictV = {}
    dictN = {}
    dictJ = {}

    for r in data['R']:
        if(r in dictR):
            dictR[r] = dictR[r] + 1
        else:
            dictR[r] = 1

    for v in data['V']:
        if(v in dictV):
            dictV[v] = dictV[v] + 1
        else:
            dictV[v] = 1

    for n in data['N']:
        if(n in dictN):
            dictN[n] = dictN[n] + 1
        else:
            dictN[n] = 1

    for j in data['J']:
        if(j in dictJ):
            dictJ[j] = dictJ[j] + 1
        else:
            dictJ[j] = 1

    #print(dictR)
    #print(dictV)
    #print(dictJ)
    #print(dictN)

    sorted_xR = sorted(dictR.items(), key=operator.itemgetter(1))
    sorted_xV = sorted(dictV.items(), key=operator.itemgetter(1))
    sorted_xJ = sorted(dictJ.items(), key=operator.itemgetter(1))
    sorted_xN = sorted(dictN.items(), key=operator.itemgetter(1))

    iR = len(sorted_xR)-1
    iV = len(sorted_xV) - 1
    iJ = len(sorted_xJ) - 1
    iN = len(sorted_xN) - 1

    rank = 0
    resultDictR = {}
    resultDictV = {}
    resultDictJ = {}
    resultDictN = {}
    while iR >= 0:
        key = sorted_xR[iR][0]
        resultDictR[key] = []
        resultDictR[key].append(sorted_xR[iR][1])
        if(iR == len(sorted_xR)-1):
            print("first")
            resultDictR[key].append(rank)
            rank+=1
        else:
            if (sorted_xR[iR][1] == sorted_xR[iR-1][1]):
                resultDictR[key].append(rank)
            else:
                resultDictR[key].append(rank)
                rank += 1
        iR-=1

    rank = 0
    while iV >= 0:
        key = sorted_xV[iV][0]
        resultDictV[key] = []
        resultDictV[key].append(sorted_xV[iV][1])
        if(iV == len(sorted_xV)-1):
            print("first")
            resultDictV[key].append(rank)
            rank+=1
        else:
            if (sorted_xV[iV][1] == sorted_xV[iV-1][1]):
                resultDictV[key].append(rank)
            else:
                resultDictV[key].append(rank)
                rank += 1
        iV -= 1
    rank = 0

    while iJ >= 0:
        key = sorted_xJ[iJ][0]
        resultDictJ[key] = []
        resultDictJ[key].append(sorted_xJ[iJ][1])
        if(iJ == len(sorted_xJ)-1):
            resultDictJ[key].append(rank)
            rank+=1
        else:
            if (sorted_xJ[iJ][1] == sorted_xJ[iJ-1][1]):
                resultDictJ[key].append(rank)
            else:
                resultDictJ[key].append(rank)
                rank += 1
        iJ -= 1
    rank = 0

    while iN >= 0:
        key = sorted_xN[iN][0]
        resultDictN[key] = []
        resultDictN[key].append(sorted_xN[iN][1])
        if(iN == len(sorted_xN)-1):
            print("first")
            resultDictN[key].append(rank)
            rank+=1
        else:
            if (sorted_xN[iN][1] == sorted_xN[iN-1][1]):
                resultDictN[key].append(rank)
            else:
                resultDictN[key].append(rank)
                rank += 1
        iN -= 1

    resultDict = {}
    resultDict['V'] = resultDictV
    resultDict['R'] = resultDictR
    resultDict['J'] = resultDictJ
    resultDict['N'] = resultDictN

    with open("result.json", "w") as jsonFileOut:
        json.dump(resultDict, jsonFileOut)


if __name__ == '__main__':
    parseInputFile()
    getTopics()
    createIndex()