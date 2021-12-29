# Import necessary libraries
import sys
import cs50


# Define main function.
def main():
    cc = getccn()
    validate(cc)


# Repeat getting input as string until valid
def getccn():
    while True:
        num = cs50.get_string("cc num?: ")
        try:
            if len(num) > 0:
                break
        except ValueError:
            continue
    return num


# Pass input credit card number to validation function.
def validate(cc):
    if (len(cc) < 13) or (len(cc) > 16):
        print("INVALID")
        sys.exit(0)

    card_len = len(cc)
    even, odd = 0, 0
# If even:
    if card_len % 2 == 0:
        for i in range(card_len):
            num = int(cc[i])
            if i % 2 == 0:
                multiple = num * 2
                if multiple >= 10:
                    even += multiple // 10
                    even += multiple % 10
                else:
                    even += multiple
# If odd:
            else:
                odd += num
    else:
        for i in range(card_len):
            num = int(cc[i])
            if i % 2 != 0:
                multiple = num * 2
                if multiple >= 10:
                    even += multiple // 10
                    even += multiple % 10
                else:
                    even += multiple
            else:
                odd += num

# Assign checksum and check if exact multiple of 10
    checksum = (even + odd) % 10

# Check the conditions for card types
    if checksum == 0:
        first_digit = int(cc[0])
        second_digit = int(cc[1])
        if first_digit == 3 and second_digit == 4 or second_digit == 7:
            print("AMEX")
        elif first_digit == 5 and 1 <= second_digit <= 5:
            print("MASTERCARD")
        elif first_digit == 4:
            print("VISA")
        else:
            print("INVALID")


# Recall main function:
if __name__ == "__main__":
    main()

