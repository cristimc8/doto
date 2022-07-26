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

read -p "Task: " TASK


