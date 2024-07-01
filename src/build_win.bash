#!/bin/bash

test -z $(git status --porcelain) 2> /dev/null
if [ $? != 0 ] ; then
	echo
	echo "WARNING -- uncommitted changes"
	echo
fi

echo \"$(git log --oneline | head -n1 | cut "-d " -f1)\" | tee gitver.txt

MSBuild.exe -property:Configuration=Release | tee MSBuild.log
