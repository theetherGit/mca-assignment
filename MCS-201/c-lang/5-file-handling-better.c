#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define EVEN_FILE "./output/evenfile.txt"
#define ODD_FILE  "./output/oddfile.txt"

/* Safely reads an integer from stdin. Returns 1 on success, 0 on EOF/error. */
int read_integer(const char *prompt, int *out) {
    char buf[256];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;

    char *endptr;
    errno = 0;
    long val = strtol(buf, &endptr, 10);

    if (errno == ERANGE || val < INT_MIN || val > INT_MAX ||
        endptr == buf || (*endptr != '\n' && *endptr != '\0')) {
        return 0; // Invalid or out-of-range
    }
    *out = (int)val;
    return 1;
}

/* Checks file write status. Returns 0 on success, -1 on failure. */
int check_write(int written, const char *filename) {
    if (written < 0) {
        fprintf(stderr, "Error: Failed to write to '%s' (%s)\n", filename, strerror(errno));
        return -1;
    }
    return 0;
}

int main(void) {
    printf("========================================\n");
    printf("    EVEN & ODD NUMBER SEPARATOR\n");
    printf("========================================\n\n");

    int n;
    if (!read_integer("Enter the number of integers (N) to process: ", &n) || n <= 0) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    FILE *even_fp = fopen(EVEN_FILE, "w");
    FILE *odd_fp  = fopen(ODD_FILE, "w");

    if (!even_fp || !odd_fp) {
        perror("Error: Failed to open output files");
        if (even_fp) fclose(even_fp);
        if (odd_fp)  fclose(odd_fp);
        return EXIT_FAILURE;
    }

    size_t even_count = 0, odd_count = 0;
    printf("\nProcessing %d integer(s):\n", n);

    for (int i = 0; i < n; i++) {
        int num;
        printf("[%2d/%d] > ", i + 1, n);
        if (!read_integer("", &num)) {
            fprintf(stderr, "\nError: Invalid input. Aborting.\n");
            goto cleanup;
        }

        FILE *target = (num % 2 == 0) ? even_fp : odd_fp;
        int written = fprintf(target, "%d\n", num);
        if (check_write(written, (num % 2 == 0) ? EVEN_FILE : ODD_FILE) != 0) {
            goto cleanup;
        }

        if (num % 2 == 0) even_count++;
        else odd_count++;
    }

cleanup:
    if (fclose(even_fp) != 0) {
        fprintf(stderr, "Warning: Error flushing/writing '%s' on close.\n", EVEN_FILE);
    }
    if (fclose(odd_fp) != 0) {
        fprintf(stderr, "Warning: Error flushing/writing '%s' on close.\n", ODD_FILE);
    }

    if (even_count + odd_count == n) {
        printf("\n========================================\n");
        printf("             SUCCESS\n");
        printf("========================================\n");
        printf("Total processed : %zu\n", (size_t)n);
        printf("Even in %-12s: %zu\n", EVEN_FILE, even_count);
        printf("Odd  in %-12s: %zu\n", ODD_FILE, odd_count);
        printf("========================================\n");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
