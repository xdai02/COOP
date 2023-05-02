#include "test_stack.h"
#include "coop.h"
#include <assert.h>

void test_stack_create() {
    stack_t *stack = stack_create(UnsignedCharacter_delete);
    assert(stack != NULL);
    assert(stack_is_empty(stack) == true);
    assert(stack_size(stack) == 0);
    stack_destroy(stack);
}

void test_stack_destroy() {
    stack_t *stack = stack_create(UnsignedCharacter_delete);
    assert(stack != NULL);
    stack_destroy(stack);
}

void test_stack_is_empty() {
    stack_t *stack = stack_create(UnsignedInteger_delete);
    assert(stack_is_empty(stack) == true);
    stack_push(stack, UnsignedInteger_new(0));
    assert(stack_is_empty(stack) == false);
    stack_destroy(stack);
}

void test_stack_size() {
    const int N = 1000;
    int i = 0;
    stack_t *stack = stack_create(UnsignedLong_delete);
    assert(stack_size(stack) == 0);
    for (i = 0; i < N; i++) {
        stack_push(stack, UnsignedLong_new(i));
    }
    assert(stack_size(stack) == N);
    stack_destroy(stack);
}

void test_stack_clear() {
    const int N = 100;
    int i = 0;
    stack_t *stack = stack_create(Short_delete);
    for (i = 0; i < N; i++) {
        stack_push(stack, Short_new(i));
    }
    assert(stack_size(stack) == N);
    stack_clear(stack);
    assert(stack_size(stack) == 0);
    stack_destroy(stack);
}

void test_stack_push() {
    const int N = 100;
    int i = 0;
    stack_t *stack = stack_create(Integer_delete);
    for (i = 0; i < N; i++) {
        stack_push(stack, Integer_new(i));
    }
    assert(stack_size(stack) == N);
    stack_destroy(stack);
}

void test_stack_pop() {
    const int N = 100;
    int i = 0;
    stack_t *stack = NULL;
    Integer *integer;

    stack = stack_create(Integer_delete);
    for (i = 0; i < N; i++) {
        stack_push(stack, Integer_new(i));
    }
    for (i = 0; i < N; i++) {
        integer = (Integer *)stack_pop(stack);
        assert(Integer_valueOf(integer) == N - i - 1);
        Integer_delete(integer);
    }
    assert(stack_size(stack) == 0);
    stack_destroy(stack);
}

void test_stack_peek() {
    const int N = 100;
    int i = 0;
    stack_t *stack = NULL;
    Integer *integer;

    stack = stack_create(Integer_delete);
    for (i = 0; i < N; i++) {
        stack_push(stack, Integer_new(i));
    }
    for (i = 0; i < N; i++) {
        assert(Integer_valueOf(stack_peek(stack)) == N - i - 1);
        integer = (Integer *)stack_pop(stack);
        Integer_delete(integer);
    }
    assert(stack_size(stack) == 0);
    stack_destroy(stack);
}
