package elevator.L2_1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws FileNotFoundException {
        System.out.println("Please enter a number to select the data entry method:");
        System.out.println("0:manual input   1:file input");
        ElevatorState elevatorState = new ElevatorState();
        elevatorState.setNumber(0);
        elevatorState.setDirection(0);
        elevatorState.setUserDestination(new int[]{0, 0, 0, 0});
        ArrayList<Task> list = new ArrayList<>();
        Scanner sc = new Scanner(System.in);
        int method = sc.nextInt();
        if (method == 0) {
            list = MatchedUtil.creatByHand(elevatorState);
        } else if (method == 1) {
            File f = new File("src/elevator/L2_1/test.txt");
            list = MatchedUtil.createByFile(f, elevatorState);
            elevatorState.setDestination(elevatorState.getFloor());
        }
        elevatorState.setDirection(list);
        elevatorState.setDestination(list);
        System.out.println("runTime\tcurrentFloor\tcurrentNumber");
        elevatorState.scheduleElevator(list);
    }
}
