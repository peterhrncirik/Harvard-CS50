# Import libraries
import cs50, csv, sys

# Check input
if len(sys.argv) != 2:
    print("You are missing arguments.")

# Create DB
open("studens.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# Open File
with open(sys.argv[1]) as f:
    reader = csv.reader(f)
    # Skip the header
    next(reader)

    # Loop through each entry, check names
    name = ""
    for row in reader:
        name = row[0].split()
        if len(name) == 2:
            name.insert(1, None)

        # Insert into DB
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name[0], name[1], name[2], row[1], row[2])
