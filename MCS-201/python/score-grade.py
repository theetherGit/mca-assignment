def get_grade():
    """Get grade for the use input."""
    try:
        # Prompt the user for input
        score_input = input("Enter a score between 0.0 and 1.0: ")
        score = float(score_input)

        # Check if the score is out of range
        if score < 0.0 or score > 1.0:
            print("Error: Score is out of range.")
        else:
            # Determine the grade based on the score
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

            print(f"Grade: {grade}")

    except ValueError:
        # Handle cases where the input is not a valid number
        print("Error: Invalid input. Please enter a numeric value.")


# Run the function
if __name__ == "__main__":
    get_grade()
