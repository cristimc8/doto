#!/bin/bash

HAS_SQLITE3=$(dpkg -l sqlite3 2>&1 | grep sqlite3 | wc -l)

if [[ $HAS_SQLITE3 -eq 0 ]]; then
	echo -e 'Package sqlite3 is needed by this script and not installed.\nInstall it by running: "sudo apt install sqlite3"'
	exit 0
fi

DIR="${BASH_SOURCE%/*}"

if [[ ! -d "$DIR" ]]; then
	DIR="$PWD"
fi

__CONNECTOR_PATH="$DIR/doto-db-connector/doto-db-connector"
CONNECTOR="$__CONNECTOR_PATH"

. "$DIR/doto-flags.sh" "$@"

if [ ! $DISPLAY -eq "0" ]; then
	if [ ! -z "$PROJECT" ]; then
		echo "$($CONNECTOR "display" "$PROJECT")"
	else
		echo "$($CONNECTOR "display")"
	fi
	exit 0
fi

if [ ! $LIST_PROJECTS -eq "0" ]; then
    echo "$($CONNECTOR "listProjects")"
    exit 0
fi

if [ ! -z "$DELETE_PROJECT" ]; then
    $($CONNECTOR "deleteProject" "$PROJECT")
    echo "The project $PROJECT was deleted with success!"
    exit 0
fi

read -p "Task: " TASK

# If a project was provided
if [ ! -z "$PROJECT" ]; then

	# Check if project exists
	PROJECT_EXISTS=$($CONNECTOR "checkProject" "$PROJECT")
	# If project does not exist
	if [[ $PROJECT_EXISTS -eq -1 ]]; then
		read -n1 -p "It seems like the project $PROJECT does not exist. Would you like creating it? (y/n) " CREATE_PROJECT

		if [ $CREATE_PROJECT = "y" ]; then
			$($CONNECTOR "saveProject" "$PROJECT")
			echo -e "\nThe project $PROJECT was created with success!\n"
		else
			echo -e "\nExiting... Bye\n"
			exit 0
		fi
	fi

	# Save task with project
	$($CONNECTOR "saveTask" "$TASK" "$PROJECT")
	echo "The task $TASK was saved to project $PROJECT."
else
	$($CONNECTOR "saveTask" "$TASK")
	echo "The task $TASK was saved globally."
fi



