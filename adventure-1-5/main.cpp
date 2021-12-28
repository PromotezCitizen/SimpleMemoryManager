#include "memManage.h"

Chunk *available;
unsigned int tag = 255;

int main(int argc, char *argv[]) {
	setTag();

	initMem();

	int a = myalloc(10);
	printMemStat();

	int b = myalloc(20);
	printMemStat();

	int c = myalloc(10);
	printMemStat();

	myfree(a, 10);
	printMemStat();

	a = myalloc(30);
	printMemStat();

	myfree(c, 10);
	printMemStat();

	c = myalloc(10);
	printMemStat();

	int d = myalloc(40);
	printMemStat();

	myfree(b, 20);
	printMemStat();

	releaseMem();

	return 0;
}