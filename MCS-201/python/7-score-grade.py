def get_grade():
    """Reads a score, validates it, and prints the corresponding grade."""
    while True:  # Loop until valid input & grading is complete
        try:
            # Step 1: Prompt & convert input to float
            score_input = input("Enter a score between 0.0 and 1.0: ")
            score = float(score_input)

            # Step 2: Validate range (inclusive 0.0 to 1.0)
            if not (0.0 <= score <= 1.0):
                print(
                    "Error: Score out of range. Please enter a value between 0.0 and 1.0.\n"
                )
                continue  # Restart loop for new input

            # Step 3: Map score to grade using descending thresholds
            if score >= 0.9:
                grade = "A"
            elif score >= 0.8:
                grade = "B"
            elif score >= 0.7:
                grade = "C"
            elif score >= 0.6:
                grade = "D"
            else:
                grade = "F"

            # Step 4: Display result & exit loop
            print(f"Grade: {grade}")
            break  # Successful execution → exit validation loop
        except ValueError:
            # Catches non-numeric inputs like "abc", "0.8x", or empty strings
            print("Error: Invalid input. Please enter a numeric value.\n")


# Standard Python entry point
if __name__ == "__main__":
    get_grade()
