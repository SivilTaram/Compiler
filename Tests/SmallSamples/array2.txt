var i : integer; array1 : array[14] of integer;
begin
	array1[0] := 0;
	array1[1] := 1;
	for i := 2 to 13
		do
			array1[i] := array1[i-1]+array1[i-2];
	if array1[13] = 233 then
		write("array operation right.")
	else
		write("array operation failed.");
end.
