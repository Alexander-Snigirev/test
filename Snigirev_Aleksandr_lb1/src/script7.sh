#!/bin/bash

DIR_NAME="$1"

if [ -z $DIR_NAME ]; then
	echo "Передайте имя нового каталога"
	exit 1
fi

rm -rf $DIR_NAME
mkdir $DIR_NAME
echo "Каталог $DIR_NAME успешно создан"
size=$(du -sh $DIR_NAME)
echo "Стартовый размер каталога: $size"
echo "Добавляем файлы..."
cp main.c $DIR_NAME
echo "main.c добавлен в каталог"
size=$(du -sh $DIR_NAME)
echo "Текущий размер: $size"
cp first.log $DIR_NAME
echo "first.log добавлен в каталог"
size=$(du -sh $DIR_NAME)
echo "Текущий размер: $size"
rm $DIR_NAME/main.c
size=$(du -sh $DIR_NAME)
echo "Текущий размер: $size"
echo "Файл main.c удален из каталога"
rm $DIR_NAME/first.log
echo "Файл first.log удален из каталога"
size=$(du -sh $DIR_NAME)
echo "Текущий размер: $size"



