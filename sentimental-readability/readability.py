from cs50 import get_string


def main():

    text = get_string("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    roundedIndex = int(round(index))

    if roundedIndex < 1:
        print("Before Grade 1")
        return 0
    elif roundedIndex >= 16:
        print("Grade 16+")
        return 0

    print(f"Grade {roundedIndex}")
    return 0


def count_letters(text):
    counter = 0
    for i in range(len(text)):
        letter = text[i].lower()
        if ord(letter) - 97 >= 0 and ord(letter) - 97 <= 25:
            counter += 1

    return counter


def count_words(text):
    counter = 0
    for i in range(len(text)):
        letter = text[i].lower()
        if ord(letter) - 32 == 0:
            counter += 1

    return counter + 1


def count_sentences(text):
    counter = 0
    for i in range(len(text)):
        letter = text[i].lower()
        if ord(letter) - 46 == 0 or ord(letter) - 33 == 0 or ord(letter) - 63 == 0:
            counter += 1

    return counter


main()