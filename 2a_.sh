echo "Enter the string"
read str

len=$(echo -n "$str" | wc -c) 
#'-n' so that no newline character is added to the end of string

end=$len
ctr=0
for((i=1; i<=len/2; i++))
do
    ch1=$(echo "$str" | cut -c $i)
    ch2=$(echo "$str" | cut -c $end)
    if [ "$ch1" != "$ch2" ]; then
        ctr=1
        break        
    fi
    end=$((end-1))
done

if [ "$ctr" = 1 ]; then
    echo "Not Palindrome"
else
    echo "Palindrome"
fi
