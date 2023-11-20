package ospf;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class ospf {
    public static void main(String[] args) throws FileNotFoundException {
        File f = new File("src/ospf/test.txt");
        ArrayList<Node> list = initNodeList(f);
        for (int i = 0; i < list.size(); i++) {
            createRoutineTable(list, list.get(i));
            ArrayList<TC> q = list.get(i).getConfirmed();
            System.out.printf("-------confirmed of node %c-------\n", list.get(i).getNumber() + 64);
            for (TC tc : q) {
                System.out.printf("(%c,%d,", tc.getDestination() + 64, tc.getCost());
                if (tc.getNextHop() == 0) {
                    System.out.println("-)");
                } else {
                    System.out.printf("%c)\n", tc.getNextHop() + 64);
                }
            }
        }
    }

    public static ArrayList<LSP> locateLSPNode(ArrayList<Node> list, int number) {
        int i = 0;
        for (; i < list.size(); i++) {
            if (list.get(i).getNumber() == number) {
                return list.get(i).getLSPList();
            }
        }
        return null;
    }

    public static void createRoutineTable(ArrayList<Node> list, Node q) {
        q.getConfirmed().add(new TC(q.getNumber(), 0, 0));
        TC cp = q.getConfirmed().get(q.getConfirmed().size() - 1);
        do {
            ArrayList<LSP> r = locateLSPNode(list, cp.getDestination());
            assert r != null;
            for (LSP lsp : r) {
                int newCost = cp.getCost() + lsp.getCost();
                TC s = ifExist(lsp.getNeighbor(), q.getTentative());
                if (!(ifExist(lsp.getNeighbor(), q.getConfirmed()) != null || s != null)) {
                    q.getTentative().add(new TC(lsp.getNeighbor(), newCost, cp.getNextHop() == 0 ? lsp.getNeighbor() : cp.getNextHop()));
                } else if (s != null && newCost < s.getCost()) {
                    s.setCost(newCost);
                    s.setNextHop(cp.getNextHop() == 0 ? lsp.getNeighbor() : cp.getNextHop());
                }
            }
            if (q.getTentative().isEmpty()) {
                break;
            } else {
                moveTCNde(q.getConfirmed(), q.getTentative());
                cp = q.getConfirmed().get(q.getConfirmed().size() - 1);
            }
        } while (true);
    }

    public static void moveTCNde(ArrayList<TC> confirmed, ArrayList<TC> tentative) {
        TC minCost = tentative.get(0);
        for (int i = 1; i < tentative.size(); i++) {
            if (tentative.get(i).getCost() < minCost.getCost()) {
                minCost = tentative.get(i);
            }
        }
        confirmed.add(new TC(minCost.getDestination(), minCost.getCost(), minCost.getNextHop()));
        tentative.remove(minCost);
    }

    public static TC ifExist(int number, ArrayList<TC> list) {
        int i = 0;
        for (; i < list.size(); i++) {
            if (list.get(i).getDestination() == number) {
                return list.get(i);
            }
        }
        assert i != list.size();
        return null;//unmeaning
    }

    public static ArrayList<Node> initNodeList(File f) throws FileNotFoundException {
        int number, neighbor, cost;
        ArrayList<Node> list = new ArrayList<>();
        int current = 0;
        Scanner sc = new Scanner(f);
        while (sc.hasNext()) {
            number = sc.nextInt();
            neighbor = sc.nextInt();
            cost = sc.nextInt();
            if (current != number) {
                current = number;
                Node q = new Node();
                q.setNumber(number);

                q.setLSPList(new ArrayList<>());
                q.getLSPList().add(new LSP(neighbor, cost));

                q.setTentative(new ArrayList<>());

                q.setConfirmed(new ArrayList<>());
                list.add(q);
            } else {
                list.get(list.size() - 1).getLSPList().add(new LSP(neighbor, cost));
            }
        }
        return list;
    }
}
