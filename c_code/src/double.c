#include "../include/double.h"
#include "../include/typeInfo.h"
#include <stdio.h>

void sum_double(void* a, void* b, void* result) {
    *(double*)result = *(double*)a + *(double*)b;
}

void multiply_double(void* a, void* b, void* result) {
    *(double*)result = *(double*)a * *(double*)b;
}

void multiply_double_by_scalar(void* a, double scalar, void* result) {
    *(double*)result = *(double*)a * scalar;
}

void print_double(void* element) {
    printf("%lf ", *(double*)element);
}


typeInfo doubleType = {
    .size = sizeof(double),
    .print = print_double,
    .sum = sum_double,
    .multiply = multiply_double,
    .multiply_scalar = multiply_double_by_scalar,
    .type = DOUBLE_TYPE
};