from nltk.corpus import wordnet as wn


def findSys(word):

    lemmas =[]

    for syn in wn.synsets(word):

        tmp = wn.synsets(word)[0].pos()

        if tmp == "n":
            specword = wn.synset(word + "." + str(tmp) + ".01")
            for l in syn.hyponyms():
                similar = l.wup_similarity(specword)
                if similar > 0.4:
                    lemmas.append(l.name())
    """""
        if tmp == "v":
            specword = wn.synset(word + "." + str(tmp) + ".01")

            similar = syn.wup_similarity(specword)
            if similar > 0.6:
                lemmas.append(syn.name())
    """
    return set(lemmas)



if __name__ == '__main__':
    print(findSys("dog"))