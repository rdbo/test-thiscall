#include <stdio.h>
#include <stdlib.h>

#define VA_ARGS(...) , ##__VA_ARGS__
#define DEF_CPP_THISCALL(ret_type, c_fn, cpp_fn, ...) \
    ret_type cpp_fn(VA_ARGS(__VA_ARGS__))\
    {\
        c_fn(this VA_ARGS(__VA_ARGS__));\
    }\

#ifdef __cplusplus
#define THISCALL(obj, fn, ...) obj.fn(VA_ARGS(__VA_ARGS__))
#else
#define THISCALL(obj, fn, ...) obj.fn(&obj VA_ARGS(__VA_ARGS__))
#endif

struct _test; //Early declaration of struct _test
void test_print(struct _test* thisptr); //Early declaration of C function

typedef struct _test
{
    int number;
#   ifdef __cplusplus
    DEF_CPP_THISCALL(void, test_print, print) //Declaration of the C++ function
#   else
    void(* print)(struct _test* thisptr);     //Declaration of the pointer to the C function
#   endif
}test;

void test_print(struct _test* thisptr)
{
    printf("Number: %i\n", thisptr->number);
}

test test_init()
{
    test ret;
    ret.number = 10;
#   ifndef __cplusplus
    ret.print  = &test_print;
#   endif
    return ret;
}

int main()
{
    test var = test_init();
#   ifdef __cplusplus
    var.print(); //Doesn't need the 'thisptr' parameter explicitly (C++)
#   else
    var.print(&var); //Needs the 'thisptr' parameters explicitly (C)
#   endif

    var.number = 20;

    THISCALL(var, print); //One line to replace the code above

    return 0;
}