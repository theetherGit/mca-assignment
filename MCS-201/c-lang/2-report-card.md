## Assumptions:

- Class Size: Exactly 20 students as specified in the problem statement.
- Curriculum: Five standard CBSE Class VIII subjects are fixed: English, Hindi, Mathematics, Science, Social Science.
- Passing Criterion: A student must secure ≥33% average per subject across 4 terms to pass that subject.
- Roll Numbers: Automatically assigned as 1 to 20 to prevent duplicate/invalid entries and ensure uniqueness.
- Grading Scale: Simplified CBSE 9-point grade band (A1 to E) based on overall percentage.
- Promotion Status:
  - PASSED → 0 subjects failed
  - COMPARTMENT → 1 subject failed
  - REPEAT YEAR → ≥2 subjects failed or overall percentage <33%
- Input Handling: Names are read as single tokens (no spaces) for standard C scanf compatibility. Marks are strictly validated between 0.0 and 100.0.
- Memory Management: All arrays are statically allocated on the stack. No dynamic memory (malloc/free) is used, guaranteeing automatic cleanup and zero memory leaks.

## Algorithm

START

1. Define Structure 'Student' containing:
   - rollNumber (int), studentName (string)
   - termMarks[4][5] (float: 4 terms × 5 subjects)
   - totalMarks, overallPercentage (float)
   - failedSubjectCount (int), finalGrade, promotionStatus (strings)

2. Declare array: Student classVIII[20]

3. FOR i = 0 to 19:


    - Assign classVIII[i].rollNumber = i + 1
    - Input classVIII[i].studentName
    - Initialize totalMarks = 0, failedSubjectCount = 0
    - FOR each subject j = 0 to 4:
      - i. subjectSum = 0
      - ii. FOR each term t = 0 to 3:
        - Prompt and read marks
        - IF marks < 0 OR marks > 100 → Reject & repeat input
        - Add marks to subjectSum
      - iii. IF (subjectSum / 4) < 33.0 → Increment failedSubjectCount
      - iv. Add subjectSum to totalMarks

    - Calculate overallPercentage = totalMarks / (4 × 5)
    - Assign finalGrade & promotionStatus using CBSE rules
    - Print confirmation message

4. DISPLAY Class Summary Table:
   Columns: Roll, Name, Overall %, Promotion Status
   Print all 20 student records in tabular format

5. LOOP:
  - Prompt user for searchRoll. IF 0 → EXIT loop
  - Search array for matching rollNumber
  - IF found: - DISPLAY Detailed Progress Report - Show subject-wise term marks, averages, overall %, grade & status
  - ELSE → DISPLAY "Roll not found in records"
6. END LOOP

7. Program terminates. Stack memory is automatically reclaimed.
   END
