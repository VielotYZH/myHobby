length, number = 0, 0

def decimalToBinary(decimal):
    binary = ['0' for _ in range(length)]
    index = length - 1
    while decimal > 1:
        quotient = int(decimal / 2)
        binary[index] = '1' if decimal - quotient * 2 == 1 else '0'
        index -= 1
        decimal = quotient
    binary[index] = '1' if decimal == 1 else '0'
    return binary

def singleOperate(digital_number, gray):
    if digital_number == 1:
        gray[1][length - 1] = '1'
    else:
        r = 2 ** digital_number
        for i in range(int(r / 2)):
            for j in range(length - 1, length - digital_number - 1, -1):
                gray[r - 1 -i][j] = gray[i][j]
                gray[r - 1- i][length - digital_number] = '1'

def encode(gray):
    for i in range(1, length + 1, 1):
        singleOperate(i, gray)

def init():
    global length, number
    length = int(input("Please enter the length of the gray code you want:"))
    number = 2 ** length
    return [['0' for _ in range(length)] for _ in range(number)]

def main():
    gray = init()
    encode(gray)
    print("Number\tGray\tBinary")
    for i in range(number):
        print(f"{i}\t{''.join(gray[i])}\t{''.join(decimalToBinary(i))}")

if __name__ == "__main__":
    main()