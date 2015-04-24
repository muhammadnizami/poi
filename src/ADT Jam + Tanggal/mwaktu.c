#include <stdio.h>
#include "waktu.h"

int main(){
	WAKTU W;
	printf("WAKTU sekarang: ");
	TulisWAKTU(GetCurrentTime());
	printf("\n");

	return 0;
}
