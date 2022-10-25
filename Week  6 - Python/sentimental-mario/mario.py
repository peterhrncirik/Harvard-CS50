from cs50 import get_int

height = get_int("Height?\n")

while height < 1 or height > 8:
    height = get_int("Height?\n")

for i in range(height):
    for k in range(height - 1, i, -1):
        print(" ", end="")
    for j in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for n in range(i + 1):
        print("#", end="")
    print("")
