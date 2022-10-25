from cs50 import get_float

amount = get_float("Change owed: ")

while amount <= 0:
    amount = get_float("Change owed: ")
 
change = round(amount * 100)
coins = 0

while change - 25 >= 0:
    change -= 25
    coins += 1
    
while change - 10 >= 0:
    change -= 10
    coins += 1

while change - 5 >= 0:
    change -= 5
    coins += 1
    
while change - 1 >= 0:
    change -= 1
    coins += 1
    

print(f"{coins}")
