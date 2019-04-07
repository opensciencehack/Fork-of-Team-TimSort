from nltk.stem import PorterStemmer

words = ["gaming", "gamed", "games"]

ps = PorterStemmer()

for word in words:
    print(ps.stem(word))