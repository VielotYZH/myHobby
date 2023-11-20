package ospf;

public class TC {
    private int destination;
    private int cost;
    private int nextHop;

    public TC() {
    }

    public TC(int destination, int cost, int nextHop) {
        this.destination = destination;
        this.cost = cost;
        this.nextHop = nextHop;
    }

    public int getDestination() {
        return destination;
    }

    public void setDestination(int destination) {
        this.destination = destination;
    }

    public int getCost() {
        return cost;
    }

    public void setCost(int cost) {
        this.cost = cost;
    }

    public int getNextHop() {
        return nextHop;
    }

    public void setNextHop(int nextHop) {
        this.nextHop = nextHop;
    }
}
