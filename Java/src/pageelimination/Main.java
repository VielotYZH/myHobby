package pageelimination;

import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    static int pageFramesNumber = 3;
    static int pagesNumber = 10000;
    static int maxPageTypes = 5;

    static int missingPageNumber = 0;

    public static void randomlyGeneratePages(char[] arr) {
        Random r = new Random();
        for (int i = 0; i < arr.length; i++) {
            arr[i] = (char) (r.nextInt(maxPageTypes) + 65);
        }
    }

    public static int contains(char[] arr, int start, int end, char element) {
        for (int i = start; i < end; i++) {
            if (arr[i] == element) {
                return i;
            }
        }
        return -1;
    }

    public static void printPageFrames(char[] arr) {
        for (char c : arr) {
            System.out.print(c + "\t\t\t");
        }
    }

    public static boolean OPT(char[] pages, int currentIndex, char[] pageFrames) {
        if (contains(pageFrames, 0, pageFrames.length, pages[currentIndex]) == -1) {
            int availableIndex = contains(pageFrames, 0, pageFrames.length, '\0');
            if (availableIndex != -1) {
                pageFrames[availableIndex] = pages[currentIndex];
            } else {
                int targetIndex = 0;
                int maxIndex = 0;
                for (int i = 0; i < pageFrames.length; i++) {
                    int tempIndex = contains(pages, currentIndex + 1, pages.length, pageFrames[i]);
                    if (tempIndex == -1) {
                        targetIndex = i;
                        break;
                    } else if (tempIndex > maxIndex) {
                        maxIndex = tempIndex;
                        targetIndex = i;
                    }
                }
                pageFrames[targetIndex] = pages[currentIndex];
            }
            return true;
        }
        return false;
    }

    public static boolean FIFO(char page, char[] pageFrames, TimeController tc) {
        tc.initTime(pageFramesNumber);
        boolean missingPage = false;
        if (contains(pageFrames, 0, pageFramesNumber, page) == -1) {
            missingPage = true;
            int availableIndex = contains(pageFrames, 0, pageFramesNumber, '\0');
            if (availableIndex != -1) {
                pageFrames[availableIndex] = page;
                tc.times[availableIndex] = 0;
            } else {
                int targetIndex = 0;
                int i = 1;
                for (; i < pageFramesNumber; i++) {
                    if (tc.times[i] > tc.times[targetIndex]) {
                        targetIndex = i;
                    }
                }
                pageFrames[targetIndex] = page;
                tc.times[targetIndex] = 0;
            }
        }
        for (int i = 0; i < pageFramesNumber; i++) {
            if (tc.times[i] != -1) {
                tc.times[i]++;
            }
        }
        return missingPage;
    }

    public static boolean LRU(char page, char[] pageFrames, TimeController tc) {
        tc.initTime(pageFramesNumber);
        boolean missingPage = false;
        int index = contains(pageFrames, 0, pageFramesNumber, page);
        if (index == -1) {
            missingPage = true;
            int availableIndex = contains(pageFrames, 0, pageFramesNumber, '\0');
            if (availableIndex != -1) {
                pageFrames[availableIndex] = page;
                tc.times[availableIndex] = 0;
            } else {
                int targetIndex = 0;
                int i = 1;
                for (; i < pageFramesNumber; i++) {
                    if (tc.times[i] > tc.times[targetIndex]) {
                        targetIndex = i;
                    }
                }
                pageFrames[targetIndex] = page;
                tc.times[targetIndex] = 0;
            }
        } else {
            tc.times[index] = 0;
        }
        for (int i = 0; i < pageFramesNumber; i++) {
            if (tc.times[i] != -1) {
                tc.times[i]++;
            }
        }
        return missingPage;
    }

    public static boolean LFU(char page, char[] pageFrames, TimeController tc) {
        tc.initTime(pageFramesNumber);
        int index = contains(pageFrames, 0, pageFramesNumber, page);
        if (index == -1) {
            int availableIndex = contains(pageFrames, 0, pageFramesNumber, '\0');
            if (availableIndex != -1) {
                pageFrames[availableIndex] = page;
                tc.times[availableIndex]++;
            } else {
                int targetIndex = 0;
                for (int i = 0; i < pageFramesNumber; i++) {
                    if (tc.times[i] < tc.times[targetIndex]) {
                        targetIndex = i;
                    }
                }
                pageFrames[targetIndex] = page;
                Arrays.fill(tc.times, 0);
            }
            return true;
        } else {
            tc.times[index]++;
            return false;
        }
    }

    public static int runOPT(OPTFunction p, boolean printProcedure, char[] pages) {
        char[] pageFrames = new char[pageFramesNumber];
        Arrays.fill(pageFrames, '\0');
        if (printProcedure) {
            for (int i = 0; i < pageFramesNumber; i++) {
                System.out.print("pageFrame" + (i + 1) + "\t");
            }
            System.out.println("pageMissing");
        }
        for (int i = 0; i < pages.length; i++) {
            boolean missingPage = p.OPT(pages, i, pageFrames);
            if (missingPage) {
                missingPageNumber++;
            }
            if (printProcedure) {
                printPageFrames(pageFrames);
                if (missingPage) {
                    System.out.print('*');
                }
                System.out.println();
            }
        }
        System.out.println("OPT missing page number:" + missingPageNumber);
        System.out.println("OPT page fault rate:" + String.format("%.2f", (double) missingPageNumber / pages.length * 100) + "%");
        System.out.println();
        System.out.println();
        int result = missingPageNumber;
        missingPageNumber = 0;
        return result;
    }

    public static int runFunction(Function p, String functionName, boolean printProcedure, char[] pages) {
        char[] pageFrames = new char[pageFramesNumber];
        Arrays.fill(pageFrames, '\0');
        if (printProcedure) {
            for (int i = 0; i < pageFramesNumber; i++) {
                System.out.print("pageFrame" + (i + 1) + "\t");
            }
            System.out.println("pageMissing");
        }
        TimeController tc = new TimeController();
        for (char page : pages) {
            boolean missingPage = p.func(page, pageFrames, tc);
            if (missingPage) {
                missingPageNumber++;
            }
            if (printProcedure) {
                printPageFrames(pageFrames);
                if (missingPage) {
                    System.out.print('*');
                }
                System.out.println();
            }
        }
        System.out.println(functionName + " missing page number:" + missingPageNumber);
        System.out.println(functionName + " page fault rate:" + String.format("%.2f", (double) missingPageNumber / pages.length * 100) + "%");
        System.out.println();
        System.out.println();
        int result = missingPageNumber;
        missingPageNumber = 0;
        return result;
    }

    public static void testOPT(OPTFunction p) {
        char[] arr = {'A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E'};

        runOPT(p, true, arr);

        System.exit(0);
    }

    public static void test(Function p, String functionName) {
        char[] arr = {'A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E'};

        runFunction(p, functionName, true, arr);

        System.exit(0);
    }

    public static void main(String[] args) {
        /*testOPT(Main::OPT);
        test(Main::FIFO, "FIFO");
        test(Main::LRU, "LRU");
        test(Main::LFU, "LFU");*/

        char[] pages = new char[pagesNumber];
        randomlyGeneratePages(pages);

        ExecutorService pool = Executors.newCachedThreadPool();

        /*runOPT(Main::OPT, false, pages);

        runFunction(Main::FIFO, "FIFO", false, pages);

        runFunction(Main::LRU, "LRU", false, pages);

        runFunction(Main::LFU, "LFU", false, pages);*/
    }
}
