#include <stdio.h>

void decimalToBinary(int n) {
    // Handle negative numbers (binary representation typically for non-negative integers)
    if (n < 0) {
        printf("Please enter a non-negative integer.\n");
        return;
    }

    int binary[32]; // Array to store binary digits (max 32 bits for standard int)
    int i = 0;

    // Special case for 0
    if (n == 0) {
        binary[0] = 0;
        i = 1;
    } else {
        // Repeatedly divide by 2 and store remainders
        while (n > 0) {
            binary[i] = n % 2;
            n = n / 2;
            i++;
        }
    }

    // Print the binary digits in reverse order
    printf("Binary Equivalent: ");
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binary[j]);
    }
    printf("\n");
}

int main() {
    int num;
    printf("Enter a decimal number: ");
    scanf("%d", &num);

    decimalToBinary(num);

    return 0;
}
