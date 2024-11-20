echo "enter the number of array elements(max 20)" 
read n

while [ "$n" -gt 20 ];do
	echo "enter number within limit"
	read n
done

echo ""
for((i=0; i<n; i++ ))
do
	echo "Enter array element $((i+1)):"
	read ele
	arr+=("$ele")
done

echo ""
for((i=0; i<n-1; i++))
do
	for((j=0; j<n-i-1; j++))
	do
		if(( ${arr[j]} >  ${arr[j+1]} )); then
			temp=${arr[j]}
			arr[j]=${arr[j+1]}
			arr[j+1]=${temp}
		fi
	done
done

echo "Sorted array is given as: ${arr[@]}"
