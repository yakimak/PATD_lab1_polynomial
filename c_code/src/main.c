#include "../include/polynomial.h"
#include "../include/int.h"
#include "../include/double.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Использование: %s <входной файл> <выходной файл>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    FILE* input = fopen(input_file, "r");
    if (!input) {
        perror("Ошибка открытия входного файла");
        return 1;
    }

    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Ошибка открытия выходного файла");
        fclose(input);
        return 1;
    }

    Polynomial* poly1 = read_polynomial(input, &doubleType);
    Polynomial* poly2 = read_polynomial(input, &doubleType);
    Polynomial* poly3 = read_polynomial(input, &doubleType);

    if (!poly1 || !poly2 || !poly3) {
        fprintf(stderr, "Ошибка чтения многочленов\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    Polynomial* sum12 = add_polynomials(poly1, poly2);
    Polynomial* sum123 = add_polynomials(sum12, poly3);
    Polynomial* product23 = multiply_polynomials(poly2, poly3);
    Polynomial* product123 = multiply_polynomials(poly1, product23);

    Polynomial* composition_12 = compose_polynomials(poly1, poly2);
    Polynomial* composition_23 = compose_polynomials(poly2, poly3);
    Polynomial* composition_31 = compose_polynomials(poly3, poly1);

    Polynomial* f5 = multiply_polynomial_by_scalar(poly1, 5);
    Polynomial* g10 = multiply_polynomial_by_scalar(poly2, 10);
    Polynomial* h333 = multiply_polynomial_by_scalar(poly3, 333);
    Polynomial* fg_2 = multiply_polynomial_by_scalar(composition_12, -2);

    void* value1 = evaluate_polynomial(poly1, 2);
    void* value2 = evaluate_polynomial(poly2, 3);
    void* value3 = evaluate_polynomial(poly3, -2);
    void* value4 = evaluate_polynomial(product123, -1);



    write_polynomial(output, poly1, "f(x)");
    write_polynomial(output, poly2, "g(x)");
    write_polynomial(output, poly3, "h(x)");
    fprintf(output, "\n");

    write_polynomial(output, sum12, "f(x) + g(x)");
    write_polynomial(output, sum123, "(f(x) + g(x)) + h(x)");
    write_polynomial(output, product23, "g(x) * h(x)");
    write_polynomial(output, product123, "f(x) * (g(x) * h(x))");
    fprintf(output, "\n");

    write_polynomial(output, composition_12, "f(g(x))");
    write_polynomial(output, composition_23, "g(h(x))");
    write_polynomial(output, composition_31, "h(f(x))");
    fprintf(output, "\n");

    write_polynomial(output, f5, "5 * f(x)");
    write_polynomial(output, g10, "10 * g(x)");
    write_polynomial(output, h333, "333 * h(x)");
    write_polynomial(output, fg_2, "(-2) * f(g(x))");
    fprintf(output, "\n");

    if (value1) {
        fprintf(output, "f(2): ");
        print_value_to_file(output, poly1->type, value1);
        fprintf(output, "\n");
        free(value1);
    }

    if (value2) {
        fprintf(output, "g(3): ");
        print_value_to_file(output, poly1->type, value2);
        fprintf(output, "\n");
    }

    if (value3) {
        fprintf(output, "h(-2): ");
        print_value_to_file(output, poly1->type, value3);
        fprintf(output, "\n");
    }

    if (value4) {
        fprintf(output, "f(-1) * (g(-1) * h(-1)): ");
        print_value_to_file(output, poly1->type, value4);
        fprintf(output, "\n");
    }
    

    free_polynomial(poly1);
    free_polynomial(poly2);
    free_polynomial(poly3);
    free_polynomial(sum12);
    free_polynomial(sum123);
    free_polynomial(product23);
    free_polynomial(product123);
    free_polynomial(composition_12);
    free_polynomial(composition_23);
    free_polynomial(composition_31);
    free_polynomial(f5);
    free_polynomial(g10);
    free_polynomial(h333);
    free_polynomial(fg_2);
    free(value1);
    free(value2);
    free(value3);
    free(value4);

    fclose(input);
    fclose(output);

    printf("Your code is ready");
    return 0;
}