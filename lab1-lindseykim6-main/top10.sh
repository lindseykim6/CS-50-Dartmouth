#!/bin/bash
if [[ -r "$1" ]] && [[ "$#" -eq 1 ]] && [[ -f "$1" ]]; then
  echo "| Area | Doses administered  |"
  echo "| :--------- | --------: |"
  grep "All" "$1" | cut -d ',' -f 2,10 | sort -t ',' -nr -k2|head|sed 's/,/|/'| sed 's/^/|/'| sed 's/$/|/'
  exit 0
else
  echo "ERROR" 1>&2
  exit 1
fi

