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

. "$DIR/doto-flags.sh" "$@"

if [ ! -z $PROJECT ]; then
	# todo: check doto-db-connector.c to query if project exists.
	read -n1 -p "It seems like the project $PROJECT does not exist. Would you like creating it? (y/n)" CREATE_PROJECT
	if [ $CREATE_PROJECT -eq "y" ]; then
		# create project in db
	fi
fi

read -p "Task: " TASK
echo "Storing task as global..."


