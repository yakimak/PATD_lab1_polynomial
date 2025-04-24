#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdio.h>
#include "typeInfo.h"

typedef struct {
    int degree;       
    void* coeffs;    
    const typeInfo* type; 
} Polynomial;

Polynomial* create_polynomial(int degree, const typeInfo* type);
void free_polynomial(Polynomial* poly);
void set_coefficient(Polynomial* poly, int index, void* value);
void* get_element(const Polynomial* poly, int row, int col); 
Polynomial* copy_polynomial(const Polynomial* src);
void print_polynomial(const Polynomial* poly);
Polynomial* add_polynomials(const Polynomial* poly1, const Polynomial* poly2);
Polynomial* multiply_polynomials(const Polynomial* poly1, const Polynomial* poly2);
Polynomial* multiply_polynomial_by_scalar(const Polynomial* poly, double scalar);
Polynomial* compose_polynomials(const Polynomial* f, const Polynomial* g);
Polynomial* read_polynomial(FILE* file, const typeInfo* type);
void write_polynomial(FILE* file, const Polynomial* poly, const char* label);
void* evaluate_polynomial(const Polynomial* poly, double x);
void print_value_to_file(FILE* file, const typeInfo* type, void* value);

#endif