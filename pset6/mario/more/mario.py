import cs50


# Define Main function to get the input and print hastags.
def main():
    height = get_height()
    for i in range(height):
        print(" "*(height-i-1) + "#"*(i+1) + "  " + "#"*(i+1))


# Define get_height to get the height in integer form
def get_height():
    while True:
        n = cs50.get_int("Height: ")
        if ((n > 0) and (n <= 8)):
            break
    return n


main()