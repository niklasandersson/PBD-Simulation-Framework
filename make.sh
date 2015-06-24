args=("$@")

if [ $# -eq 0 ]; then
  echo --- make main.makefile ---
  make -f main.makefile
  echo --- make test.makefile ---
  make -f test.makefile
fi

if [[ ${args[0]} = clean ]]; then
  echo --- make main.makefile ---
  make -f main.makefile ${args[0]} 
  echo --- make test.makefile ---
  make -f test.makefile ${args[0]}
fi

if [[ ${args[0]} = all ]]; then
  echo --- make main.makefile ---
  make -f main.makefile ${args[1]}
  echo --- make test.makefile ---
  make -f test.makefile ${args[1]} 
else

  if [[ ${args[0]} = test ]]; then
    echo --- make test.makefile ---
    make -f test.makefile ${args[1]}
  fi

  if [[ ${args[0]} = src ]]; then
    echo --- make main.makefile ---
    make -f main.makefile ${args[1]}
  fi 

  if [[ ${args[0]} = main ]]; then
    echo --- make main.makefile ---
    make -f main.makefile ${args[1]}
  fi 

fi
