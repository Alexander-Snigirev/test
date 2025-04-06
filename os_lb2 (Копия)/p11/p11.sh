#!/bin/bash


echo 'int main() { return 0; }' > p11_1.c
echo 'int sum(int a, int b) { return a + b; }' > p11_2.c
echo "Hello! This is the intro file." > intro

# Запуск фоновых задач
sleep 20 &                 # [1]
md5sum p11_1.c > /dev/null &  # [2]
sort intro > /dev/null &       # [3]

jobs         # Смотри список заданий
fg %2        # Вернуть задание 2 на передний план
kill %1
jobs