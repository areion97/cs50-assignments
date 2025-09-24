from cs50 import get_float

while True:
    input = get_float("Change owed: ")
    if input != "" and input >= 0:
        resto = input
        count = 0
        while resto != 0:
            if resto >= 0.25:
                resto = round(resto - 0.25, 2)

            elif resto >= 0.10:
                resto = round(resto - 0.10, 2)

            elif resto >= 0.05:
                resto = round(resto - 0.05, 2)

            elif resto >= 0.01:
                resto = round(resto - 0.01, 2)
            count = count + 1
        break

print(count)
