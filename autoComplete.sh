# echo "AUTOCOMPLETE"

args=("$@")

filename="tests.txt"

AUTOCOMPLETES=""

while IFS='' read -r line || [[ -n $line ]]; do
  # echo "Add: $line"
  AUTOCOMPLETES="$AUTOCOMPLETES $line"
done < "$filename"

complete -W "$AUTOCOMPLETES" ./test

echo "Done adding auto completes."




# if [[ ${args[0]} = "remove" ]]; then
#   while IFS='' read -r line || [[ -n $line ]]; do
#     echo "Remove: $line"
#     #complete -W -rm $line ./test
#   done < "$filename"
# fi



## declare an array variable
#declare -a arr=("element1" "element2" "element3")

## now loop through the above array
#for i in "${arr[@]}"
#do
#   echo "$i"
#   # or do whatever with individual element of the array
#done

# You can access them using echo "${arr[0]}", "${arr[1]}" also




#    IFS='' (or IFS=) prevents leading/trailing whitespace from being trimmed.
#    -r prevents backslash escapes from being interpreted.
#    || [[ -n $line ]] prevents the last line from being ignored if it doesn't end with a \n (since read returns a non-zero exit code when it encounters EOF).


