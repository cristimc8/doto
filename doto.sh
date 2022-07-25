#!/bin/bash

DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$DIR" ]]; then 
	DIR="$PWD" 
fi

. "$DIR/options.sh"

echo $TEST
