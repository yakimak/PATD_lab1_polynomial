#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdlib.h>

typedef enum {
    INT_TYPE,
    DOUBLE_TYPE
} DataType;
//узнать и исбавиться


typedef struct {
    size_t size;                          
    void (*print)(void*);   //tostring sprintf(buff, "...", x)    возвращает char*   чтобы неважно в файл или не в файлы      
    void (*sum)(void*, void*, void*);      
    void (*multiply)(void*, void*, void*); 
    void (*multiply_scalar)(void*, double, void*); 
    DataType type;                        
} typeInfo;

#endif