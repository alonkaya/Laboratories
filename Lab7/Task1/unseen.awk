BEGIN{
	FS = ","
}
{
	print "year:" $2 "\t" "age:" $3
}
