from cs50 import get_string

# Function that sums individual digits
def sum_digits(n):
    s = 0
    while n:
        s += n % 10
        n //= 10
    return s

# Calculate even nums
def return_even(n):
    digit = 0
    for i in n[::-1][0::2]:
        digit += int(i)
    return digit
    
# Calculate odd & multiply
def return_odd(n):
    digit = 0
    digits = []
    for i in n[::-1][1::2]:
        digits.append(sum_digits(int(i) * 2))
    
    for num in digits:
        digit += num
    return digit
    
# Card is Okay, check the type
def check_card(card):
    if len(card) == 15 and card[0] == "3":
        print("AMEX")
    elif len(card) == 16 and card[0] == "5":
        print("MASTERCARD")
    elif (len(card) <= 16 and len(card) >= 13) and card[0] == "4":
        print("VISA")
    else:
        print("INVALID")
    
# Prompt for input
card = get_string("Number: ")

if len(card) < 13 or len(card) > 16:
    print("INVALID")

# Check if card is good, else exit
if((return_even(card) + return_odd(card)) % 10 == 0):
    check_card(card)
