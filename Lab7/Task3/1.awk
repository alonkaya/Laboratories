BEGIN{
	FS = ","
	
}
$(0) ~ /spring/ { print $2 "----" $3 "----" $4 }




