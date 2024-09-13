'''
Filename:  tokenExample.py
To run: python3 tokenExample.py <infile>
e.g., python3 tokenExample.py input.txt
'''

#Fun fact:
#This file cannot be titled "token.py"
#This is because one of the library files is named this,
#which messes with the compilation and linking of the file!

from typing import Counter
import spacy
import sys
from spacy.matcher import Matcher
import os

# Define the common function to handle token matching and annotation
def process_match(matcher, doc, id, matches, output):
    token_index = matches[id][1]
    #we define a custom attribute to determine whether or not the token has been matched
    #this creates a "psuedo-priority" system, where each token can only match to one rule
    if not doc[token_index]._.get("matched"):
        doc[token_index]._.set("matched", True)

def html_match(matcher, doc, id, matches):
    process_match(matcher, doc, id, matches, "")

def defaultMatch(matcher, doc, id, matches):
    token_index = matches[id][1]
    if not doc[token_index]._.get("matched"):
        word = doc[token_index].text
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
    [{"TEXT": {"REGEX": "\S+"}}],                     # Any other NON-WHITESPACE token
]

matcher.add("default", [patterns[6]], on_match=defaultMatch)

file_path = sys.argv[1]
output_directory = sys.argv[2]
file_name = os.path.basename(file_path)
newFileName = file_name.replace(".html", ".out")
output_file_path = os.path.join(output_directory, newFileName)


with open(file_path, "r") as file:
    text = file.read()

doc = nlp(text)

with open(output_file_path, "w") as out_file:
    # Write sorted tokens and frequencies to the file
    for token in doc:
        out_file.write(token.text + '\n')

# Process the matches
matches = matcher(doc)

