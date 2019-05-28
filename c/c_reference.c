#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

// Not supported on all systems
#if 0
#include <complex.h>
#include <stdatomic.h>
#include <threads.h>
#endif

struct position {
	
	int x;
	int y;
	int z;
};

int main(int argc, const char *argv[]) {

	//--Syntax -s
	//--Build_Systems -b
	
	struct position pos = { 0, 5, 11 };	
	struct position *ppos;

	ppos = &pos;

	printf("x: %d y: %d z: %d\n", (*ppos).x, ppos->y, ppos->z);
	

	return 0;
}
