package elevator.L2_1;

public class Task {
    private int userFloor;
    private int destination;

    public Task() {
    }

    public Task(int userFloor, int destination) {
        this.userFloor = userFloor;
        this.destination = destination;
    }

    public int getUserFloor() {
        return userFloor;
    }

    public void setUserFloor(int userFloor) {
        this.userFloor = userFloor;
    }

    public int getDestination() {
        return destination;
    }

    public void setDestination(int destination) {
        this.destination = destination;
    }
}
