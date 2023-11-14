package Gray;

import java.util.Arrays;
import java.util.Scanner;

public class Gray {
    public static int length, number;

    public static void main(String[] args) {
        char[][] gray = init();
        encode(gray);
        System.out.println("Number\tGray\tBinary");
        for (int i = 0; i < number; i++) {
            System.out.println(i + "\t\t" + new String(gray[i]) + "\t" + new String(decimalToBinary(i)));
        }
    }

    public static char[][] init() {
        System.out.print("Please enter the length of the gray code you want:");
        Scanner sc = new Scanner(System.in);
        length = sc.nextInt();
        number = (int) Math.pow(2, length);
        char[][] gray = new char[number][length];
        for (int i = 0; i < number; i++) {
            Arrays.fill(gray[i], '0');
        }
        return gray;
    }

    public static void encode(char[][] gray) {
        for (int i = 1; i <= length; i++) {
            singleOperate(i, gray);
        }
    }

    public static void singleOperate(int digital_number, char[][] gray) {
        if (digital_number == 1) {
            gray[1][length - 1] = '1';
        } else {
            int range = (int) Math.pow(2, digital_number);
            for (int i = 0; i < range / 2; i++) {
                for (int j = length - 1; j >= length - digital_number; j--) {
                    gray[range - 1 - i][j] = gray[i][j];
                    gray[range - 1 - i][length - digital_number] = '1';
                }
            }
        }
    }

    public static char[] decimalToBinary(int decimal) {
        char[] binary = new char[length];
        Arrays.fill(binary, '0');
        int index = length - 1;
        while (decimal > 1) {
            int quotient = decimal / 2;
            binary[index--] = decimal - quotient * 2 == 1 ? '1' : '0';
            decimal = quotient;
        }
        binary[index] = decimal == 1 ? '1' : '0';
        return binary;
    }
}
