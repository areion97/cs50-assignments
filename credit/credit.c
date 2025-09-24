#include <cs50.h>
#include <stdio.h>
#include <math.h>

int printCardType(long accountNumber, int length);
int isValidCard(long accountNumber, int length);
int getFirstDigit(long number);
int getDigit(long number, int index, int length);
int sumDigits(int digit);


int main(void)
{
    long accountNumber = 0;
    int length = 0;

    //Get input
    do
    {
        accountNumber = get_long("Number: ");
    }
    while (accountNumber == 0);
    long tmp = accountNumber;

    // Calculate account number length
    while (tmp >= 1)
    {
        length ++;
        tmp = tmp / 10;
    }

    // Validate Card
    if (!isValidCard(accountNumber, length))
    {
        printf("INVALID\n");
        return 0;
    }

    // Prints card type
    printCardType(accountNumber, length);

    return 0;

}


/* All American Express numbers start with 34 or 37;
   most MasterCard numbers start with 51, 52, 53, 54, or 55
   and all Visa numbers start with 4.
   American Express uses 15-digit numbers,
   MasterCard uses 16-digit numbers, and Visa uses 13- and 16-digit numbers.
*/

int printCardType(long accountNumber, int length)
{
    if (length == 15 && getDigit(accountNumber, 0, length) == 3 && (getDigit(accountNumber, 1, length) == 4
			|| getDigit(accountNumber, 1, length) == 7))
	{
		printf("AMEX\n");
		return 1;
	}
	if (length < 17 && length > 12 && getDigit(accountNumber, 0, length) == 4)
	{
		printf("VISA\n");
		return 1;
	}
	if (length == 16 && getDigit(accountNumber, 0, length) == 5 &&
            (
            getDigit(accountNumber, 1, length) == 1 ||
	        getDigit(accountNumber, 1, length) == 2 ||
            getDigit(accountNumber, 1, length) == 3 ||
		    getDigit(accountNumber, 1, length) == 4 ||
		    getDigit(accountNumber, 1, length) == 5
		    )
		)
	{
		printf("MASTERCARD\n");
		return 1;
	}

	printf("INVALID\n");
	return 0;
}

int isValidCard(long accountNumber, int length)
{
	int odd = 0;
	int sum = 0;
    int partial = 0;
	if (length % 2 != 0)
	{
		odd = 1;
	}

	for (int i = 0; i < length; i++)
	{
		int digit = getDigit(accountNumber, i, length);
		if (i % 2 == 0)
		{
			if (odd == 0)
			{
                partial += sumDigits(digit * 2);
			}
			else
			{
				sum += digit;
			}
		}
		else
		{
			if (odd == 0)
			{
                sum += digit;

			}
			else
			{
				partial += sumDigits(digit * 2);
			}

		}

	}
    sum += partial;

	if (sum % 10 == 0)
	{
	    return 1;
	}

	return 0;

}

int getFirstDigit(long number)
{
    while (number >= 10)
    {
        number = number / 10;
    }

    return number;
}

int sumDigits(int digit)
{
	if (digit < 10)
	{
		return digit;
	}
    else
    {
        return getDigit(digit, 0, 2) + getDigit(digit, 1, 2);
    }
    return 0;
}

int getDigit(long number, int index, int length)
{
    long resto = -1;

    resto = number % (long)pow(10, length - index);


    if (resto < pow(10, length - index - 1))
    {
        return 0;
    }
    return getFirstDigit(resto);

}
