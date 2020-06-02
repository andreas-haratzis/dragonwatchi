#!/bin/bash

for file in `find . -name "*.bmp"`
do
    tools/miniz c "$file" "$file.gz"
done
