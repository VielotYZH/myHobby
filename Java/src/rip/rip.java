package rip;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class rip {
    public static final int INFINITY = 1024;

    public static boolean ifChanged = true;

    public static void main(String[] args) throws FileNotFoundException {
        File f = new File("src/rip/test.txt");
        RoutingInfo[][] routingTable = initRountingTable(f);
        int nodeNumber = routingTable.length;
        while (ifChanged) {
            ifChanged = false;
            sendRoutingTable(routingTable);
            for (RoutingInfo[] routingInfos : routingTable) {
                for (int j = 0; j < nodeNumber; j++) {
                    if (routingInfos[j].getCost() == INFINITY) {
                        System.out.print("* ");
                    } else {
                        System.out.print(routingInfos[j].getCost() + " ");
                    }
                }
                System.out.println();
            }
            System.out.println("------------------------------------");
        }
    }

    public static boolean isAdjacent(RoutingInfo[][] routingTable, int nodeNum1, int nodeNum2) {
        for (int i = 0; i < routingTable.length; i++) {
            if (routingTable[nodeNum1][i].getNextHop() == nodeNum2) {
                return true;
            }
        }
        return false;
    }

    public static void receiveRoutingTable(RoutingInfo[][] routingTable, int senderNum, int receiverNum, int[] senderInfo) {
        for (int i = 0; i < routingTable.length; i++) {
            if (routingTable[receiverNum][i].getCost() > routingTable[receiverNum][senderNum].getCost() + senderInfo[i]) {
                routingTable[receiverNum][i].setCost(routingTable[receiverNum][senderNum].getCost() + senderInfo[i]);
                routingTable[receiverNum][i].setNextHop(senderNum);
                ifChanged = true;
            }
        }
    }

    public static void sendRoutingTable(RoutingInfo[][] routingTable) {
        int nodeNumber = routingTable.length;
        int[][] senderInfos = new int[nodeNumber][nodeNumber];
        for (int i = 0; i < nodeNumber; i++) {
            for (int j = 0; j < nodeNumber; j++) {
                senderInfos[i][j] = routingTable[i][j].getCost();
            }
        }
        for (int i = 0; i < nodeNumber; i++) {
            for (int j = 0; j < nodeNumber; j++) {
                if (isAdjacent(routingTable, i, j)) {
                    receiveRoutingTable(routingTable, i, j, senderInfos[i]);
                }
            }
        }
    }

    public static RoutingInfo[][] initRountingTable(File f) throws FileNotFoundException {
        Scanner sc = new Scanner(f);
        int nodeNumber = sc.nextInt();
        RoutingInfo[][] routingTable = new RoutingInfo[nodeNumber][nodeNumber];
        for (int i = 0; i < nodeNumber; i++) {
            for (int j = 0; j < nodeNumber; j++) {
                routingTable[i][j] = new RoutingInfo();
            }
        }
        int token;
        String buffer;
        while (sc.hasNext()) {
            for (RoutingInfo[] routingInfos : routingTable) {
                for (int j = 0; j < routingInfos.length; j++) {
                    try {
                        token = sc.nextInt();
                        routingInfos[j].setCost(token);
                    } catch (InputMismatchException e) {
                        buffer = sc.next();
                        if (buffer.equals("i")) {
                            routingInfos[j].setCost(INFINITY);
                        }
                    }
                    routingInfos[j].setDestination(j);
                    routingInfos[j].setNextHop(routingInfos[j].getCost() == INFINITY || routingInfos[j].getCost() == 0 ? routingInfos.length : j);
                }
            }
        }
        return routingTable;
    }
}
