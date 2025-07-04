ls -lR /home/alexander 2>/dev/null | grep -E "^l" | awk '{print $9, $11}' | while read link target; do
	if [ "$(readlink -f "$link")" = "$1" ]; then
		echo "$link"
	fi
done		