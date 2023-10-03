package elevator.L2_1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;

public class MatchedUtil {
    public static ArrayList<int[]> toBeDones = new ArrayList<>();

    private MatchedUtil() {
    }

    public static ArrayList<Task> creatByHand(ElevatorState elevatorState) {
        System.out.print("Please enter the initial floor:");
        Scanner sc = new Scanner(System.in);
        elevatorState.setFloor(sc.nextInt());
        elevatorState.setDestination(elevatorState.getFloor());
        ArrayList<Task> list = new ArrayList<>();
        System.out.println("Please enter the call time, current floor, and target floor respectively");
        System.out.println("The three are separated by a space,a value of 0 for all three indicates the end");
        int callTime = sc.nextInt();
        int userFloor = sc.nextInt();
        int destination = sc.nextInt();
        while (userFloor != 0) {
            if (callTime == 0) {
                Task task = new Task();
                task.setUserFloor(userFloor);
                task.setDestination(destination);
                list.add(task);
            } else {
                int[] toBeDone = new int[3];
                toBeDone[0] = callTime;
                toBeDone[1] = userFloor;
                toBeDone[2] = destination;
                toBeDones.add(toBeDone);
            }
            callTime = sc.nextInt();
            userFloor = sc.nextInt();
            destination = sc.nextInt();
        }
        return list;
    }

    public static ArrayList<Task> createByFile(File f, ElevatorState elevatorState) throws FileNotFoundException {
        Scanner sc = new Scanner(f);
        String buffer = sc.next();
        elevatorState.setFloor(sc.nextInt());
        int callTime, userFloor, destination;
        ArrayList<Task> list = new ArrayList<>();
        while (sc.hasNext()) {
            callTime = sc.nextInt();
            userFloor = sc.nextInt();
            destination = sc.nextInt();
            if (callTime == 0) {
                Task task = new Task(userFloor, destination);
                list.add(task);
            } else {
                int[] toBeDone = new int[3];
                toBeDone[0] = callTime;
                toBeDone[1] = userFloor;
                toBeDone[2] = destination;
                toBeDones.add(toBeDone);
            }
        }
        sc.close();
        return list;
    }

    public static int isExist(int[] array, int target) {
        for (int i = 0; i < array.length; i++) {
            if (array[i] == target) {
                return i;
            }
        }
        return -1;
    }

    public static void addTask(ArrayList<Task> list, int runTime) {
        Iterator<int[]> iterator = toBeDones.iterator();
        while (iterator.hasNext()) {
            int[] toBeDone = iterator.next();
            if (toBeDone[0] == runTime) {
                Task task = new Task();
                task.setUserFloor(toBeDone[1]);
                task.setDestination(toBeDone[2]);
                list.add(task);
                iterator.remove();
            }
        }
    }
}
