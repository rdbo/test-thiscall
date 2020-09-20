#include <stdio.h>
#include <stdlib.h>

#define VA_ARGS(...) , ##__VA_ARGS__
#ifdef __cplusplus
#define THISCALL(obj, func, ...) obj.func(__VA_ARGS__)
#else
#define THISCALL(obj, func, ...) obj.func(&obj VA_ARGS(__VA_ARGS__))
#endif

struct _test;

#ifdef __cplusplus
extern "C"
{
#endif

struct _test test_init();
void test_set_num(struct _test* thisptr, int number);

#ifdef __cplusplus
}
#endif

typedef struct _test
{
    int num;
#   ifdef __cplusplus
    void set_num(int number) { return test_set_num(this, number); }
#   else
    void(* set_num)(struct _test* thisptr, int number);
#   endif
}test;

struct _test test_init()
{
    struct _test ret;
    ret.num = 0;
#   ifndef __cplusplus
    ret.set_num = &test_set_num;
#   endif
    return ret;
}

void test_set_num(struct _test* thisptr, int number)
{
    thisptr->num = number;
    printf("num has been set to: %i\n", thisptr->num);
}

int main()
{
    test var = test_init();
#   ifdef __cplusplus
    var.set_num(10);
#   else
    var.set_num(&var, 10);
#   endif

    THISCALL(var, set_num, 100);

    return 0;
}