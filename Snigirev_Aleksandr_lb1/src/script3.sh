#!/bin/bash
fname="$1"

if [ -z $fname ]; then
echo "Вы не ввели имя файла";
exit 1
fi

LINKS=$(ls -lR $HOME 2>/dev/null | grep -E "^l" | grep "$fname")

echo "Всего ссылок: $COUNT"
echo "$LINKS" | wc -l
echo "$LINKS"
