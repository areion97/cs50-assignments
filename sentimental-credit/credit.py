import re
from cs50 import get_string

def luhn(account_number):


def main():
    account_number = get_string("Number: ")

    if luhn(account_numer):
        if re.match('^3[47][0-9]{13}$', account_number):
            print("AMEX")
            return
        if re.match('^(5[1-5][0-9]{14}|2(22[1-9][0-9]{12}|2[3-9][0-9]{13}|[3-6][0-9]{14}|7[0-1][0-9]{13}|720[0-9]{12}))$', account_number):
            print("MASTERCARD")
            return
        if re.match('^4[0-9]{12}(?:[0-9]{3})?$', account_number):
            print("VISA")
            return

    print("INVALID")


main()