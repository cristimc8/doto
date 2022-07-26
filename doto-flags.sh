#!/bin/bash

while getopts p: flag
do
	case "${flag}" in
		p) PROJECT=${OPTARG};;
	esac
done
