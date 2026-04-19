#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERMS 4
#define MAX_NAME_LEN 20

typedef struct {
  float *marks;
} Term;

typedef struct {
  char name[50];
  int roll_no;
  Term terms[MAX_TERMS];
  float grand_total;
  float percentage;
  char grade[3];
  char status[20];
  int *failed_subject_indices;
  int total_failed_subs;
} Student;

void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void printHeader(const char *title) {
  printf("\n==================================================================="
         "=======\n");
  printf("    %s\n", title);
  printf("====================================================================="
         "=====\n");
}

const char *getCBSEGrade(float per, int failed_count) {
  // If student has failed subjects (Compartment or ER), the overall grade is E
  if (failed_count > 0 || per < 33.0)
    return "E";
  if (per >= 91.0)
    return "A1";
  if (per >= 81.0)
    return "A2";
  if (per >= 71.0)
    return "B1";
  if (per >= 61.0)
    return "B2";
  if (per >= 51.0)
    return "C1";
  if (per >= 41.0)
    return "C2";
  if (per >= 33.0)
    return "D";
  return "E";
}

int main() {
  int n = 20, m = 5;
  char inputBuffer[10], choice;
  char(*subjects)[MAX_NAME_LEN];

  printHeader("CBSE PROMOTION & RESULT MANAGEMENT SYSTEM");

  printf(" >> Number of students [Default 20]: ");
  if (fgets(inputBuffer, sizeof(inputBuffer), stdin) &&
      inputBuffer[0] != '\n') {
    n = atoi(inputBuffer);
  }

  while (1) {
    printf(" >> Use default subjects (Eng, Hin, Mat, Sci, SSt)? (Y/N): ");
    scanf(" %c", &choice);
    clearBuffer();
    choice = toupper(choice);
    if (choice == 'Y' || choice == 'N')
      break;
    printf("    [!] Invalid. Enter Y/N.\n");
  }

  if (choice == 'N') {
    printf(" >> How many subjects? ");
    if (scanf("%d", &m) != 1) {
      m = 5;
      clearBuffer();
    }
    subjects = malloc(m * MAX_NAME_LEN);
    for (int i = 0; i < m; i++) {
      printf("    Subject %d Name: ", i + 1);
      scanf("%s", subjects[i]);
    }
  } else {
    m = 5;
    subjects = malloc(5 * MAX_NAME_LEN);
    char *defaults[] = {"English", "Hindi", "Math", "Science",
                        "Social Science"};
    for (int i = 0; i < 5; i++)
      strcpy(subjects[i], defaults[i]);
  }

  Student *s = (Student *)malloc(n * sizeof(Student));

  // --- PHASE 1: DATA ENTRY ---
  for (int i = 0; i < n; i++) {
    printf("\n\n**********************************************************");
    printf("\n  STUDENT No. %d INFORMATION (Student %d of %d)", i + 1, i + 1,
           n);
    printf("\n**********************************************************\n");

    printf(" >> Enter Name    : ");
    scanf("%s", s[i].name);
    while (1) {
      printf(" >> Enter Roll No : ");
      if (scanf("%d", &s[i].roll_no) == 1 && s[i].roll_no > 0)
        break;
      else {
        printf("    [!] Invalid Roll. Try again.\n");
        clearBuffer();
      }
    }

    s[i].failed_subject_indices = calloc(m, sizeof(int));
    s[i].grand_total = 0;
    s[i].total_failed_subs = 0;

    for (int t = 0; t < MAX_TERMS; t++) {
      s[i].terms[t].marks = malloc(m * sizeof(float));
    }

    printf("\n  STEP 2: SUBJECT-WISE PERFORMANCE");
    for (int j = 0; j < m; j++) {
      float sub_sum = 0;
      printf("\n\n [ SUBJECT: %s ]\n ----------------------------\n",
             subjects[j]);
      for (int t = 0; t < MAX_TERMS; t++) {
        while (1) {
          printf("  >> Term %d Marks (0-100): ", t + 1);
          if (scanf("%f", &s[i].terms[t].marks[j]) == 1 &&
              s[i].terms[t].marks[j] >= 0 && s[i].terms[t].marks[j] <= 100)
            break;
          else {
            printf("     [!] Error: Enter 0-100.\n");
            clearBuffer();
          }
        }
        sub_sum += s[i].terms[t].marks[j];
      }

      // CBSE Pass Logic: Must average 33% in each subject
      if ((sub_sum / MAX_TERMS) < 33.0) {
        s[i].failed_subject_indices[j] = 1;
        s[i].total_failed_subs++;
      }
      s[i].grand_total += sub_sum;
    }

    s[i].percentage = s[i].grand_total / (MAX_TERMS * m);

    // Promotion Logic
    if (s[i].total_failed_subs == 0)
      strcpy(s[i].status, "PASSED");
    else if (s[i].total_failed_subs == 1)
      strcpy(s[i].status, "COMPARTMENT");
    else
      strcpy(s[i].status, "REPEAT YEAR");

    strcpy(s[i].grade, getCBSEGrade(s[i].percentage, s[i].total_failed_subs));
    printf("\n[✔] Data for %s processed.\n", s[i].name);
  }

  // --- PHASE 2: SUMMARY DASHBOARD ---
  printHeader("ACADEMIC DASHBOARD: SUMMARY VIEW");
  printf("| %-6s | %-15s | %-10s | %-15s |\n", "ROLL", "NAME", "PERCENT %",
         "STATUS");
  printf("|--------|-----------------|------------|-----------------|\n");
  for (int i = 0; i < n; i++) {
    printf("| %-6d | %-15s | %-10.2f | %-15s |\n", s[i].roll_no, s[i].name,
           s[i].percentage, s[i].status);
  }

  // --- PHASE 3: SEARCH FOR DETAILED REPORT ---
  int searchRoll;
  while (1) {
    printf("\n >> Enter Roll No for Detailed Grade Card (0 to Exit): ");
    if (scanf("%d", &searchRoll) != 1) {
      clearBuffer();
      continue;
    }
    if (searchRoll == 0)
      break;

    int found = 0;
    for (int i = 0; i < n; i++) {
      if (s[i].roll_no == searchRoll) {
        found = 1;
        printHeader("OFFICIAL PROGRESS REPORT");
        printf(" Name: %-20s Roll No: %-10d Status: %s\n", s[i].name,
               s[i].roll_no, s[i].status);
        printf("---------------------------------------------------------------"
               "-----------\n");
        printf(" %-12s | %-6s | %-6s | %-6s | %-6s | %-7s | %-5s\n", "SUBJECT",
               "T1", "T2", "T3", "T4", "TOTAL", "GRD");
        printf("---------------------------------------------------------------"
               "-----------\n");
        for (int j = 0; j < m; j++) {
          float sub_t = 0;
          // Tag failed subjects with '!'
          printf(" %-11s%c |", subjects[j],
                 s[i].failed_subject_indices[j] ? '!' : ' ');
          for (int t = 0; t < MAX_TERMS; t++) {
            printf(" %-6.1f |", s[i].terms[t].marks[j]);
            sub_t += s[i].terms[t].marks[j];
          }
          printf(" %-7.1f | %-5s\n", sub_t, getCBSEGrade(sub_t / MAX_TERMS, 0));
        }
        printf("---------------------------------------------------------------"
               "-----------\n");
        printf(" FINAL PERCENTAGE: %.2f%%    FINAL GRADE: %s\n",
               s[i].percentage, s[i].grade);

        // Detailed Remarks based on CBSE guidelines
        if (strcmp(s[i].status, "COMPARTMENT") == 0) {
          printf(" REMARK: Eligible for Compartment Exam in subject(s) marked "
                 "'!'.\n");
        } else if (strcmp(s[i].status, "REPEAT YEAR") == 0) {
          printf(" REMARK: Essential Repeat. Student must repeat the grade "
                 "level.\n");
        } else {
          printf(" REMARK: Successfully Promoted to the next Class.\n");
        }
        printf("==============================================================="
               "===========\n");
        break;
      }
    }
    if (!found)
      printf(" [!] Roll No %d not found in records.\n", searchRoll);
  }

  // Cleanup
  for (int i = 0; i < n; i++) {
    for (int t = 0; t < MAX_TERMS; t++)
      free(s[i].terms[t].marks);
    free(s[i].failed_subject_indices);
  }
  free(s);
  free(subjects);
  return 0;
}
