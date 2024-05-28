#include "CUnit/Basic.h"
#include <stdio.h>
#include "buffer.h"

void buffer_put_test(void)
{
    int ret;
    Buffer *buffer = buffer_create(3);

    ret = buffer->put(buffer, 'c');

    CU_ASSERT_EQUAL(ret, 0);

    ret = buffer->put(buffer, 'd');

    CU_ASSERT_EQUAL(ret, 0);

    ret = buffer->put(buffer, 'e');

    CU_ASSERT_EQUAL(ret, 1);
}

void buffer_get_test(void)
{
    int ret;
    unsigned char c;

    Buffer *buffer = buffer_create(3);

    ret = buffer->get(buffer, &c);
    CU_ASSERT_EQUAL(ret, 1);

    ret = buffer->put(buffer, 'c');
    ret = buffer->get(buffer, &c);

    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(c, 'c');

    ret = buffer->put(buffer, 'd');
    ret = buffer->put(buffer, 'e');

    ret = buffer->put(buffer, 'f');
    CU_ASSERT_EQUAL(ret, 1);

    ret = buffer->get(buffer, &c);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(c, 'd');

    ret = buffer->get(buffer, &c);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(c, 'e');
}

void buffer_empty_test(void)
{
    unsigned char c;

    Buffer *buffer = buffer_create(3);

    CU_ASSERT_EQUAL(buffer->empty(buffer), 1);

    buffer->put(buffer, 'c');

    CU_ASSERT_EQUAL(buffer->empty(buffer), 0);

    buffer->get(buffer, &c);

    CU_ASSERT_EQUAL(buffer->empty(buffer), 1);
}

void buffer_full_test(void)
{
    int ret;
    unsigned char c;

    Buffer *buffer = buffer_create(2);

    CU_ASSERT_EQUAL(buffer->full(buffer), 0);

    ret = buffer->put(buffer, 'c');

    CU_ASSERT_EQUAL(buffer->full(buffer), 1);

    ret = buffer->put(buffer, 'c');

    CU_ASSERT_EQUAL(ret, 1);
 
    ret = buffer->get(buffer, &c);

    CU_ASSERT_EQUAL(buffer->full(buffer), 0);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("buffer_test_suite", 0, 0);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "buffer_put_test", buffer_put_test)) {
        CU_cleanup_registry();
    }    
    if (NULL == CU_add_test(pSuite, "buffer_get_test", buffer_get_test)) {
        CU_cleanup_registry();
    }  
    if (NULL == CU_add_test(pSuite, "buffer_empty_test", buffer_empty_test)) {
        CU_cleanup_registry();
    }  
    if (NULL == CU_add_test(pSuite, "buffer_full_test", buffer_full_test)) {
        CU_cleanup_registry();
    }    

    CU_basic_run_tests();
}