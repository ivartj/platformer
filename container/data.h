#ifndef DATA_H
#define DATA_H

/* union of common data types */

typedef union _data data;

union _data {
	void *v;
	int i;
	double d;
};

#endif
