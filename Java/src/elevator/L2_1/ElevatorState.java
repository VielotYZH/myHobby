package elevator.L2_1;

import java.util.ArrayList;
import java.util.Iterator;

public class ElevatorState {
    public static final int MAX_LOAD = 4;

    private int floor;
    private int number;
    private int direction;
    private int destination;
    private int[] userDestination;

    public ElevatorState() {
    }

    public ElevatorState(int floor, int number, int direction, int destination, int[] userDestination) {
        if (userDestination.length != MAX_LOAD) {
            System.out.println("The length of the array passed in can only be " + MAX_LOAD);
            System.exit(1);
        }
        this.floor = floor;
        this.number = number;
        this.direction = direction;
        this.destination = destination;
        this.userDestination = userDestination;
    }

    public int getFloor() {
        return floor;
    }

    public void setFloor(int floor) {
        this.floor = floor;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public int getDirection() {
        return direction;
    }

    public void setDirection(int direction) {
        this.direction = direction;
    }

    public int getDestination() {
        return destination;
    }

    public void setDestination(int destination) {
        this.destination = destination;
    }

    public int[] getUserDestination() {
        return userDestination;
    }

    public void setUserDestination(int[] userDestination) {
        if (userDestination.length != MAX_LOAD) {
            System.out.println("The length of the array passed in can only be " + MAX_LOAD);
            System.exit(1);
        }
        this.userDestination = userDestination;
    }

    public void setDirection(ArrayList<Task> list) {
        for (int i = 0; i < MAX_LOAD; i++) {
            if (userDestination[i] != 0) {
                direction = Integer.compare(0, floor - userDestination[i]);
                return;
            }
        }
        if (!list.isEmpty()) {
            direction = Integer.compare(0, floor - list.get(0).getUserFloor());
        } else {
            direction = 0;
        }
    }

    public void setDestination(ArrayList<Task> list) {
        if (direction == 1) {
            for (int i = 0; i < MAX_LOAD; i++) {
                if (userDestination[i] > destination) {
                    destination = userDestination[i];
                }
            }
            for (Task task : list) {
                if (task.getUserFloor() > destination) {
                    destination = task.getUserFloor();
                }
            }
        } else if (direction == -1) {
            for (int i = 0; i < MAX_LOAD; i++) {
                if (userDestination[i] != 0) {
                    if (userDestination[i] < destination) {
                        destination = userDestination[i];
                    }
                }
            }
            for (Task task : list) {
                if (task.getUserFloor() < destination) {
                    destination = task.getUserFloor();
                }
            }
        }
    }

    public boolean determineIfSyntropy(Task task) {
        if (direction == 1 && task.getUserFloor() - task.getDestination() < 0) {
            return true;
        } else return direction == -1 && task.getUserFloor() - task.getDestination() > 0;
    }

    public void scheduleElevator(ArrayList<Task> list) {
        boolean isEmpty = true;
        int runTime = 0;
        while (!(MatchedUtil.toBeDones.isEmpty() && isEmpty && list.isEmpty())) {
            int index = MatchedUtil.isExist(userDestination, floor);
            while (index != -1) {
                userDestination[index] = 0;
                number--;
                index = MatchedUtil.isExist(userDestination, floor);
            }
            Iterator<Task> iterator = list.iterator();
            while (iterator.hasNext()) {
                Task task = iterator.next();
                if (task.getUserFloor() == floor) {
                    if (determineIfSyntropy(task) || task.getUserFloor() == destination) {
                        index = MatchedUtil.isExist(userDestination, 0);
                        if (index == -1) {
                            System.out.println("The elevator is overloaded on the " + floor + " floor");
                        } else {
                            number++;
                            userDestination[index] = task.getDestination();
                            iterator.remove();
                        }
                    }
                }
            }
            System.out.println(runTime + "\t" + floor + "\t\t" + number);
            setDirection(list);
            setDestination(list);
            floor += direction;
            runTime++;
            MatchedUtil.addTask(list, runTime);
            for (int i = 0; i < MAX_LOAD; i++) {
                if (userDestination[i] != 0) {
                    isEmpty = false;
                    break;
                }
                isEmpty = true;
            }
        }
    }
}
