# Import library
import sys, cs50

# Check args
if len(sys.argv) != 2:
    print("You are missing arguments")

# Get the house
house = sys.argv[1]

# Create DB
open("studens.db", "r").close()
db = cs50.SQL("sqlite:///students.db")

# Query data from DB
values = db.execute("SELECT first, middle, last, house, birth FROM students WHERE house = (?) ORDER BY last, first", house)

# Loop through data
for person in values:

    if person['middle'] == None:
        print(f"{person['first']} {person['last']}, born {person['birth']}")
    else:
        print(f"{person['first']} {person['middle']} {person['last']}, born {person['birth']}")
