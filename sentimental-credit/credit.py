from cs50 import get_string

sum = 0
number = get_string("Number: ")
isValid = False
isFirst = True

if len(number) % 2 == 0:
    isFirst = True
else:
    isFirst = False

for i in range(0, len(number)):
    if isFirst:
        product = int(number[i]) * 2
        if product > 9:
            sum += int(str(product)[0])
            sum += int(str(product)[1])
        else:
            sum += product
        isFirst = False
    else:
        sum += int(number[i])
        isFirst = True

if sum % 10 == 0:
    isValid = True

if isValid:
    lastTwoD = number[0] + number[1]
    if (lastTwoD == "34" or lastTwoD == "37") and len(number) == 15:
        print("AMEX")
    elif (lastTwoD == "51" or lastTwoD == "52" or lastTwoD == "53" or lastTwoD == "54" or lastTwoD == "55") and len(number) == 16:
        print("MASTERCARD")
    elif lastTwoD[0] == "4" and (len(number) == 13 or len(number) == 16):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
