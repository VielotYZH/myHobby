import random

MAX_PAGE_TYPES = 5
PAGE_FRAMES_NUMBER = 3
PAGE_NUMBER = 10000

def randomlyGeneratePages(arr):
    for i in range(len(arr)):
        arr[i] = random.randint(0, MAX_PAGE_TYPES - 1) + 65

def contains(arr, start, end, element):
    for i in range(start, end):
        if arr[i] == element:
            return i
    return -1

def printPageFrames(arr):
    for e in arr:
        if type(e) is str:
            print(f"{e}\t\t", end="")
        else:
            print(f"{chr(e)}\t\t", end="")

def OPT(pages, currentIndex, pageFrames):
    if contains(pageFrames, 0, PAGE_FRAMES_NUMBER, pages[currentIndex]) == -1:
        availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0)
        if availableIndex != -1:
            pageFrames[availableIndex] = pages[currentIndex]
        else:
            targetIndex, maxIndex = 0, 0
            for i in range(PAGE_FRAMES_NUMBER):
                tempIndex = contains(pages, currentIndex + 1, len(pages), pageFrames[i])
                if tempIndex == -1:
                    targetIndex = i
                    break
                elif tempIndex > maxIndex:
                    maxIndex = tempIndex
                    targetIndex = i
            pageFrames[targetIndex] = pages[currentIndex]
        return True
    return False

def FIFO():
    times = [-1 for _ in range(PAGE_FRAMES_NUMBER)]
    def fifo(page, pageFrames):
        missingPage = False
        if contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page) == -1:
            missingPage = True
            availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0)
            if availableIndex != -1:
                pageFrames[availableIndex] = page
                times[availableIndex] = 0
            else:
                targetIndex = 0
                for i in range(1, PAGE_FRAMES_NUMBER):
                    if times[i] > times[targetIndex]:
                        targetIndex = i
                pageFrames[targetIndex] = page
                times[targetIndex] = 0
        for i in range(len(times)):
            if times[i] != -1:
                times[i] += 1
        return missingPage
    return fifo

def LRU():
    times = [-1 for _ in range(PAGE_FRAMES_NUMBER)]
    def lru(page, pageFrames):
        missingPage = False
        index = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page)
        if index == -1:
            missingPage = True
            availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0)
            if availableIndex != -1:
                pageFrames[availableIndex] = page
                times[availableIndex] = 0
            else:
                targetIndex = 0
                for i in range(1, PAGE_FRAMES_NUMBER):
                    if times[i] > times[targetIndex]:
                        targetIndex = i
                pageFrames[targetIndex] = page
                times[targetIndex] = 0
        else:
            times[index] = 0
        for i in range(len(times)):
            if times[i] != -1:
                times[i] += 1
        return missingPage
    return lru

def LFU():
    times = [-1 for _ in range(PAGE_FRAMES_NUMBER)]
    def lfu(page, pageFrames):
        index = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page)
        if index == -1:
            availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0)
            if availableIndex != -1:
                pageFrames[availableIndex] = page
                times[availableIndex] += 1
            else:
                targetIndex = 0
                for i in range(PAGE_FRAMES_NUMBER):
                    if times[i] < times[targetIndex]:
                        targetIndex = i
                pageFrames[targetIndex] = page
                times[:] = [0] * PAGE_FRAMES_NUMBER
            return True
        else:
            times[index] += 1
            return False
    return lfu

def runOPT(p, printProcedure, pages):
    missingPageNumber = 0
    pageFrames = [0 for _ in range(PAGE_FRAMES_NUMBER)]
    if printProcedure:
        for i in range(PAGE_FRAMES_NUMBER):
            print(f"pageFramed{i + 1}\t", end="")
        print("pageMissing")
    for i in range(len(pages)):
        missingPage = p(pages, i, pageFrames)
        if missingPage:
            missingPageNumber += 1
        if printProcedure:
            printPageFrames(pageFrames)
            if missingPage:
                print("*", end="")
            print()
    print(f"OPT missing page number:{missingPageNumber}")
    print(f"OPT page fault rate:{round(missingPageNumber / len(pages) * 100, 2)}%")
    print("\n")
    return missingPageNumber

def runFunction(p, functionName, printProcedure, pages):
    missingPageNumber = 0
    pageFrames = [0 for _ in range(PAGE_FRAMES_NUMBER)]
    if printProcedure:
        for i in range(PAGE_FRAMES_NUMBER):
            print(f"pageFramed{i + 1}\t", end="")
        print("pageMissing")
    f = p()
    for page in pages:
        missingPage = f(page, pageFrames)
        if missingPage:
            missingPageNumber += 1
        if printProcedure:
            printPageFrames(pageFrames)
            if missingPage:
                print("*", end="")
            print()
    print(f"{functionName} missing page number:{missingPageNumber}")
    print(f"{functionName} page fault rate:{round(missingPageNumber / len(pages) * 100, 2)}%")
    print("\n")
    return missingPageNumber

def testOPT(p):
    arr = ['A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E']
    runOPT(p, True, arr)

def test(p, functionName):
    arr = ['A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E']
    runFunction(p, functionName, True, arr)

def main():
    #testOPT(OPT)
    #test(FIFO, "FIFO")
    #test(LRU, "LRU")
    #test(LFU, "LFU")
    pages = [0] * PAGE_NUMBER
    randomlyGeneratePages(pages)
    runOPT(OPT, False, pages)
    runFunction(FIFO, "FIFO", False, pages)
    runFunction(LRU, "LRU", False, pages)
    runFunction(LFU, "LFU", False, pages)

if __name__ == "__main__":
    main()