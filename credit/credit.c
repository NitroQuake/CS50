#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number;
    long numberCopy;
    int counter = 0;
    int sum = 0;
    bool isValid = false;

    do
    {
        number = get_long("Number: ");
    } while(number <= 0);

    numberCopy = number;
    while(numberCopy != 0)
    {
        numberCopy /= 10;
        counter++;
    }


    for(int i = 0; i < counter; i++)
    {
        long tens = 1;
        for(int j = 0; j < i + 1; j++) {
            tens *= 10;
        }

        if(i % 2 != 0) {
            int product = number % tens / (tens / 10) * 2;
            if(product > 9) {
                sum += product % 10;
                sum += product / 10;
            } else {
                sum += product;
            }
        } else {
            sum += number % tens / (tens / 10);
        }
    }

    if(sum % 10 == 0) {
        isValid = true;
    }

    if(isValid) {
        long tens = 1;
        for(int i = 0; i < counter - 2; i++) {
            tens *= 10;
        }
        if((number / tens == 34 || number / tens == 37) && counter == 15) {
            printf("AMEX\n");
        } else if((number / tens == 51 || number / tens == 52 || number / tens == 53 || number / tens == 54 || number / tens == 55) && counter == 16) {
            printf("MASTERCARD\n");
        } else if(number / (tens * 10) == 4 && (counter == 13 || counter == 16)) {
            printf("VISA\n");
        } else {
            printf("INVALID\n");
        }
    } else {
        printf("INVALID\n");
    }
}
