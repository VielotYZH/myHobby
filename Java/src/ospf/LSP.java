package ospf;

public class LSP {
    private int neighbor;
    private int cost;

    public LSP() {
    }

    public LSP(int neighbor, int cost) {
        this.neighbor = neighbor;
        this.cost = cost;
    }

    public int getNeighbor() {
        return neighbor;
    }

    public void setNeighbor(int neighbor) {
        this.neighbor = neighbor;
    }

    public int getCost() {
        return cost;
    }

    public void setCost(int cost) {
        this.cost = cost;
    }
}
