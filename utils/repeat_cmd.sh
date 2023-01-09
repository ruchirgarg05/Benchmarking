#!/bin/bash

# Sample Usage - 
# ./repeat_cmd.sh rdtsc 20
# The executable named 'rdtsc' will be run 20 times with the expectation that 
# it will print precisely 'cycles_taken:<some uint64>'

EXECUTABLE=$1
REPEAT_COUNT=$2

PWD=`pwd`
echo "Running command - ${0} ${1} ${2}"
echo "Currently in $PWD"

if [ -z "$EXECUTABLE" ]
then
    echo "Error! Need an EXECUTABLE: depthfirst_parallel | depthfirst_serial"
    exit 1
else
    if [ ! -f "$EXECUTABLE" ]
    then 
        echo "Error! ${EXECUTABLE} not found!"
        exit 1
    fi
fi

if [ -z "$REPEAT_COUNT" ]
then
    echo "INFO! REPEAT_COUNT not provided. Using 10 by default"
    REPEAT_COUNT=10
fi





echo "Running ${EXECUTABLE} ${REPEAT_COUNT} times to get the median & average."

echo "\nS.No.\t       \tAv time (in cycles)"
echo "==================================================================="
SUM=0
LOOP_COUNT=0

declare -a times_array

for i in {1..100000}
do
    LOOP_COUNT=`expr $LOOP_COUNT + 1`
    TIME_TAKEN=`./$EXECUTABLE | grep -i "cycles_taken =" | rev | cut -d = -f1 | rev | cut -d ' ' -f2`
    printf "%d\t    \t%d\n" "$i" "${TIME_TAKEN}"
    SUM=$(echo "$TIME_TAKEN + $SUM" | bc)
    times_array[$i]=$TIME_TAKEN

    if [ $LOOP_COUNT -eq $REPEAT_COUNT ]
    then
        echo "Total counter is ${LOOP_COUNT}"
        break
    fi
done


int(){ printf '%d' ${1:-} 2>/dev/null || :; }

#echo "Times array is ${times_array[@]}."

median() {
    # echo "$@"
    N=$#
    #echo $N
    #echo $arr
    arr=($(printf '%d\n' "${@}" | sort -n))

    if (( $N % 2 == 1 )); then
        # echo "ODD"
        (( idx=N/2))
        # echo $idx
        (( val=${arr[idx]} ))
    else
        # echo "EVEN"
        idx1=$(($N / 2))
        (( idx1=N/2))
        (( idx2=idx1-1))
        (( val=(${arr[idx1]} + ${arr[idx2]})/2 ))
    fi
    printf "Median:%d\n" "${val}"
}


stddev() {
  arr=$(printf '%d\n' "${@}")
  stdd="$(echo ${arr[@]} | ruby -e 'arr=$<.read.split(/\s/).map { |e| Float(e) rescue nil }.compact
                             sumsq=arr.inject(0) { |acc, e| acc+=e*e }
                             p (sumsq/arr.length - (arr.sum/arr.length)**2)**0.5')"
  echo "Std dev: $stdd"     
}

median ${times_array[@]}
echo "Sum is $SUM"
AVERAGE=$(echo "scale = 6; $SUM / $REPEAT_COUNT" | bc)
# printf "Average:%.2f\n" "${AVERAGE}"
echo "Average is $AVERAGE"
stddev ${times_array[@]}
echo "==================================================================="
