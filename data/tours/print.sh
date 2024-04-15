#!/bin/bash

# Check if no argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <string>"
    exit 1
fi

# Print the provided string
echo "The provided string is: $1"
