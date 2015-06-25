args=("$@")

if [ $# -eq 0 ]; then
  echo --- main.makefile ---
  make -f main.makefile
  echo --- test.makefile ---
  make -f test.makefile
fi

if [[ ${args[0]} = clean ]]; then
  echo --- main.makefile ---
  make -f main.makefile ${args[0]} 
  echo --- test.makefile ---
  make -f test.makefile ${args[0]}
fi

if [[ ${args[0]} = all ]]; then
  echo --- main.makefile ---
  make -f main.makefile ${args[1]}
  echo --- test.makefile ---
  make -f test.makefile ${args[1]} 
else

  if [[ ${args[0]} = test ]]; then
    echo --- test.makefile ---
    make -f test.makefile ${args[1]}
  fi

  if [[ ${args[0]} = src ]]; then
    echo --- main.makefile ---
    make -f main.makefile ${args[1]}
  fi 

  if [[ ${args[0]} = main ]]; then
    echo --- main.makefile ---
    make -f main.makefile ${args[1]}
  fi 

fi

echo --- autoComplete.sh ---
. ./autoComplete.sh
