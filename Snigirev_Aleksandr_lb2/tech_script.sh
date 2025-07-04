#!/bin/bash

LOG_FILE="lb2.log"

log_command() {
    local command="$1"
    local start_time end_time elapsed_time
    local user_host current_dir time_stamp

    time_stamp=$(date '+%b %d %H:%M:%S')
    user_host=$(whoami)@$(hostname)
    current_dir=$(pwd)

    echo -e "[$time_stamp] $user_host:$current_dir\$ $command" >> "$LOG_FILE"
    output=$(eval "$command" 2>&1)
    echo -e "$output"

    while IFS= read -r line; do
  echo -e "[$time_stamp] $line" >> "$LOG_FILE"
    done <<< "$output"
}

if [ $# -gt 0 ]; then
    log_command "$*"
else
    echo "Usage: $0 <command>"
fi