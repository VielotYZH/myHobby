package HDBn;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class HDBn {
    public static final int n = 3;

    public static void main(String[] args) throws FileNotFoundException {
        File f = new File("src/HDBn/test.txt");
        String source = readFile(f);
        String coding = encode(source);
        String decoding = decode(coding);
        System.out.println("source:\t\t" + source);
        System.out.println("coding:\t\t" + coding);
        System.out.println("decoding:\t" + decoding);
    }

    public static String readFile(File f) throws FileNotFoundException {
        Scanner sc = new Scanner(f);
        return sc.next();
    }

    public static String encode(String source) {
        StringBuilder result = new StringBuilder();
        int counter = 0;
        int last = 1;
        int lastV = 1;
        for (int i = 0; i < source.length(); i++) {
            if (source.charAt(i) == '1') {
                counter = 0;
                result.append(last == 1 ? 'n' : '1');
                last = -last;
            } else if (source.charAt(i) == '0') {
                counter++;
                result.append('0');
                if (counter == n + 1) {
                    counter = 0;
                    result.setCharAt(i, lastV == 1 ? 'n' : '1');
                    lastV = -lastV;
                    if (result.charAt(i - n - 1) != result.charAt(i)) {
                        result.setCharAt(i - n, result.charAt(i));
                    }
                }
            }
        }
        return result.toString();
    }

    public static String decode(String coding) {
        StringBuilder result = new StringBuilder();
        int last = 1;
        for (int i = 0; i < coding.length(); i++) {
            if (coding.charAt(i) == 'n') {
                result.append('1');
                if (last == -1) {
                    for (int j = i; j >= i - n; j--) {
                        result.setCharAt(j, '0');
                    }
                } else {
                    last = -last;
                }
            } else if (coding.charAt(i) == '1') {
                result.append('1');
                if (last == 1) {
                    for (int j = i; j >= i - n; j--) {
                        result.setCharAt(j, '0');
                    }
                } else {
                    last = -last;
                }
            } else if (coding.charAt(i) == '0') {
                result.append('0');
            }
        }
        return result.toString();
    }
}
