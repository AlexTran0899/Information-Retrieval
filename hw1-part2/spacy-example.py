'''
Filename:  tokenExample.py
To run: python3 tokenExample.py <infile>
e.g., python3 tokenExample.py input.txt
'''

#Fun fact:
#This file cannot be titled "token.py"
#This is because one of the library files is named this,
#which messes with the compilation and linking of the file!

import spacy
import sys
from spacy.matcher import Matcher

# Define the common function to handle token matching and annotation
def process_match(matcher, doc, id, matches, output):
    token_index = matches[id][1]
    #we define a custom attribute to determine whether or not the token has been matched
    #this creates a "psuedo-priority" system, where each token can only match to one rule
    if not doc[token_index]._.get("matched"):
        sys.stdout.write(output)
        doc[token_index]._.set("matched", True)

def himHisMatch(matcher, doc, id, matches):
    process_match(matcher, doc, id, matches, "her")

def heMatch(matcher, doc, id, matches):
    process_match(matcher, doc, id, matches, "she")

def susanMatch(matcher, doc, id, matches):
    token_index = matches[id][1]
    if not doc[token_index]._.get("matched"):
        sys.stdout.write(doc[token_index].text[0])
        doc[token_index]._.set("matched", True)

def upperMatch(matcher, doc, id, matches):
    token_index = matches[id][1]
    if not doc[token_index]._.get("matched"):
        Ch = doc[token_index].text.lower()
        sys.stdout.write(Ch)
        doc[token_index]._.set("matched", True)

def digitMatch(matcher, doc, id, matches):
    process_match(matcher, doc, id, matches, "NUMBER")

def defaultMatch(matcher, doc, id, matches):
    token_index = matches[id][1]
    if not doc[token_index]._.get("matched"):
        word = doc[token_index].text
        sys.stdout.write(word)
        doc[token_index]._.set("matched", True)

nlp = spacy.load("en_core_web_sm")
matcher = Matcher(nlp.vocab)

# Add a custom extension to tokens to track if they have been matched
spacy.tokens.Token.set_extension("matched", default=False)

# Prioritize patterns based on specificity
patterns = [
    [{"TEXT": "Susan"}],                              # Specific name match first
    [{"TEXT": {"REGEX": r"[hH]im"}}],                 # Pronoun match
    [{"TEXT": {"REGEX": r"[hH]is"}}],                 # Possessive pronoun match
    [{"TEXT": {"REGEX": r"[hH]e"}}],                  # Pronoun match
    [{"TEXT": {"REGEX": r"[0-9]+"}}],                 # Number match
    [{"TEXT": {"REGEX": r"[A-Z]"}}],                  # Uppercase match
    [{"TEXT": {"REGEX": "\S+"}}]                      # Any other NON-WHITESPACE token
]

matcher.add("susan", [patterns[0]], on_match=susanMatch)
matcher.add("himHis", [patterns[1], patterns[2]], on_match=himHisMatch)
matcher.add("he", [patterns[3]], on_match=heMatch)
matcher.add("digit", [patterns[4]], on_match=digitMatch)
matcher.add("upper", [patterns[5]], on_match=upperMatch)
matcher.add("default", [patterns[6]], on_match=defaultMatch)

inputFile = sys.argv[1]

with open(inputFile, "r") as file:
    text = file.read()

doc = nlp(text)

# Process the matches
matches = matcher(doc)