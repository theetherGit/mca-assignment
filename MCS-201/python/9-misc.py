import os
import string
from collections import Counter

# Resolve absolute path to "sample-data.txt" in the same directory as this script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DEFAULT_FILE = os.path.join(SCRIPT_DIR, "data.txt")


# Helper: Auto-create sample file for seamless testing
def setup_sample_file():
    if not os.path.exists(DEFAULT_FILE):
        with open(DEFAULT_FILE, "w", encoding="utf-8") as f:
            f.write(
                "Python programming is fun and powerful.\n"
                "File handling makes data processing easy.\n"
                "Lambda functions simplify code structure.\n"
                "Always validate user inputs properly.\n"
                "Python is widely used in academia and industry.\n"
            )
        print(f"✔ Auto-created '{DEFAULT_FILE}' with sample data.\n")


# =========================================================
# TASK 1: Cube of numbers using lambda function
# =========================================================
def task1_lambda_cubes():
    print("[1] Cube of Numbers using Lambda")
    numbers = [2, 3, 4, 5, 6]  # Predefined list (no user input)
    # map() applies lambda to each element; list() materializes the iterator
    cubes = list(map(lambda x: x**3, numbers))
    print(f"   Original: {numbers}")
    print(f"   Cubes   : {cubes}\n")


# =========================================================
# TASK 2: Word frequency in a file
# =========================================================
def task2_word_frequency():
    print(f"[2] Word Frequency in '{DEFAULT_FILE}'")
    try:
        with open(DEFAULT_FILE, "r", encoding="utf-8") as f:
            text = f.read().lower()
            # Remove punctuation for clean word boundaries
            translator = str.maketrans("", "", string.punctuation)
            words = text.translate(translator).split()

        if not words:
            print("   ⚠️ File is empty.\n")
            return

        # Counter efficiently tallies word occurrences
        freq = Counter(words)
        print("   Word | Frequency")
        print("   -----------------")
        for word, count in freq.most_common():
            print(f"   {word:<15} | {count}")
        print()
    except Exception as e:
        print(f"   ❌ Error: {e}\n")


# =========================================================
# TASK 3: First N lines from file (USER INPUT HERE)
# =========================================================
def task3_first_n_lines():
    print("[3] First N Lines from File")
    try:
        n = int(input("   Enter number of lines (n): "))
        if n <= 0:
            print("   ⚠️ n must be a positive integer.\n")
            return
    except ValueError:
        print("   ⚠️ Invalid input. Please enter an integer.\n")
        return

    try:
        with open(DEFAULT_FILE, "r", encoding="utf-8") as f:
            print(f"   --- First {n} line(s) ---")
            i = 0
            for i, line in enumerate(f, 1):
                if i > n:
                    break
                print(f"   {i}: {line.rstrip()}")
            if i < n:
                print(f"   ⚠️ File only contained {i} lines.\n")
            else:
                print()
    except FileNotFoundError:
        print(f"   ❌ '{DEFAULT_FILE}' not found.\n")


# =========================================================
# TASK 4: File size in bytes
# =========================================================
def task4_file_size():
    print(f"[4] Size of '{DEFAULT_FILE}' in Bytes")
    try:
        # os.path.getsize() returns exact byte count without opening the file
        size_bytes = os.path.getsize(DEFAULT_FILE)
        print(f"   📁 {size_bytes} bytes\n")
    except FileNotFoundError:
        print(f"   ❌ '{DEFAULT_FILE}' not found.\n")


# =========================================================
# MAIN EXECUTION
# =========================================================
if __name__ == "__main__":
    print("=== Python File & Lambda Operations ===")
    setup_sample_file()
    task1_lambda_cubes()
    task2_word_frequency()
    task3_first_n_lines()  # <-- ONLY TASK WITH USER INPUT
    task4_file_size()
    print("✔ All tasks completed.")
