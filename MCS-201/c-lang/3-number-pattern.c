#include <stdio.h>

// Function to print the pattern
void printNumberPattern(int rows) {
    for (int i = 1; i <= rows; i++) {           // Outer loop: each row
        for (int j = 1; j <= i; j++) {          // Inner loop: numbers in row
            printf("%d", j);
            if (j < i) printf(" ");             // Space between numbers
        }
        printf("\n");                            // New line after each row
    }
}

int main() {
    int rows;

    printf("=== Number Pattern Generator ===\n");
    printf("Enter number of rows (1-20): ");

    // Input with validation
    if (scanf("%d", &rows) != 1 || rows < 1 || rows > 20) {
        printf("Invalid input. Using default: 5 rows\n");
        rows = 5;
    }

    printf("\nGenerated Pattern:\n");
    printf("------------------\n");
    printNumberPattern(rows);   // Call pattern function

    return 0;
}
