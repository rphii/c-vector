#ifndef PERSON_H

#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#include "../src/vec.h"


#define STR_DEFAULT_SIZE    8

typedef struct Str {
    char *s;    /* string */
    size_t l;   /* length */
    size_t c;   /* capacity */
} Str;

int str_app(Str *str, char *format, ...);
int str_cap_ensure(Str *str, size_t cap);
void str_free_single(Str *str);



typedef struct Person
{
    int age;
    Str name;
} Person;

VEC_INCLUDE(VecPersonV, vec_person_v, Person, BY_VAL);
VEC_INCLUDE(VecPersonR, vec_person_r, Person, BY_REF);

void person_test_by_val(time_t t);
void person_test_by_ref(time_t t);

void person_print(Person *person);
int person_random(Person *person);
void person_free(Person *person);

#define PERSON_H
#endif

