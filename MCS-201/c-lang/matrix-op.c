#include <stdio.h>

/* Helper function to clear the input buffer when non-numeric
   data is entered, preventing infinite loops. */
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Function to print the 3x3 matrix in a readable format */
void printMatrix(int mat[3][3]) {
    for (int i = 0; i < 3; i++) {
        printf("  [ ");
        for (int j = 0; j < 3; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf(" ]\n");
    }
}

/* Function to handle user input one index at a time with validation */
void inputMatrix(char name, int mat[3][3]) {
    printf("\n--- Enter elements for Matrix %c (3x3) ---\n", name);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("Enter element %c[%d][%d]: ", name, i, j);

            // Validate that the input is actually an integer
            while (scanf("%d", &mat[i][j]) != 1) {
                printf("  [!] Invalid input. Please enter a number for %c[%d][%d]: ", name, i, j);
                clearBuffer();
            }
        }
    }
}

int main() {
    int A[3][3], B[3][3], C[3][3];
    int BC[3][3] = {0}; // Intermediate product matrix
    int D[3][3];        // Final result matrix

    printf("Matrix Calculator: D = A + (B * C)\n");
    printf("====================================\n");

    // 1. Data Entry
    inputMatrix('A', A);
    inputMatrix('B', B);
    inputMatrix('C', C);

    // 2. Multiplication Phase: BC = B * C
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                BC[i][j] += B[i][k] * C[k][j];
            }
        }
    }

    // 3. Addition Phase: D = A + BC
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            D[i][j] = A[i][j] + BC[i][j];
        }
    }

    // 4. Final Display
    printf("\n====================================");
    printf("\nStep 1: Result of (B * C):\n");
    printMatrix(BC);

    printf("\nStep 2: Final Result D = A + (B * C):\n");
    printMatrix(D);
    printf("====================================\n");

    return 0;
}
