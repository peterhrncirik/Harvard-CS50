import sys, csv, re

# Check arguments
if len(sys.argv) != 3:
    print("Looks like you are missing some arguments.")

# Dictionary to store STRs counts
str_count = {}

# Open CSV Dictionary, get the header
with open(sys.argv[1]) as f:
    reader = csv.reader(f)
    STR_header = next(reader)

# Open STR sequence
f_seq = open(sys.argv[2])
s_seq = f_seq.read()

# Find longest STR sequence, starting from 1 (excluding name)
for header in STR_header[1::]:
    groups = re.findall(r'(?:' + header + ')+', s_seq)
    if not groups:
        break
    longest = max(groups, key=len)
    # Push longest sequence into STR dictionary, divided by its length
    str_count[header] = int(len(longest) / len(header))

# Open Database
f_dict = open(sys.argv[1])
s_dict = csv.DictReader(f_dict)

# Function to compare result and return name of the match
def get_name(result, db):
    names = []
    for row in db:
        name = row['name']
        row.pop('name')
        row = {key: int(value) for key, value in row.items()}
        if row == result:
            names.append(name)
    return names

# Store result as a match
match = get_name(str_count, s_dict)

# Final logic
if not match:
    print("No match")
else:
    for name in match:
        print(name)
