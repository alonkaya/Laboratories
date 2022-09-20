BEGIN{
	FS = ","
}
{
	if($1 > 49)
		if($1 < 71)
			print $5
}
