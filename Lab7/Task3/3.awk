BEGIN{
	FS = ","
	OFS = "-----"
}
$(2) ~ /happy/ { print $2,$3,$4 }

