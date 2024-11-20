cmd -- $cat data.txt
output -- 
1|Mustafa|89|34|90
2|Kamlesh|89|45|92
3|Mohit|89|38|94

cmd -- $cat awkP.awk
BEGIN{
    print"Name  | %tage |   Grades"
    print"------------------------"
}
{
    if($3 < 40 || $4<40 || $5<40){
        grade = "F";
    }else{
        grade = "P";
    }
    printf "%-10s | %-7f | %-12s\n",$2,percentage,grade;
}

cmd -- 
$ awk -F "|" -f awkP.awk data.txt