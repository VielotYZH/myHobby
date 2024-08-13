import os

INFINITY = 1024

class RoutingInfo():
    def __init__(self, destination, cost, nextHop):
        self.destination = destination
        self.cost = cost
        self.nextHop = nextHop

ifChanged = True

def initRoutingTable(f):
    nodeNumber = int(f.readline())
    routingTable = [[None] * nodeNumber for _ in range(nodeNumber)]
    for i in range(nodeNumber):
        line = f.readline().strip().split()
        for j in range(nodeNumber):
            try:
                token = int(line[j])
                routingTable[i][j] = RoutingInfo(j, token, 0)
            except ValueError:
                buffer = line[j]
                if buffer == "i":
                    routingTable[i][j] = RoutingInfo(j, INFINITY, 0)
            routingTable[i][j].nextHop = nodeNumber if routingTable[i][j].cost == INFINITY or routingTable[i][j].cost == 0 else j
    return routingTable

def isAdjacent(routingTable, nodeNum1, nodeNum2):
    for i in range(len(routingTable)):
        if routingTable[nodeNum1][i].nextHop == nodeNum2:
            return True
    return False

def receiveRoutingTable(routingTable, senderNum, receiverNum, senderInfo):
    global ifChanged
    for i in range(len(routingTable)):
        if routingTable[receiverNum][i].cost > routingTable[receiverNum][senderNum].cost + senderInfo[i]:
            routingTable[receiverNum][i].cost = routingTable[receiverNum][senderNum].cost + senderInfo[i]
            routingTable[receiverNum][i].nextHop = senderNum
            ifChanged = True

def sendRoutingTable(routingTable):
    nodeNumber = len(routingTable)
    senderInfos = [[None] * nodeNumber for _ in range(nodeNumber)]
    for i in range(nodeNumber):
        for j in range(nodeNumber):
            senderInfos[i][j] = routingTable[i][j].cost
    for i in range(nodeNumber):
        for j in range(nodeNumber):
            if isAdjacent(routingTable, i ,j):
                receiveRoutingTable(routingTable, i, j, senderInfos[i])

def main():
    if os.path.exists(r"Python\rip\test.txt"):
        routingTable = None
        with open(r"Python\rip\test.txt", "r") as f:
            routingTable = initRoutingTable(f)
        nodeNumber = len(routingTable)
        global ifChanged
        while ifChanged:
            ifChanged = False
            sendRoutingTable(routingTable)
            for routingInfos in routingTable:
                for i in range(nodeNumber):
                    if routingInfos[i].cost == INFINITY:
                        print("* ", end="")
                    else:
                        print(f"{routingInfos[i].cost} ", end="")
                print()
            print("------------------------------------")
    else:
        print("file not found")

if __name__ == "__main__":
    main()