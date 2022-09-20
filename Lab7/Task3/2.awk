BEGIN{
	FS = ","
}
$(0) ~ /fun|fan/ { count++ }
END{
	printf "The number of poems: %d\n", count
}
