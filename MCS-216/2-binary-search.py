def binary_search(arr, low, high, target):
    if high >= low:
        mid = low + (high - low) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] > target:
            return binary_search(arr, low, mid - 1, target)
        else:
            return binary_search(arr, mid + 1, high, target)
    return -1


if __name__ == "__main__":
    # Assignment Data
    arr = [10, 35, 40, 45, 50, 55, 60, 65, 70, 100]
    idx = binary_search(arr, 0, len(arr) - 1, 100)
    print(f"Element found at index: {idx}" if idx != -1 else "Not found")
