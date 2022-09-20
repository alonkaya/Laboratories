BEGIN{
	FS = ","
	print "===============\nSuccess students list\n================\n"
}
{
	if($6 >79)
		if($7 >79)
			if($8 >79){
				print $0
				count++
			}
}
END{
	printf "\nThe number of students: %d\n", count
}

