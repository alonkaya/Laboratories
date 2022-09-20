BEGIN{
	FS = ","
}
{
	print $4"\t\t|"$3"\t|"$2
}
