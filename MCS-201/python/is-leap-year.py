def is_leap_year(year: int) -> bool:
    """Determine whether a given year is a leap year.

    Leap Year Rules (Gregorian Calendar):
    1. A year divisible by 4 is a leap year.
    2. EXCEPT if it's divisible by 100, it's NOT a leap year.
    3. UNLESS it's also divisible by 400, then it IS a leap year.

    Args:
        year (int): The year to evaluate.

    Return:
        bool: True if the year is a leap year, False otherwise.

    """
    # Step 1: Check if the year is divisible by 4
    if year % 4 == 0:
        # Step 2: If divisible by 100, apply the 400-year exception rule
        if year % 100 == 0:
            # Divisible by both 100 and 400 -> Leap Year
            return year % 400 == 0
        # Divisible by 4 but NOT by 100 -> Leap Year
        return True
    # Not divisible by 4 -> Not a Leap Year
    return False


def main():
    """Handle user input, validation, and displays the result."""
    try:
        # Prompt the user to enter a year
        user_input = input("Enter a year to check: ")
        year = int(user_input)

        # Basic validation: years must be positive in the Gregorian calendar
        if year <= 0:
            print("Please enter a valid positive year.")
            return

        # Check leap year status and display the appropriate message
        if is_leap_year(year):
            print(f"✅ {year} is a leap year.")
        else:
            print(f"❌ {year} is not a leap year.")

    except ValueError:
        # Catches cases where the input cannot be converted to an integer
        print("⚠️ Invalid input. Please enter a numeric year.")


# Standard Python practice: ensures the script runs only when executed directly
if __name__ == "__main__":
    main()

    # import calendar
    # print(calendar.isleap(2024))
