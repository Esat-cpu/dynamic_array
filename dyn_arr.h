#ifndef DYN_ARR_H
#define DYN_ARR_H

#include <stddef.h>

typedef struct Numbers Numbers;

enum DynCode {
    DYN_OK = 0,
    DYN_REALLOC_ERR,
    DYN_INDEX_OUT_OF_RANGE_ERR,
    DYN_ARRAY_EMPTY_ERR
};

typedef enum DynCode DynCode;


Numbers* num_init();
DynCode num_push(Numbers* dyn, double thing);
DynCode num_pop(Numbers* dyn, double *out);
DynCode num_add(Numbers* dyn, double thing, ptrdiff_t index);
DynCode num_at(Numbers* dyn, ptrdiff_t index, double *out);
DynCode num_alter(Numbers* dyn, ptrdiff_t index, double data);
void num_close(Numbers* dyn);
void num_print(Numbers* dyn);

#endif

