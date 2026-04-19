# main.py

from area import circle, rectangle, square


def main():
    print("--- Area Calculator Demo ---")

    # Square
    s_side = 5.0
    print(f"Square (side={s_side}): {square.area(s_side):.2f}")

    # Circle
    c_radius = 3.0
    print(f"Circle (radius={c_radius}): {circle.area(c_radius):.2f}")

    # Rectangle
    r_length, r_width = 4.0, 6.0
    print(f"Rectangle ({r_length}x{r_width}): {rectangle.area(r_length, r_width):.2f}")


if __name__ == "__main__":
    main()
