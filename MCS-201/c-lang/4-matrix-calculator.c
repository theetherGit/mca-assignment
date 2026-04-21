#include <stdio.h>

#define SIZE 3

/* Helper function to clear the input buffer when non-numeric
   data is entered, preventing infinite loops. */
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Function to handle user input one index at a time with validation */
void inputMatrix(const char *name, int mat[SIZE][SIZE]) {
    printf("\n--- Enter elements for Matrix %s (3x3) ---\n", name);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("Enter element %s[%d][%d]: ", name, i, j);

            // Validate that the input is actually an integer
            while (scanf("%d", &mat[i][j]) != 1) {
                printf("  [!] Invalid input. Please enter a number for %s[%d][%d]: ", name, i, j);
                clearBuffer();
            }
        }
    }
}

// Function to display a 3×3 matrix
void displayMatrix(int mat[SIZE][SIZE], const char *name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < SIZE; i++) {
        printf("  ");
        for (int j = 0; j < SIZE; j++)
            printf("%4d ", mat[i][j]);
        printf("\n");
    }
}

// Function to multiply two 3×3 matrices: result = X × Y
void multiplyMatrices(int X[SIZE][SIZE], int Y[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
                result[i][j] += X[i][k] * Y[k][j];
        }
    }
}

// Function to add two 3×3 matrices: result = X + Y
void addMatrices(int X[SIZE][SIZE], int Y[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            result[i][j] = X[i][j] + Y[i][j];
}

int main() {
    int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    int BC[SIZE][SIZE], D[SIZE][SIZE];  // BC = B×C, D = A + BC

    printf("=== Matrix Operation: D = A + (B × C) ===\n\n");

    // Input matrices
    inputMatrix("A", A); inputMatrix("B", B);
    inputMatrix("C", C);

    // Display input matrices
    printf("\n--- Input Matrices ---\n");
    displayMatrix(A, "A");
    displayMatrix(B, "B");
    displayMatrix(C, "C");

    // Step 1: Compute B × C
    multiplyMatrices(B, C, BC);
    printf("\n--- B × C Result ---\n");
    displayMatrix(BC, "B×C");

    // Step 2: Compute D = A + (B × C)
    addMatrices(A, BC, D);
    printf("\n--- Final Result: D = A + (B × C) ---\n");
    displayMatrix(D, "D");

    return 0;
}
