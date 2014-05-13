#!/bin/bash

while 1

	for f in *.hex
	do
		upload $f e
		upload $f a
		rm $f
	done
done
