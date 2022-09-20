BEGIN{
	FS = ","
	OFS = "\n"
}
{
	if($2 > 1970)
		print "Actor name:\t" $3, "Movie name:\t" $5
		print "-----------------------------------"
}
