BEGIN{
	FS = ","
}
{
	if($4 == "standard"){
			count++
			summath = summath + $6
			sumread = sumread + $7
			sumwrite = sumwrite + $8
	}
}
END{
	math = summath/count
	read = sumread/count
	write = sumwrite/count
	printf "math: %f\tread: %f\twrite: %f\n", math, read, write
}
