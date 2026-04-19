# ruff: noqa: T201

import os
from itertools import islice

# Resolve absolute path to "sample-data.txt" in the same directory as this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
FILE_PATH = os.path.join(SCRIPT_DIR, "sample-data.txt")


# 1. Find cube of numbers in a list using lambda function
def cube_numbers_lambda(numbers):
    """Return a list containing cubes of the input numbers using lambda."""
    return list(map(lambda x: x**3, numbers))  # noqa: C417


# 2. Display frequency of each word in a file
def word_frequency(filepath):
    """Return a dictionary with word frequencies from the given file."""
    try:
        word_freq: dict[str, int] = {}  # Dictionary to store word:count pairs
        with open(filepath, encoding="utf-8") as file:
            # Iterating over the file object directly reads
            # line-by-line (Memory Efficient)
            for line in file:
                # Process one line at a time
                words = line.lower().split()
                for word in words:
                    word_freq[word] = word_freq.get(word, 0) + 1
        return word_freq
    except Exception as e:
        print(f"⚠️ Error reading file for word frequency: {e}")  # noqa: C417
        return None


# 3. Display first n lines from a file (n given by user)
def first_n_lines(filepath, n):
    """Return the first n lines from the file."""
    if n < 1:
        print("⚠️ Warning: n must be a positive integer.")
        return []
    try:
        with open(filepath, encoding="utf-8") as f:
            # islice reads exactly n lines without loading the whole file into memory
            return list(islice(f, n))
    except Exception as e:
        print(f"⚠️ Error reading lines: {e}")
        return None


# 4. Display size of a file in bytes
def file_size_bytes(filepath):
    """Return the size of the file in bytes."""
    try:
        return os.path.getsize(filepath)
    except Exception as e:
        print(f"⚠️ Error getting file size: {e}")
        return None


# ==========================
# MAIN EXECUTION
# ==========================
if __name__ == "__main__":
    print("=== 1. Cube Numbers Using Lambda ===")
    numbers = [1, 2, 3, 4, 5]
    print(f"Original list: {numbers}")
    print(f"Cubed list:    {cube_numbers_lambda(numbers)}\n")

    print("=== File Operations ===")
    print(f"📁 Target file: {FILE_PATH}")

    if not os.path.exists(FILE_PATH):
        print("❌ Error: 'sample-data.txt' not found in the script directory.")
        print("   Please place the file in the same folder as this Python script.")
    elif not os.access(FILE_PATH, os.R_OK):
        print("❌ Error: No read permission for 'sample-data.txt'.")
    else:
        # Task 2: Word Frequency
        print("\n--- 2. Word Frequency ---")
        freq = word_frequency(FILE_PATH)
        if freq:
            print(f"📊 {len(freq)} unique words found. Sorted by frequency:")
            for word, count in sorted(freq.items(), key=lambda x: x[1], reverse=True):
                print(f"  {word}: {count}")

        # Task 3: First n Lines
        print("\n--- 3. First n Lines ---")
        try:
            n = int(input("🔢 Enter number of lines to display (n): "))
            lines = first_n_lines(FILE_PATH, n)
            if lines is not None:
                print(f"\n📄 First {n} line(s):")
                for line in lines:
                    print(f"  {line.rstrip()}")
        except ValueError:
            print("❌ Invalid input. Please enter a whole number.")

        # Task 4: File Size
        print("\n--- 4. File Size ---")
        size = file_size_bytes(FILE_PATH)
        if size is not None:
            print(f"📏 File size: {size:,} bytes ({size / 1024:.2f} KB)")

    print("\n✅ Program completed successfully.")
