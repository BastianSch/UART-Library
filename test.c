#include "CUnit/Basic.h"
#include <stdio.h>
#include "uart.h"

void fifo_put_test(void)
{
    int ret;
    fifo_t fifo = {.data={}, .size=3, .read=0, .write=0};

    ret = fifo_put(&fifo, 'c');

    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(fifo.write, 1);
    CU_ASSERT_EQUAL(fifo.data[0], 'c');

    ret = fifo_put(&fifo, 'd');

    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(fifo.write, 2);
    CU_ASSERT_EQUAL(fifo.data[1], 'd'); 

    ret = fifo_put(&fifo, 'd');

    CU_ASSERT_EQUAL(ret, 1);
    CU_ASSERT_EQUAL(fifo.write, 2); 
}

void fifo_get_test(void)
{
    int ret;
    unsigned char c;

    fifo_t fifo = {.data={}, .size=3, .read=0, .write=0};

    ret = fifo_get(&fifo, c);
    CU_ASSERT_EQUAL(ret, 1);

    ret = fifo_put(&fifo, 'c');
    ret = fifo_get(&fifo, c);

    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(fifo.read, 1);
    CU_ASSERT_EQUAL(c, 'c');

    ret = fifo_put(&fifo, 'd');
    ret = fifo_put(&fifo, 'e');

    ret = fifo_get(&fifo, c);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(fifo.read, 2);
    CU_ASSERT_EQUAL(c, 'd');

    ret = fifo_get(&fifo, c);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(fifo.read, 0);
    CU_ASSERT_EQUAL(c, 'e');
}

void fifo_empty_test(void)
{
    unsigned char c;

    fifo_t fifo = {.data={}, .size=8, .read=0, .write=0};

    CU_ASSERT_EQUAL(fifo_empty(&fifo), 1);

    fifo_put(&fifo, 'a');

    CU_ASSERT_EQUAL(fifo_empty(&fifo), 0);

    fifo_get(&fifo, &c);

    CU_ASSERT_EQUAL(fifo_empty(&fifo), 1);
}

void fifo_full_test(void)
{
    int ret;
    unsigned char c;

    fifo_t fifo = {.data={}, .size=2, .read=0, .write=0};

    CU_ASSERT_EQUAL(fifo_full(&fifo), 0);

    ret = fifo_put(&fifo, 'a');

    CU_ASSERT_EQUAL(fifo_full(&fifo), 1);

    ret = fifo_put(&fifo, 'a');

    CU_ASSERT_EQUAL(ret, 1);
 
    ret = fifo_get(&fifo, &c);

    CU_ASSERT_EQUAL(fifo_full(&fifo), 0);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("fifo_test_suite", 0, 0);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "fifo_put_test", fifo_put_test)) {
        CU_cleanup_registry();
    }    
    if (NULL == CU_add_test(pSuite, "fifo_empty_test", fifo_empty_test)) {
        CU_cleanup_registry();
    }
    if (NULL == CU_add_test(pSuite, "fifo_full_test", fifo_full_test)) {
        CU_cleanup_registry();
    }

    CU_basic_run_tests();
}