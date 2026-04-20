#include <stdio.h>
#include <string.h>

#define TOTAL_STUDENTS 2
#define TOTAL_SUBJECTS 5
#define TOTAL_TERMS 4

const char *subjectNames[TOTAL_SUBJECTS] = {"English", "Hindi", "Mathematics",
                                            "Science", "Social Science"};

struct Student {
  int rollNumber;
  char studentName[50];
  float termMarks[TOTAL_TERMS][TOTAL_SUBJECTS];
  float totalMarks;
  float overallPercentage;
  int failedSubjectCount;
  char finalGrade[5];
  char promotionStatus[20];
};

// Clear leftover characters from input buffer
void clearInputBuffer() {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}

// Assign CBSE Grade & Promotion Status
void assignGradeAndStatus(float percentage, int failedCount, char *grade,
                          char *status) {
  if (failedCount > 0) {
    strcpy(grade, "E");
    strcpy(status, (failedCount == 1) ? "Compartment" : "Repeat Year");
  } else if (percentage < 33.0) {
    strcpy(grade, "E");
    strcpy(status, "Repeat Year");
  } else {
    if (percentage >= 90.0)
      strcpy(grade, "A1");
    else if (percentage >= 80.0)
      strcpy(grade, "A2");
    else if (percentage >= 70.0)
      strcpy(grade, "B1");
    else if (percentage >= 60.0)
      strcpy(grade, "B2");
    else if (percentage >= 50.0)
      strcpy(grade, "C1");
    else if (percentage >= 40.0)
      strcpy(grade, "C2");
    else
      strcpy(grade, "D");
    strcpy(status, "Passed");
  }
}

int main() {
  struct Student students[TOTAL_STUDENTS];

  printf("=== CBSE Class VIII Progress Report System ===\n");
  printf("Defaults: %d Students | %d Terms | %d Subjects\n", TOTAL_STUDENTS,
         TOTAL_TERMS, TOTAL_SUBJECTS);
  printf("Roll numbers assigned automatically (1 to %d)\n\n", TOTAL_STUDENTS);

  // 1. DATA ENTRY
  for (int i = 0; i < TOTAL_STUDENTS; i++) {
    printf("Student %2d:\n", i + 1);
    students[i].rollNumber = i + 1; // Auto-assign roll
    printf("  Roll No : %d\n", students[i].rollNumber);

    printf("  Name    : ");
    scanf("%49s", students[i].studentName); // Safe string input

    students[i].totalMarks = 0.0f;
    students[i].failedSubjectCount = 0;

    for (int s = 0; s < TOTAL_SUBJECTS; s++) {
      float subjectTotal = 0.0f;
      printf("\n  [%s]\n", subjectNames[s]);

      for (int t = 0; t < TOTAL_TERMS; t++) {
        float mark;
        while (1) {
          printf("    Term %d Marks (0-100): ", t + 1);
          if (scanf("%f", &mark) == 1 && mark >= 0.0f && mark <= 100.0f) {
            break; // Valid mark entered
          }
          printf("    [!] Invalid. Enter a number between 0 and 100.\n");
          clearInputBuffer(); // Discard bad input
        }
        students[i].termMarks[t][s] = mark;
        subjectTotal += mark;
      }

      // CBSE Pass Rule: <33% subject average = fail
      if ((subjectTotal / TOTAL_TERMS) < 33.0) {
        students[i].failedSubjectCount++;
      }
      students[i].totalMarks += subjectTotal;
    }

    students[i].overallPercentage =
        students[i].totalMarks / (TOTAL_TERMS * TOTAL_SUBJECTS);
    assignGradeAndStatus(students[i].overallPercentage,
                         students[i].failedSubjectCount, students[i].finalGrade,
                         students[i].promotionStatus);
    printf("  >> Record saved.\n\n");
  }

  // 2. SUMMARY DASHBOARD
  printf("\n--- CLASS SUMMARY ---\n");
  printf("%-6s %-15s %-8s %-12s\n", "Roll", "Name", "%", "Status");
  printf("--------------------------------------------\n");
  for (int i = 0; i < TOTAL_STUDENTS; i++) {
    printf("%-6d %-15s %-8.2f %-12s\n", students[i].rollNumber,
           students[i].studentName, students[i].overallPercentage,
           students[i].promotionStatus);
  }

  // 3. DETAILED REPORT SEARCH
  int searchRoll;
  printf("\nEnter Roll No for Detailed Report (0 to exit): ");
  while (scanf("%d", &searchRoll) == 1 && searchRoll != 0) {
    int found = 0;
    for (int i = 0; i < TOTAL_STUDENTS; i++) {
      if (students[i].rollNumber == searchRoll) {
        found = 1;
        printf("\n=== PROGRESS REPORT: %s (Roll %d) ===\n",
               students[i].studentName, students[i].rollNumber);
        printf("Status: %s | Overall: %.2f%% | Grade: %s\n",
               students[i].promotionStatus, students[i].overallPercentage,
               students[i].finalGrade);
        printf("%-12s | %4s | %4s | %4s | %4s | Avg\n", "Subject", "T1", "T2",
               "T3", "T4");
        printf("------------------------------------------------\n");
        for (int s = 0; s < TOTAL_SUBJECTS; s++) {
          float subjectAvg = 0.0f;
          printf("%-12s |", subjectNames[s]);
          for (int t = 0; t < TOTAL_TERMS; t++) {
            printf(" %4.1f |", students[i].termMarks[t][s]);
            subjectAvg += students[i].termMarks[t][s];
          }
          printf(" %4.1f\n", subjectAvg / TOTAL_TERMS);
        }
        printf("------------------------------------------------\n");
        break;
      }
    }
    if (!found)
      printf("Roll %d not found in records.\n", searchRoll);
    printf("\nEnter Roll No for Detailed Report (0 to exit): ");
  }
  return 0;
}
