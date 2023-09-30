#include<stdio.h>
void main() {
    double a[30][2];
    int index = 0;
    scanf("%lf %lf", &a[index][0], &a[index][1]);
    while (a[index][0] != 0) {
        index++;
        scanf("%lf %lf", &a[index][0], &a[index][1]);
    }
    for (int i = 0; i < index; i++) {
        if (a[i][0] < 31.1) {
            //爆伤头
            printf("%lf\n", a[i][0] * 2 + a[i][1] - 62.2);
        }
        else {
            //暴击头
            printf("%lf\n", (a[i][0] - 31.1) * 2 + a[i][1]);
        }
    }
}