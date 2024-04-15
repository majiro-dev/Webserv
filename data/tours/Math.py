
import sys

def add_numbers(num1, num2):
    return num1 + num2

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script_name.py num1 num2")
    else:
        try:
            num1 = int(sys.argv[1])
            num2 = int(sys.argv[2])
            result = add_numbers(num1, num2)
            print("Sum:", result)
        except ValueError:
            print("Please enter valid numbers.")
