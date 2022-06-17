# summarize.sh, Lindsey Kim, 09/21/2021, Someone can use this script to get a brief overview of all files that use Bash or C.
#!/bin/bash
for arg in "$@"; do
  if [[ -r "$arg" ]]; then
    if [[ -f "$arg" ]]; then
      if [[ "$arg" == *.sh ]]; then
        echo "##" $(basename "$arg")
        echo  "\`\`\` bash"
        sed '/^$/Q;/#!/d' "$arg"
        echo "\`\`\`"
      elif [[ "$arg" == *.c ]] || [[ "$arg" == *.h ]]; then
        echo "##" $(basename "$arg")
        echo "\`\`\` c"
        sed '/^$/Q;/#!/d' "$arg"
        echo "\`\`\`"
      fi
    fi
  else
    echo "ERROR" 1>&2
  fi
done

