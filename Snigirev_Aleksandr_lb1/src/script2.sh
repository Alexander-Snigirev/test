#!/bin/bash

if [ -z "$1" ]; then
	echo "Введите имя файла"
	exit 1
fi

FILENAME="$1"

if [ ! -e "$FILENAME" ]; then
	echo "Такого файла не существует"
	exit 1
fi

INODE=$(ls -i "$FILENAME" | awk '{print $1}')
echo "Ищем ссылки на $FILENAME"


ls -iR $HOME 2>/dev/null | grep "^$INODE" | awk '{print $2}' > hardlinks.txt
echo "Жесткие ссылки записаны в hardlinks.txt"

