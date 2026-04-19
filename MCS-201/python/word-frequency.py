try:
    word_freq = {}  # Dictionary to store word:count pairs
    with open(filepath, "r", encoding="utf-8") as file:
        for line in file:
            # Convert line to lowercase and split by whitespace into words
            words = line.lower().split()

            for word in words:
                # Manual cleaning of common punctuation without using regex
                # We strip characters from the edges of the word
                clean_word = word.strip(".,!?;:()[]\"'")

                print("word", word, clean_word)

                if clean_word:  # Ensure we don't count empty strings
                    if clean_word in word_freq:
                        # If word exists, increment the count
                        word_freq[clean_word] += 1
                    else:
                        # If word is new, initialize it at 1
                        word_freq[clean_word] = 1

    print("--- Word Frequency Results ---")
    # To display them in a sorted manner manually:
    # Sort the dictionary items by value (count) in descending order
    sorted_words = sorted(word_freq.items(), key=lambda item: item[1], reverse=True)

    for word, count in sorted_words[:5]:  # Display top 5
        print(f"'{word}': {count}")
    print()
except Exception as e:
    print(f"⚠️ Error reading file for word frequency: {e}")
    return None
