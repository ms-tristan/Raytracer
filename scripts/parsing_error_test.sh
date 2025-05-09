#!/bin/bash

# Parsing Error Test Script for Raytracer
# This script runs the raytracer with all the test scenes designed 
# to trigger different types of exceptions

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Path to the raytracer executable
RAYTRACER="../raytracer"
# Directory containing test scenes
SCENES_DIR="../scenes"

# If we're in the scripts directory, the path is different
if [ -f "./parsing_error_test.sh" ]; then
    RAYTRACER="../raytracer"
    SCENES_DIR="../scenes"
else
    RAYTRACER="./raytracer"
    SCENES_DIR="./scenes"
fi

# Make sure raytracer executable exists
if [ ! -f "$RAYTRACER" ]; then
    echo -e "${RED}Error: Raytracer executable not found at $RAYTRACER${NC}"
    echo "Make sure you've built the project and are running this script from the correct directory."
    exit 1
fi

# Test scenes that should trigger exceptions
TEST_SCENES=(
    "test_invalid_value_range.cfg"
    "test_missing_field.cfg"
    "test_import_nonexistent.cfg"
    "test_unknown_primitive.cfg"
    "test_invalid_operation.cfg"
)

# Display banner
echo -e "${BLUE}==============================================${NC}"
echo -e "${BLUE}     Raytracer Exception Handling Tests      ${NC}"
echo -e "${BLUE}==============================================${NC}"
echo ""

# Track number of tests that showed expected behavior
EXPECTED_COUNT=0
# Track total number of tests
TOTAL_TESTS=0

# Run each test scene
for scene in "${TEST_SCENES[@]}"; do
    TOTAL_TESTS=$((TOTAL_TESTS+1))
    
    # Full path to the scene file
    scene_path="$SCENES_DIR/$scene"
    
    # Check if the scene file exists
    if [ ! -f "$scene_path" ]; then
        echo -e "${RED}Error: Test scene not found: $scene_path${NC}"
        continue
    fi
    
    echo -e "${YELLOW}Testing scene: $scene${NC}"
    
    # Extract expected exception type from scene file (from the first line comment)
    expected_exception=$(head -n 1 "$scene_path" | sed -nE 's/.*for ([A-Za-z]+Exception).*/\1/p')

    if [ -z "$expected_exception" ]; then
        echo -e "${RED}Error: Could not extract expected exception from $scene_path${NC}"
        echo -e "${RED}  Ensure the first line of the file contains the format: '... for [ExceptionType]Exception'${NC}"
        continue
    fi

    echo -e "${PURPLE}Expected exception: $expected_exception${NC}"
    
    # Run the raytracer with the test scene
    echo -e "${BLUE}Running: $RAYTRACER $scene_path${NC}"
    echo -e "${BLUE}-----------------------------------------${NC}"
    
    output=$($RAYTRACER "$scene_path" 2>&1)
    exit_code=$?
    
    echo "$output"
    echo -e "${BLUE}-----------------------------------------${NC}"
    
    # Check if the program returned with error code 84 as expected
    if [ "$exit_code" -eq 84 ]; then
        echo -e "${GREEN}✓ Test passed: Program exited with code 84 as expected${NC}"
        EXPECTED_COUNT=$((EXPECTED_COUNT+1))
    else
        echo -e "${RED}✗ Test failed: Program exited with code $exit_code (expected 84)${NC}"
    fi
    
    # Check if the exception type appears in the output
    if echo "$output" | grep -q "$expected_exception" || echo "$output" | grep -q "Exception"; then
        echo -e "${GREEN}✓ Test passed: Output contains exception information${NC}"
    else
        echo -e "${RED}✗ Test failed: Output does not contain exception information${NC}"
        echo -e "${RED}  Expected to see '$expected_exception' in the output${NC}"
    fi
    
    echo ""
done

# Show summary
echo -e "${BLUE}==============================================${NC}"
echo -e "${YELLOW}Summary: $EXPECTED_COUNT out of $TOTAL_TESTS tests showed expected behavior${NC}"
echo -e "${BLUE}==============================================${NC}"

# Return success if all tests showed expected behavior, failure otherwise
if [ "$EXPECTED_COUNT" -eq "$TOTAL_TESTS" ]; then
    exit 0
else
    exit 1
fi
