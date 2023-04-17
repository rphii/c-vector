#include "person.h"

VEC_IMPLEMENT(VecPersonV, vec_person_v, Person, BY_VAL, 0);
VEC_IMPLEMENT(VecPersonR, vec_person_r, Person, BY_REF, 0);


int main(void)
{
    printf("Hello\n");
    return 0;
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


