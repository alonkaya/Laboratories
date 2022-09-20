#! /usr/bin/awk -f

BEGIN{
	FS = ""
	while((i = getline line < "mark") > 0)
		print line
	print "---------------------------------"
	
}
{
	print $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26
	print "---------------------------------"
}
