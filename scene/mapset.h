#ifndef MAPSET_H
#define MAPSET_H

#include <rect.h>
#include <data.h>
#include <iter.h>

typedef struct _mapset mapset;
typedef struct _map map;

mapset *mapset_create(void);
#define mapset_add(set, r, data) _mapset_add(set, r, &data, sizeof(data))
map *_mapset_add(mapset *set, rect *r, void *data, size_t datalen);
void mapset_iter_overlap(mapset *set, iter *i, rect *r);
void map_setrect(map *m, rect *r);
#define map_setdata(m, data) _map_setdata(m, data, sizeof(data))
void _map_setdata(map *m, void *data, size_t datalen);
rect *map_rect(map *m);

void map_data(map *m, void *data);
void map_destroy(map *m);

#endif
