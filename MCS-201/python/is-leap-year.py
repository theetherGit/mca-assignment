# =============================================================================
# Program: Leap Year Checker
# Description: Determines if a user-entered year is a leap year using
#              Gregorian calendar rules. Includes input validation & comments.
# =============================================================================


def check_leap_year():
    """Determine whether a given year is a leap year."""
    while True:  # Loop until valid input is processed
        try:
            # Step 1: Get user input and safely convert to integer
            year = int(input("Enter a year to check: "))

            # Step 2: Apply leap year logic in priority order
            # Rule 1: Centurial years divisible by 400 ARE leap years (e.g., 2000)
            if year % 400 == 0:
                is_leap = True
            # Rule 2: Centurial years NOT divisible by 400 are NOT leap years (e.g., 1900)
            elif year % 100 == 0:
                is_leap = False
            # Rule 3: Non-centurial years divisible by 4 ARE leap years (e.g., 2024)
            elif year % 4 == 0:
                is_leap = True
            # Rule 4: All remaining years are NOT leap years
            else:
                is_leap = False

            # Step 3: Display result and exit the loop
            if is_leap:
                print(f"✅ {year} is a Leap Year.")
            else:
                print(f"❌ {year} is NOT a Leap Year.")
            break  # Exit while loop after successful processing

        except ValueError:
            # Catches inputs like "abc", "20.5", or empty entries
            print("⚠️ Invalid input. Please enter a valid integer year.")


# Standard Python entry point
if __name__ == "__main__":
    check_leap_year()
