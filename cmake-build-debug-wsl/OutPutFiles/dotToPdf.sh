#! /bin/bash

for file in *.dot
do
	dot -Tpdf "$file" -o "${file%.dot}.pdf"
done
