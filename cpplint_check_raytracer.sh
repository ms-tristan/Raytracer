#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <folder_path> <output_directory>"
    exit 1
fi

FOLDER_PATH="$1"
OUTPUT_DIR="$2"

if [ ! -d "$FOLDER_PATH" ]; then
    echo "Error: Folder '$FOLDER_PATH' does not exist."
    exit 1
fi

if [ "$FOLDER_PATH" = "." ]; then
    FOLDER_PATH=$(pwd)
fi

if [ "$OUTPUT_DIR" = "." ]; then
    OUTPUT_DIR=$(pwd)
fi

if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
    if [ $? -ne 0 ]; then
        echo "Error: Could not create output directory '$OUTPUT_DIR'."
        exit 1
    fi
fi

OUTPUT_FILE="$OUTPUT_DIR/cpp-lint-reports.log"

> "$OUTPUT_FILE"

FILTER="--filter=-legal/copyright,-whitespace/indent,-whitespace/line_length,-build/include_subdir,-whitespace/parens"
LINELENGTH="--linelength=100"

find "$FOLDER_PATH" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.cc" \) -print0 | while IFS= read -r -d '' file; do
    LINT_OUTPUT=$(cpplint "$FILTER" "$LINELENGTH" "$file" 2>&1)
    if [[ "$LINT_OUTPUT" == *":"* ]]; then
        echo "$LINT_OUTPUT" | grep -v "Done processing" | grep -v "Total errors found" >> "$OUTPUT_FILE"
    fi
done

ERROR_COUNT=$(grep -c ":" "$OUTPUT_FILE" || echo 0)

echo "CPPlint check completed."
echo "$ERROR_COUNT errors found."
echo "Report saved to: $OUTPUT_FILE"
