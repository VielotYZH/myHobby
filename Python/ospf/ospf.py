import os

class LSP():
    def __init__(self, neighbor, cost):
        self.neighbor = neighbor
        self.cost = cost

class TC():
    def __init__(self, destination, cost, nextHop):
        self.destination = destination
        self.cost = cost
        self.nextHop = nextHop

class Node():
    def __init__(self, number, LSPList, tentative, confirmed):
        self.number = number
        self.LSPList = LSPList
        self.tentative = tentative
        self.confirmed = confirmed

def initNodeList(f):
    number, neighbor, cost = 0, 0, 0
    list = []
    current = 0
    for line in f:
        number, neighbor, cost = map(int, line.strip().split())
        if current != number:
            current = number
            q = Node(number, [], [], [])
            q.LSPList.append(LSP(neighbor, cost))
            list.append(q)
        else:
            list[len(list) - 1].LSPList.append(LSP(neighbor, cost))
    return list

def locateLSPNode(list, number):
    for i in range(len(list)):
        if list[i].number == number:
            return list[i].LSPList
    return None

def ifExist(number, list):
    for i in range(len(list)):
        if list[i].destination == number:
            return list[i]
    return None

def moveTCNode(confirmed, tentative):
    minCost = tentative[0]
    for i in range(1, len(tentative), 1):
        if tentative[i].cost < minCost.cost:
            minCost = tentative[i]
    confirmed.append(TC(minCost.destination, minCost.cost, minCost.nextHop))
    tentative.remove(minCost)

def createRoutineTable(list, q):
    q.confirmed.append(TC(q.number, 0, 0))
    cp = q.confirmed[-1]
    while True:
        r = locateLSPNode(list, cp.destination)
        assert r != None
        for lsp in r:
            newCost = cp.cost + lsp.cost
            s = ifExist(lsp.neighbor, q.tentative)
            if not (ifExist(lsp.neighbor, q.confirmed) != None or s != None):
                q.tentative.append(TC(lsp.neighbor, newCost, lsp.neighbor if cp.nextHop == 0 else cp.nextHop))
            elif s != None and newCost < s.cost:
                s.cost = newCost
                s.nextHop = lsp.neighbor if cp.nextHop == 0 else cp.nextHop
        if len(q.tentative) == 0:
            break
        else:
            moveTCNode(q.confirmed, q.tentative)
            cp = q.confirmed[-1]
        
def main():
    if os.path.exists(r"Python\ospf\test.txt"):
        list = None
        with open(r"Python\ospf\test.txt", "r") as f:
            list = initNodeList(f)
        for i in range(len(list)):
            createRoutineTable(list, list[i])
            q = list[i].confirmed
            print(f"-------confirmed of node {chr(list[i].number + 64)}-------")
            for tc in q:
                print(f"({chr(tc.destination + 64)},{tc.cost},", end="")
                if tc.nextHop == 0:
                    print("-)")
                else:
                    print(f"{chr(tc.nextHop + 64)})")
    else:
        print("file not found")

if __name__ == "__main__":
    main()