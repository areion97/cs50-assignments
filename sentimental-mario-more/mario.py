
from cs50 import get_int


def print_spaces(num):
    for i in range(num):
        print(" ", end='')


def print_hashtag(num):
    for i in range(num):
        print("#", end='')


n = 0
while n < 1 or n > 8:
    n = get_int("Height: ")

for i in range(n):
    print_spaces(n - i - 1)
    print_hashtag(i + 1)
    print("  ", end='')
    print_hashtag(i + 1)
    print("")

