from cs50 import get_string

# Ask for input
text = get_string("Text: ")

# Initialize counters (words at 1, because last word is never counted)
letters = 0
words = 1
sentences = 0

# Loop through text & check each character
for letter in text:
    if letter.isalpha():
        letters += 1
    elif letter.isspace():
        words += 1
    elif letter == "." or letter == "!" or letter == "?":
        sentences += 1

# Count the grade
grade = 0.0588 * (letters * 100 / words) - 0.296 * (sentences * 100 / words) - 15.8

# Decide on the level
if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {round(grade)}")
