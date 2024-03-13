package pageelimination;

@FunctionalInterface
public interface Function {
    boolean func(char page, char[] pageFrames, TimeController tc);
}
