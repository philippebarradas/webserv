#!/bin/sh
for i in $(seq 1 10)
	do
	curl -s -k 'GET' -H 'header info' -b 'stuff' 'localhost:8080'
	done
