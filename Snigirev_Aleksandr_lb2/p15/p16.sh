#!/bin/bash
PRA_PID=$1
echo "Имя: "; grep ^Name /proc/$PRA_PID/status
echo "UID: "; grep ^Uid /proc/$PRA_PID/status
echo "PPID: "; grep ^PPid /proc/$PRA_PID/status
echo "Память: "; grep ^Vm /proc/$PRA_PID/status
echo "Исполняемый файл: "; readlink /proc/$PRA_PID/exe