package ospf;

import java.util.ArrayList;

public class Node {
    private int number;
    private ArrayList<LSP> LSPList;
    private ArrayList<TC> tentative;
    private ArrayList<TC> confirmed;

    public Node() {
    }

    public Node(int number, ArrayList<LSP> LSPList, ArrayList<TC> tentative, ArrayList<TC> confirmed) {
        this.number = number;
        this.LSPList = LSPList;
        this.tentative = tentative;
        this.confirmed = confirmed;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public ArrayList<LSP> getLSPList() {
        return LSPList;
    }

    public void setLSPList(ArrayList<LSP> LSPList) {
        this.LSPList = LSPList;
    }

    public ArrayList<TC> getTentative() {
        return tentative;
    }

    public void setTentative(ArrayList<TC> tentative) {
        this.tentative = tentative;
    }

    public ArrayList<TC> getConfirmed() {
        return confirmed;
    }

    public void setConfirmed(ArrayList<TC> confirmed) {
        this.confirmed = confirmed;
    }
}
