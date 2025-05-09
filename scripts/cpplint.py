import os
import sys
import subprocess

EXCLUDED_RULES = [
    "-whitespace/indent",  # Exclude indentation in a namespace
    "-legal/copyright",  # Exclude copyright message checks
    "-whitespace/line_length",  # Exclude 80 columns line length check
    "-build/include_subdir",  # Exclude include subdirectory warnings
    "-readability/namespace",  # Exclude namespace readability warnings
    "-whitespace/comments",
    "-build/header_guard",
    "-whitespace/parens",
    "-build/c++11",
    "-runtime/references"
]

def run_cpplint(directory):
    """Run cpplint recursively on the specified directory."""
    excluded_rules_arg = f"--filter={','.join(EXCLUDED_RULES)}"
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(('.cpp', '.hpp')):
                filepath = os.path.join(root, file)
                try:
                    subprocess.run(
                        ['cpplint', excluded_rules_arg, filepath],
                        check=True,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE
                    )
                except subprocess.CalledProcessError as e:
                    print(e.stderr.decode())

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 cpplint.py <directory>")
        sys.exit(1)

    target_directory = sys.argv[1]
    if not os.path.isdir(target_directory):
        print(f"Error: {target_directory} is not a valid directory.")
        sys.exit(1)

    run_cpplint(target_directory)
