# Show the project stats like: lines, bytes and commits.
#!/bin/bash

files=`find . -name '*.c' -o -name '*.h'`

lines=`cat $files | wc -l`
bytes=`du -sh | awk '{print $1}'`
commits=`git rev-list --count --all`

echo -e "Project stats:"
echo -e "\tLines: $lines"
echo -e "\tBytes: $bytes"
echo -e "\tCommits: $commits"
