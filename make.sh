args=("$@")

if [ $# -eq 0 ]; then
  make -f main.makefile
  make -f test.makefile
fi

if [[ ${args[0]} = clean ]]; then
  make -f main.makefile ${args[0]} 
  make -f test.makefile ${args[0]}
fi

if [[ ${args[0]} = all ]]; then
  make -f main.makefile ${args[1]}
  make -f test.makefile ${args[1]} 
else

  if [[ ${args[0]} = test ]]; then
    make -f test.makefile ${args[1]}
  fi

  if [[ ${args[0]} = src ]]; then
    make -f main.makefile ${args[1]}
  fi 

  if [[ ${args[0]} = main ]]; then
    make -f main.makefile ${args[1]}
  fi 

fi
