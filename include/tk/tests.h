#ifndef __TESTS_H__
#define __TESTS_H__

struct Test {
	int (*run) (void);
	const char * description;
};

extern struct Test tests[];

/**
 * RunTests: Runs all the unit tests
 */
void RunTests(void);

#endif
