package pageelimination;

@FunctionalInterface
public interface OPTFunction {
    boolean OPT(char[] pages, int currentIndex, char[] pageFrames);
}
