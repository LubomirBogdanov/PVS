
.text

.global asm_rms

	divider: .double 4.00

//d0 - first (leftmost) parameter
//d1 - second parameter
//d2 - third parameter
//d3 - fourth (rightmost) parameter
//d0 - return value
asm_rms:
	...Place assembly for calculating the square of the first parameter...
	...Place assembly for calculating the square of the second parameter...
	...Place assembly for calculating the square of the third parameter...
	...Place assembly for calculating the square of the fourth parameter...
	...Place assembly for calculating the sum of all the registers d0 through d3...
	vldr.f64	d5,divider
	vdiv.f64	d4,d4,d5
	vsqrt.f64	d4,d4
	vmov.f64	d0,d4
	mov pc,lr //Return to main
