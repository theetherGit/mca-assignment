#include <stdio.h>

int main() {
    int rows = 5; // Change this value to adjust the number of rows

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= i; j++) {
            printf("%d", j);
            // Print a space after the number only if it's not the last one in the row
            if (j < i) {
                printf(" ");
            }
        }
        // Move to the next line after printing each row
        printf("\n");
    }

    return 0;
}
