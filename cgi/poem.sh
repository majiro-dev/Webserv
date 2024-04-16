#!/bin/bash

# Check if exactly 4 arguments are passed
if [ "$#" -ne 4 ]; then
	echo "You must enter exactly 4 arguments"
	exit 0
fi

# Print the arguments
echo "In a land of $1 and $2 so bright,
There lived a creature, a peculiar sight.
Its name was $3, odd and rare,
With a nose like a $4, and tufty hair."