lws = [0, 0, 0]


def main():
    text = input("Text: ")
    countLWS(text)

    index = 0.0588 * ((lws[0]/lws[1]) * 100) - 0.296 * ((lws[2]/lws[1]) * 100) - 15.8

    grade = int(round(index))

    if grade < 0:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def countLWS(text):
    for i in text:
        if i.lower() >= 'a' and i.lower() <= 'z':
            lws[0] += 1
        elif i == '.' or i == '!' or i == '?':
            lws[2] += 1
        elif i == ' ':
            lws[1] += 1
    lws[1] += 1


main()
