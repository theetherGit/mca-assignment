# Algorithm

### **Phase 1: Initialization and Setup**
1.  **Start.**
2.  **Define Constants**: Set the number of terms ($MAX\_TERMS = 4$).
3.  **Setup Environment**:
    * Input the total number of students ($n$).
    * Choose between **Default Subjects** (English, Hindi, Math, Science, Social) or **Custom Subjects**.
    * If custom, input the number of subjects ($m$) and their names.
4.  **Allocate Memory**: Dynamically allocate memory for student records and subject-specific marks.

---

### **Phase 2: Data Collection & Calculation**
5.  **For each Student ($i=1$ to $n$):**
    * **A. Identify**: Input Name and unique Roll Number (Validate for positive integers).
    * **B. Collect Marks**: 
        * For each Subject ($j=1$ to $m$):
            * Input marks for Term 1, 2, 3, and 4.
            * Validate that each input is between $0$ and $100$.
            * Calculate $\text{Subject Sum} = \sum (\text{Term Marks})$.
            * Calculate $\text{Subject Average} = \frac{\text{Subject Sum}}{MAX\_TERMS}$.
    * **C. Evaluate Pass Criteria**:
        * If $\text{Subject Average} < 33.0$, increment `total_failed_subs` for this student.
        * Keep track of the index of the failed subject for the final report.
    * **D. Aggregate**:
        * $\text{Grand Total} = \sum (\text{All Subject Sums})$.
        * $\text{Overall Percentage} = \frac{\text{Grand Total}}{m \times MAX\_TERMS}$.

---

### **Phase 3: Result & Promotion Logic**
6.  **Determine Status** (Based on CBSE Promotion Policy):
    * **If** `total_failed_subs == 0`: Status = **PASSED**.
    * **Else If** `total_failed_subs == 1`: Status = **COMPARTMENT** (Eligible for re-exam).
    * **Else**: Status = **ESSENTIAL REPEAT** (Must repeat the year).
7.  **Assign Final Grade**: 
    * If Status is "PASSED", use the 8-point scale (A1 to D) based on the overall percentage.
    * If Status is "COMPARTMENT" or "REPEAT", assign Grade **"E"**.



---

### **Phase 4: Reporting & Interaction**
8.  **Display Summary Dashboard**: 
    * Print a table showing Roll No, Name, Percentage, and Status for all students to provide an immediate overview.
9.  **Initiate Search Loop**:
    * Prompt user for a **Roll Number**.
    * If Roll Number exists:
        * Generate a detailed **Grade Card**.
        * Display individual term marks, subject-wise totals, and subject-wise grades.
        * Mark failed subjects with a visual indicator (e.g., `!`).
        * Display specific **Remarks** based on the promotion status.
    * If Roll Number is **0**, exit the loop.
10. **Memory Cleanup**: Free all dynamically allocated memory (subject names, marks arrays, and student structures).
11. **End.**
