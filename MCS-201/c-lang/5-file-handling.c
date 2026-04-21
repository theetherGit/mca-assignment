/*
 * Program: Even & Odd Number File Separator
 * Description: Reads N integers, validates input, separates evens/odds into files
 */

#include <stdio.h>

// Clear leftover characters from stdin
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Robust integer input validator
int readValidInteger(const char *prompt) {
    int value;
    char nextChar;
    while (1) {
        printf("%s", prompt);
        // Read integer + capture next character to detect trailing garbage
        if (scanf("%d%c", &value, &nextChar) == 2 && (nextChar == '\n' || nextChar == ' ')) {
            return value;
        }
        clearBuffer(); // Discard invalid input
        printf("   [!] Invalid. Enter a valid integer.\n");
    }
}

int main() {
    int n;
    FILE *evenFile, *oddFile;

    printf("=== Even & Odd Number Separator ===\n");
    n = readValidInteger("Enter number of elements (N): ");

    if (n <= 0) {
        printf("   [!] N must be a positive integer. Exiting.\n");
        return 1;
    }

    // Open files (creates or overwrites)
    evenFile = fopen("./output/evenfile", "w");
    oddFile  = fopen("./output/oddfile",  "w");

    if (!evenFile || !oddFile) {
        printf("   [!] Error: Could not create output files.\n");
        return 1;
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        char prompt[32];
        snprintf(prompt, sizeof(prompt), " Number %d: ", i + 1);
        int num = readValidInteger(prompt);
        if (num % 2 == 0)
            fprintf(evenFile, "%d\n", num);
        else
            fprintf(oddFile, "%d\n", num);
    }

    // Proper text file formatting
    fprintf(evenFile, "\n");
    fprintf(oddFile, "\n");

    fclose(evenFile);
    fclose(oddFile);

    printf("\n✔ Separation complete!\n");
    printf("  Even numbers → evenfile\n");
    printf("  Odd numbers  → oddfile\n");

    return 0;
}
