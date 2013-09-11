#include "prime.h"
#include <stdlib.h>

static int add(int prime);
static int inc(void);

static int *primes = NULL;
static int cap = 0;
static int len = 0;

int add(int prime)
{
	if(cap == len) {
		if(cap == 0)
			cap = 256;
		else
			cap <<= 1;
		primes = realloc(primes, sizeof(int) * cap);
	}

	primes[len++] = prime;

	return prime;
}

int inc(void)
{
	int num;

	switch(len) {
	case 0:
		return add(2);
	case 1:
		return add(3);
	default:
		num = primes[len - 1];
		do {
			num += 2;
		} while(!prime_check(num));
		return add(num);
	}
}

int prime_get(int idx)
{
	while(len <= idx)
		inc();
	return primes[idx];
}

int prime_check(int num)
{
	int i;
	int prime;

	for(prime = prime_get(i = 0);
	    (prime * prime <= num) && (num % prime != 0);
	    prime = prime_get(++i));

	if(prime * prime > num)
		return 1;
	else
		return 0;
}

int prime_above(int num)
{
	for(++num; !prime_check(num); num++);

	return num;
}
