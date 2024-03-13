package pageelimination;

public class TimeController {
    public int[] times;
    private boolean hasInit = false;

    public TimeController() {
    }

    public void initTime(int length) {
        if (!hasInit) {
            times = new int[length];
            for (int i = 0; i < length; i++) {
                times[i] = -1;
            }
            hasInit = true;
        }
    }
}
