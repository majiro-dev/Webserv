<?php
function playGame($userChoice) {
	$choices = array('rock', 'paper', 'scissors');
	$computerChoice = $choices[array_rand($choices)];

	echo "User choice: $userChoice\n";
	echo "Computer choice: $computerChoice\n";

	if ($userChoice === $computerChoice) {
		echo "It's a tie!\n";
	} else {
		switch ($userChoice) {
			case 'rock':
				echo $computerChoice === 'scissors' ? "User wins!\n" : "Computer wins!\n";
				break;
			case 'paper':
				echo $computerChoice === 'rock' ? "User wins!\n" : "Computer wins!\n";
				break;
			case 'scissors':
				echo $computerChoice === 'paper' ? "User wins!\n" : "Computer wins!\n";
				break;
			default:
				echo "Invalid user choice. Please choose either rock, paper, or scissors.\n";
		}
	}
}

if ($argc > 1) {
	playGame($argv[1]);  // User choice is taken from the command line argument
} else {
	echo "Please provide a choice (rock, paper, or scissors) as a command line argument.\n";
}
?>