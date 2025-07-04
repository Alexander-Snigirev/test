#!/bin/bash
if [ -z "$1" ]; then
	echo "Использование: $0"
	exit 1
fi
OUTPUT_FILE="$1"
ls -lR / 2>/dev/null | awk '{print $1, $9, $NF}' | grep -E "^-|^b|^c|^d|^l|^p|^s" > all_files.txt
>"$OUTPUT_FILE"
grep "^-" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^b" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^c" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^d" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^p" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^l" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
grep "^s" all_files.txt | head -n 1 >> "$OUTPUT_FILE"
echo "Результаты сохранены в $OUTPUT_FILE"

