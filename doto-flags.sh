#!/bin/bash

DISPLAY=0
LIST_PROJECTS=0
DELETE_PROJECT=0

while getopts "p:ljd" flag
do
	case "${flag}" in
		p)
			PROJECT=${OPTARG}
			;;
		l)
			DISPLAY=1
			;;
        j)
            LIST_PROJECTS=1
            ;;
        d)
            DELETE_PROJECT=1
            ;;
	esac
done
