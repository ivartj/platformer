#include "iter.h"

int iterate(iter *i, void *retval)
{
	return i->next(i, retval);
}
