#include <endian.h>
#include <stdio.h>

//General idea:
// BIG_ENDIAN is defined in endian.h as the byte order. It differs between systems.
// Some use 3421. Some use 4321.
// To account for both of these, we use modding and bit shifting.
// Take the BIG_ENDIAN variable, and find out the positions of the bites.
// Then, bit shift accordingly. I came up with a formula.
// Basically, bit shift left for the first 2 bytes. Then bit shift right.
// Bit shift for first half: Shift left 4x - 4 - 4 * y, where x is the destination position and y is source position.
// Bit shift for second half: Shift right -1*(4x - 4 - 4 * y), where x is the destination position and y is source position.
// Make sure to AND the bits with their position to zero out the rest of them.
// Then OR everything together.

//BIG_ENDIAN on my system: 4321

#define BYTE_SWAP(bytes) (bytes << (4*(BIG_ENDIAN/1000)-4) & 0xF000) | (bytes << (4*(BIG_ENDIAN % 1000 / 100)-4*2) & 0x0F00) | (bytes >> abs(4*(BIG_ENDIAN % 1000 / 100)-4*3-4) & 0x00F0) | (bytes >> abs(4*(BIG_ENDIAN % 100 / 10)-4*4-4) & 0x000F)
int main(int argc, char **argv)
{
	short val;
	char *p_val;
	p_val = (char *) &val;
	
	p_val[0] = 0x12;
	p_val[1] = 0x34;
	
	val = BYTE_SWAP(0x1234);
	
	printf("%x\n", val);
	
	return 0;
}

