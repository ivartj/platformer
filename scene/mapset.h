#ifndef MAPSET_H
#define MAPSET_H

#include <rect.h>
#include <data.h>
#include <iter.h>

typedef struct _mapset mapset;
typedef struct _map map;

mapset *mapset_create(void);
map *mapset_add(mapset *set, rect *r, data data);
void mapset_iter_overlap(mapset *set, iter *i, rect *r);
void map_setrect(map *m, rect *r);
void map_setdata(map *m, data data);
rect *map_rect(map *m);
data map_data(map *m);
void map_destroy(map *m);

#endif
