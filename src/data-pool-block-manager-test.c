#include "data-pool-block-manager.h"
#include <assert.h>

void testConstants(){
	assert(DATA_POOL_BLOCK_MIN_IDX==0x00);
	assert(DATA_POOL_BLOCK_MAX_IDX==0xFFFF);
}

int main(){
	testConstants();
}
