#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


enum DynCode {
    DYN_OK = 0,
    DYN_REALLOC_ERR,
    DYN_INDEX_OUT_OF_RANGE_ERR,
    DYN_ARRAY_EMPTY_ERR
};


typedef struct Numbers {
    double* nums;
    size_t cap;
    size_t count;
} Numbers;



// Returns 1 if the given index is out of bounds
// otherwise returns 0
static int index_bound_control(Numbers* dyn, ptrdiff_t index) {
    if (index >= dyn->count || index < 0) return 1;
    else return 0;
}


// new_size_to_alloc(cap) = cap + cap / 8 + 6 then make it a multiple of 4
static size_t new_size_to_alloc(size_t current_size) {
    return (current_size + (current_size >> 3) + 6) & ~(size_t)3;
}


// Allocate and initialize an empty Numbers object
Numbers* num_init() {
    Numbers* dyn = malloc(sizeof(Numbers));

    if (!dyn) return NULL;

    dyn->nums = NULL;
    dyn->cap = 0;
    dyn->count = 0;

    return dyn;
}


// Append element to the end of the array
enum DynCode num_push(Numbers* dyn, double value) {
    if (dyn->cap == dyn->count) {
        size_t new_cap = new_size_to_alloc(dyn->cap);

        double* temp = realloc(dyn->nums, new_cap * sizeof(double));

        if (temp == NULL)
            return DYN_REALLOC_ERR;

        dyn->nums = temp;
        dyn->cap = new_cap;
    }

    dyn->nums[dyn->count++] = value;
    return DYN_OK;
}



// Remove the last element of the array and store it in out
enum DynCode num_pop(Numbers* dyn, double *out) {
    if (dyn->count == 0)
        return DYN_ARRAY_EMPTY_ERR;

    if (out)
        *out = dyn->nums[dyn->count - 1];

    dyn->count--;


    if (dyn->count == 0) {
        free(dyn->nums);
        dyn->nums = NULL;
        dyn->cap = 0;
    }

    else if (dyn->count < (dyn->cap >> 1)) {
        size_t new_cap = new_size_to_alloc(dyn->count);

        double* temp = realloc(dyn->nums, new_cap * sizeof(double));

        if (temp) {
            dyn->nums = temp;
            dyn->cap = new_cap;
        }
    }

    return DYN_OK;
}




// Insert a value at the given index.
// Elements at and above the index are shifted to the right
// Allows insertion at the end of the array
enum DynCode num_add(Numbers* dyn, double value, ptrdiff_t index) {
    if (index < 0) index += dyn->count;

    if (index != dyn->count && index_bound_control(dyn, index)) {
        return DYN_INDEX_OUT_OF_RANGE_ERR;
    }


    if (dyn->cap == dyn->count) {
        size_t new_cap = new_size_to_alloc(dyn->cap);

        double* temp = realloc(dyn->nums, new_cap * sizeof(double));

        if (temp == NULL)
            return DYN_REALLOC_ERR;

        dyn->nums = temp;
        dyn->cap = new_cap;
    }


    dyn->count++;

    for (size_t i = (dyn->count - 1); i > index; --i) {
        dyn->nums[i] = dyn->nums[i - 1];
    }

    dyn->nums[index] = value;
    return DYN_OK;
}



// Remove the value at the given index of the array and store it in out
// out can be NULL if the value does not have to be stored
enum DynCode num_remove(Numbers* dyn, ptrdiff_t index, double *out) {
    if (index < 0) index += dyn->count;

    if (index_bound_control(dyn, index))
        return DYN_INDEX_OUT_OF_RANGE_ERR;

    if (out)
        *out = dyn->nums[index];

    for (size_t i = index; i < (dyn->count - 1); i++) {
        dyn->nums[i] = dyn->nums[i + 1];
    }

    dyn->count--;

    if (dyn->count == 0) {
        free(dyn->nums);
        dyn->nums = NULL;
        dyn->cap = 0;
    }

    else if (dyn->count < (dyn->cap >> 1)) {
        size_t new_cap = new_size_to_alloc(dyn->count);

        double* temp = realloc(dyn->nums, new_cap * sizeof(double));

        if (temp) {
            dyn->nums = temp;
            dyn->cap = new_cap;
        }
    }

    return DYN_OK;
}





// Retrieve the value at the given index and store it in out
enum DynCode num_at(Numbers* dyn, ptrdiff_t index, double *out) {
    if (index < 0)
        index += dyn->count;

    if (index_bound_control(dyn, index))
        return DYN_INDEX_OUT_OF_RANGE_ERR;

    *out = dyn->nums[index];
    return DYN_OK;
}



// Replace the value at the given index
enum DynCode num_alter(Numbers* dyn, ptrdiff_t index, double data) {
    if (index < 0)
        index += dyn->count;

    if (index_bound_control(dyn, index))
        return DYN_INDEX_OUT_OF_RANGE_ERR;

    dyn->nums[index] = data;
    return DYN_OK;
}



// Returns the count of the nums
size_t num_get_count(Numbers* dyn) {
    return dyn->count;
}


// Returns the amount of bytes that is allocated for the Numbers object
size_t num_mem_usage(Numbers* dyn) {
    return dyn->cap * sizeof(double) + sizeof(dyn->count) + sizeof(dyn->cap);
}


// Free the Numbers object and its internal buffer
void num_close(Numbers* dyn) {
    if (!dyn) return;
    if (dyn->nums) free(dyn->nums);
    free(dyn);
}


// print nums
void num_print(Numbers* dyn) {
    printf("[");

    for (size_t i = 0; i < dyn->count; ++i) {
        printf("%.2f, ", dyn->nums[i]);
    }

    if (dyn->count) printf("\b\b");
    printf("]\n");
}


int main() {
    Numbers* denem = num_init();

    num_push(denem, 10.3);
    num_push(denem, 32.2);
    num_push(denem, 42.1);

    if (num_alter(denem, -2, 3) == DYN_INDEX_OUT_OF_RANGE_ERR)
        return 1;

    double v;
    num_at(denem, 1, &v);
    printf("Sayı: %f\n", v);

    num_add(denem, 42.141592653589, 1);

    num_print(denem);

    num_close(denem);
}

