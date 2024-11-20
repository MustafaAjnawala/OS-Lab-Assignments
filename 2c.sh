echo "Enter the 1st string(NOT NULL)"
read str1

# Check if the first string length is zero by using -z
if [ -z "$str1" ]; then
  echo "The first string cannot be null."
  exit 1
fi

read -p "Enter the second string:" str2

echo ""
# Check if the second string is a substring of the first string
if [[ "$str1" == *"$str2"* ]]; then
  echo "The second string is a substring!!"
else
  echo "The second string is NOT a substring of the first string."
fi