#!/bin/bash

DISPLAY=0

while getopts ":p:l" flag
do
	case "${flag}" in
		p) 
			PROJECT=${OPTARG}
			;;
		l)
			DISPLAY=1
			;;
	esac
done
