#ifndef __BUDDY_H__
#define __BUDDY_H__

void *buddyinit(long n_bytes, unsigned int flags, int pg_size);

void *buddyalloc(char *mem, long requested);

void buddyfree(char *mem, void *region);

int lg2(int num);

int bit_is_free(char *array, int num);

void setbit(char *array, int num, int state);

void set_lower_levels(char *array, int num_pages, int num, int state);

void dump_bm(char *array);

void dump_pm(char *array);

#endif
