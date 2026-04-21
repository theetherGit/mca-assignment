# Import functions from the Area package using aliases to avoid naming conflicts
from Area.circle import area as circle_area
from Area.rectangle import area as rectangle_area
from Area.square import area as square_area


def get_positive_float(prompt):
    """Helper: Repeatedly prompts until a valid positive float is entered."""
    while True:
        try:
            value = float(input(prompt))
            if value <= 0:
                print("  ⚠️ Value must be greater than 0. Try again.")
                continue
            return value
        except ValueError:
            print("  ⚠️ Invalid input. Please enter a numeric value.")


def main():
    print("=== Modular Area Calculator ===\n")

    # Square
    side = get_positive_float("Enter side of square: ")
    print(f"▶ Square Area  : {square_area(side):.2f} sq units\n")

    # Circle
    radius = get_positive_float("Enter radius of circle: ")
    print(f"▶ Circle Area  : {circle_area(radius):.2f} sq units\n")

    # Rectangle
    length = get_positive_float("Enter length of rectangle: ")
    width = get_positive_float("Enter width of rectangle: ")
    print(f"▶ Rectangle Area: {rectangle_area(length, width):.2f} sq units\n")


if __name__ == "__main__":
    main()
