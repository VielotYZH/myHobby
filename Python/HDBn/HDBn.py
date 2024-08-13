# n代表-1

import os

N = 3

def decode(coding):
    result = []
    last = 1
    for i in range(len(coding)):
        if coding[i] == 'n':
            result.append('1')
            if last == -1:
                for j in range(i, i - N - 1, -1):
                    result[j] = '0'
            else:
                last = -last
        elif coding[i] == '1':
            result.append('1')
            if last == 1:
                for j in range(i, i - N -1, -1):
                    result[j] = '0'
            else:
                last = -last
        elif coding[i] == '0':
            result.append('0')
    return ''.join(result)

def encode(source):
    result = []
    # 第一个“1”被编码为“-1”
    # 第一个“V”被编码为“-V”
    counter, last, lastV = 0, 1, 1
    for i in range(len(source)):
        if source[i] == '1':
            counter = 0
            result.append('n' if last == 1 else '1')
            last = -last
        elif source[i] == '0':
            counter += 1
            result.append('0')
            if counter == N + 1:
                counter = 0
                result[i] = 'n' if lastV == 1 else '1'
                lastV = -lastV
                if result[i - N - 1] != result[i]:
                    result[i - N] = result[i]
    return ''.join(result)

def main():
    if os.path.exists("Python/HDBn/test.txt"):
        with open(r"Python\HDBn\test.txt", "r") as f:
            source = f.readline()
            coding = encode(source)
            decoding = decode(coding)
            print(f"source:\t\t{source}")
            print(f"coding:\t\t{coding}")
            print(f"decoding:\t{decoding}")
    else:
        print("file is not exist")

if __name__ == "__main__":
    main()