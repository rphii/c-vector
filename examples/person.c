#include "person.h"

#include <stdlib.h>
#include <time.h>

VEC_IMPLEMENT(VecPersonV, vec_person_v, Person, BY_VAL, person_free);
VEC_IMPLEMENT(VecPersonR, vec_person_r, Person, BY_REF, person_free);

int main(void)
{
    time_t t = time(0);

    person_test_by_val(t);
    person_test_by_ref(t);

    printf("\ndone\n");
    return 0;
}

void person_test_by_val(time_t t)
{
    srand(time(&t));
    printf("--- test by val ---\n");
    VecPersonV vec = {0};
    Person p = {0};

    for(size_t i = 0; i < 100; i++) {
        person_random(&p);
        vec_person_v_push_back(&vec, p);
    }

    for(size_t i = 0; i < 100; i++) {
        vec_person_v_pop_front(&vec, &p);
        printf("[%zu] ", i);
        person_print(&p);
    }

    vec_person_v_free(&vec);
}

void person_test_by_ref(time_t t)
{
    srand(time(&t));
    printf("--- test by ref ---\n");
    VecPersonR vec = {0};
    Person p = {0};

    for(size_t i = 0; i < 100; i++) {
        person_random(&p);
        vec_person_r_push_back(&vec, &p);
    }

    for(size_t i = 0; i < 100; i++) {
        vec_person_r_pop_front(&vec, &p);
        printf("[%zu] ", i);
        person_print(&p);
    }

    vec_person_r_free(&vec);
}


void person_print(Person *person)
{
    assert(person);
    printf("age: %u, name: %.*s\n", person->age, (int)person->name.l, person->name.s);
}

int person_random(Person *person)
{
    assert(person);
    int result = 0;
    memset(person, 0, sizeof(*person));
    person->age = rand() % 100;
    size_t name_len = rand() % 10 + 3;
    for(size_t i = 0; i < name_len; i++) {
        result |= result ?: str_app(&person->name, "%c", rand() % ('Z' - 'A') + 'A');
    }
    return result;
}

void person_free(Person *person)
{
    assert(person);
    str_free_single(&person->name);
    memset(person, 0, sizeof(*person));
}


int str_app(Str *str, char *format, ...)
{
    assert(str);
    assert(format);
    // calculate length of append string
    va_list argp;
    va_start(argp, format);
    int len_app = vsnprintf(0, 0, format, argp);
    va_end(argp);
    // calculate required memory
    size_t len_new = str->l + len_app;
    size_t required = str->c ? str->c : STR_DEFAULT_SIZE;
    while(required < len_new) required = required << 1;
    // make sure to have enough memory
    if(required > str->c)
    {
        char *temp = realloc(str->s, required);
        // safety check
        // apply address and set new allocd
        if(!temp) return -1;
        str->s = temp;
        str->c = required;
    }
    // actual append
    va_start(argp, format);
    int len_chng = vsnprintf(&(str->s)[str->l], len_app + 1, format, argp);
    va_end(argp);
    // check for success
    if(len_chng >= 0 && len_chng <= len_app) {
        str->l += len_chng; // successful, change length
    } else {
        return -1;
    }
    return 0;
}

int str_cap_ensure(Str *str, size_t cap)
{
    assert(str);
    if(cap) {
        /* calculate required memory */
        size_t required = str->c ? str->c : STR_DEFAULT_SIZE;
        while(required < cap) required *= 2;
        /* only increase capacity */
        if(required > str->c) {
            void *temp = realloc(str->s, sizeof(*str->s) * cap);
            if(!temp) return -1;
            str->s = temp;
            str->c = required;
        }
    }
    return 0;
}

void str_free_single(Str *str)
{
    assert(str);
    free(str->s);
    str->s = 0;
    str->c = 0;
    str->l = 0;
}


