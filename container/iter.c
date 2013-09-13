#include "iter.h"

int iterate(iter *i, data *retval)
{
	return i->next(i, retval);
}
