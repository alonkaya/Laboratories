BEGIN{
	FS = ","
}
{
	if($3 == "bachelor's degree")
		counter++
}
END{
	printf "%d\n", counter
}
