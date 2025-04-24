#include "../include/polynomial.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

Polynomial* create_polynomial(int degree, const typeInfo* type) {
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    if (!poly) return NULL;

    poly->degree = degree;
    poly->type = type;
    poly->coeffs = malloc((degree + 1) * type->size);
    if (!poly->coeffs) {
        free(poly);
        return NULL;
    }

    for (int i = 0; i <= degree; i++) {
        char* coeff = (char*)poly->coeffs + i * type->size;
        memset(coeff, 0, type->size);
    }

    return poly;
}

void free_polynomial(Polynomial* poly) {
    if (poly) {
        free(poly->coeffs);
        free(poly);
    }
}

void set_coefficient(Polynomial* poly, int index, void* value) {
    if (index >= 0 && index <= poly->degree) {
        char* coeff = (char*)poly->coeffs + index * poly->type->size;
        memcpy(coeff, value, poly->type->size);
    } else {
        printf("Ошибка: индекс %d вне диапазона!\n", index);
    }
}

void print_polynomial(const Polynomial* poly) {
    for (int i = poly->degree; i >= 0; i--) {
        char* coeff = (char*)poly->coeffs + i * poly->type->size;
        if (i == 0) {
            poly->type->print(coeff);
        } else if (i == 1) {
            poly->type->print(coeff);
            printf("x + ");
        } else {
            poly->type->print(coeff);
            printf("x^%d + ", i);
        }
    }
    printf("\n");
}

Polynomial* multiply_polynomial_by_scalar(const Polynomial* poly, double scalar) {

    Polynomial* result = create_polynomial(poly->degree, poly->type);
    if (!result) {
        return NULL; 
    }

    for (int i = 0; i <= poly->degree; i++) {
        char* src_coeff = (char*)poly->coeffs + i * poly->type->size;
        char* dest_coeff = (char*)result->coeffs + i * result->type->size;

    
        void* temp = malloc(poly->type->size);
        if (!temp) {
            free_polynomial(result); 
            return NULL;
        }

        poly->type->multiply_scalar(src_coeff, scalar, temp);
        memcpy(dest_coeff, temp, poly->type->size);
        free(temp);
    }

    return result; 
}

Polynomial* add_polynomials(const Polynomial* poly1, const Polynomial* poly2) {

    int max_degree = (poly1->degree > poly2->degree) ? poly1->degree : poly2->degree;
    Polynomial* result = create_polynomial(max_degree, poly1->type);

    for (int i = 0; i <= poly1->degree; i++) {
        char* coeff1 = (char*)poly1->coeffs + i * poly1->type->size;
        char* coeff_result = (char*)result->coeffs + i * result->type->size;
        memcpy(coeff_result, coeff1, poly1->type->size);
    }

    for (int i = 0; i <= poly2->degree; i++) {
        char* coeff2 = (char*)poly2->coeffs + i * poly2->type->size;
        char* coeff_result = (char*)result->coeffs + i * result->type->size;
        void* temp = malloc(result->type->size);
        result->type->sum(coeff_result, coeff2, temp);
        memcpy(coeff_result, temp, result->type->size);
        free(temp);
    }

    return result;
}

Polynomial* multiply_polynomials(const Polynomial* poly1, const Polynomial* poly2) {

    int result_degree = poly1->degree + poly2->degree;
    Polynomial* result = create_polynomial(result_degree, poly1->type);

    for (int i = 0; i <= poly1->degree; i++) {
        for (int j = 0; j <= poly2->degree; j++) {
            char* coeff1 = (char*)poly1->coeffs + i * poly1->type->size;
            char* coeff2 = (char*)poly2->coeffs + j * poly2->type->size;
            char* coeff_result = (char*)result->coeffs + (i + j) * result->type->size;
            void* temp = malloc(result->type->size);
            result->type->multiply(coeff1, coeff2, temp);
            result->type->sum(coeff_result, temp, coeff_result);
            free(temp);
        }
    }

    return result;
}

Polynomial* compose_polynomials(const Polynomial* f, const Polynomial* g) {
    Polynomial* result = create_polynomial(f->degree * g->degree, f->type);

    for (int i = 0; i <= f->degree; i++) {
        char* coeff_f = (char*)f->coeffs + i * f->type->size;
        Polynomial* term = create_polynomial(0, f->type);
        set_coefficient(term, 0, coeff_f);

        Polynomial* g_powered = create_polynomial(0, g->type);
        set_coefficient(g_powered, 0, g->type->size == sizeof(int) ? (void*)&(int){1} : (void*)&(double){1.0});

        for (int j = 0; j < i; j++) {
            Polynomial* temp = multiply_polynomials(g_powered, g);
            free_polynomial(g_powered);
            g_powered = temp;
        }

        Polynomial* temp = multiply_polynomials(term, g_powered);
        Polynomial* result_new = add_polynomials(result, temp);
        free_polynomial(result);
        result = result_new;

        free_polynomial(g_powered);
        free_polynomial(term);
        free_polynomial(temp);
    }

    return result;
}

void* get_element(const Polynomial* poly, int row, int col) {
    if (!poly || row < 0 || row > poly->degree || col != 0) {
        return NULL;
    }
    return (char*)poly->coeffs + row * poly->type->size;
}

Polynomial* copy_polynomial(const Polynomial* src) {
    if (!src) return NULL;

    Polynomial* dest = create_polynomial(src->degree, src->type);
    if (!dest) return NULL;

    memcpy(dest->coeffs, src->coeffs, (src->degree + 1) * src->type->size);

    return dest;
}

Polynomial* read_polynomial(FILE* file, const typeInfo* type) {
    if (!file) {
        printf("Ошибка: файл не открыт\n");
        return NULL;
    }

    int degree;
    if (fscanf(file, "%d", &degree) != 1) {
        printf("Ошибка чтения степени многочлена\n");
        return NULL;
    }

    Polynomial* poly = create_polynomial(degree, type);
    if (!poly) {
        printf("Ошибка создания многочлена\n");
        return NULL;
    }

    for (int i = 0; i <= degree; i++) {
        double coeff;
        if (fscanf(file, "%lf", &coeff) != 1) {
            printf("Ошибка чтения коэффициента при x^%d\n", i);
            free_polynomial(poly);
            return NULL;
        }
        set_coefficient(poly, i, &coeff);
    }

    return poly;
}


void* evaluate_polynomial(const Polynomial* poly, double x) {
    if (!poly) {
        printf("Ошибка: многочлен не существует\n");
        return NULL;
    }

    void* result = malloc(poly->type->size);
    if (!result) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }

    memset(result, 0, poly->type->size);

    void* temp = malloc(poly->type->size);
    if (!temp) {
        printf("Ошибка выделения памяти\n");
        free(result);
        return NULL;
    }

    for (int i = 0; i <= poly->degree; i++) {
        char* coeff = (char*)poly->coeffs + i * poly->type->size;

        double x_powered = pow(x, i);

 
        poly->type->multiply_scalar(coeff, x_powered, temp);
        poly->type->sum(result, temp, result);
        
    }

    free(temp);
    return result;
}

void write_polynomial(FILE* file, const Polynomial* poly, const char* label) {
    fprintf(file, "%s: ", label);
    for (int i = poly->degree; i >= 0; i--) {
        void* coeff = get_element(poly, i, 0);
        if (!coeff) {
            fprintf(stderr, "Ошибка: коэффициент не найден\n");
            return;
        }
        if (poly->type->type == INT_TYPE) {
            fprintf(file, "%d", *(int*)coeff);
        } else if (poly->type->type == DOUBLE_TYPE) {
            fprintf(file, "%.0lf", *(double*)coeff);
        }
        if (i > 0) {
            fprintf(file, "x^%d + ", i);
        }
    }
    fprintf(file, "\n");
} 

void print_value_to_file(FILE* file, const typeInfo* type, void* value) {
    if (type->type == INT_TYPE) {
        fprintf(file, "%d", *(int*)value);
    } else if (type->type == DOUBLE_TYPE) {
        fprintf(file, "%.0lf", *(double*)value);
    } else {
        fprintf(file, "Неизвестный тип данных");
    }
}