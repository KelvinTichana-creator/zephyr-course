/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * L8 Task 1:
 * Complete the 7 ZTEST bodies for the ring_buf module.
 *
 * Run:
 * west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>

#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook:
 * Every suite reinitialises the ring buffer with capacity 4.
 */
static void before(void *f)
{
	ARG_UNUSED(f);

	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 * ============================================================================
 */

ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — worked example. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(),
		     "Fresh buffer must be empty");

	zassert_equal(rb_count(), 0,
		      "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	int ret;

	ret = rb_push(99);
	zassert_equal(ret, 0,
		      "Initial push must succeed");

	rb_init(4);

	zassert_true(rb_is_empty(),
		     "Reinitialised buffer must be empty");

	zassert_equal(rb_count(), 0,
		      "Reinitialised buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 * ============================================================================
 */

ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int ret;
	int value = 0;

	ret = rb_push(42);
	zassert_equal(ret, 0,
		      "Push of 42 must succeed");

	ret = rb_pop(&value);
	zassert_equal(ret, 0,
		      "Pop must succeed");

	zassert_equal(value, 42,
		      "Popped value must be 42");

	zassert_true(rb_is_empty(),
		     "Buffer must be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int ret;
	int value;

	ret = rb_push(1);
	zassert_equal(ret, 0,
		      "Push of 1 must succeed");

	ret = rb_push(2);
	zassert_equal(ret, 0,
		      "Push of 2 must succeed");

	ret = rb_push(3);
	zassert_equal(ret, 0,
		      "Push of 3 must succeed");

	ret = rb_pop(&value);
	zassert_equal(ret, 0,
		      "First pop must succeed");
	zassert_equal(value, 1,
		      "First popped value must be 1");

	ret = rb_pop(&value);
	zassert_equal(ret, 0,
		      "Second pop must succeed");
	zassert_equal(value, 2,
		      "Second popped value must be 2");

	ret = rb_pop(&value);
	zassert_equal(ret, 0,
		      "Third pop must succeed");
	zassert_equal(value, 3,
		      "Third popped value must be 3");

	zassert_true(rb_is_empty(),
		     "Buffer must be empty after three pops");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	int ret;

	ret = rb_push(1);
	zassert_equal(ret, 0,
		      "Push 1 must succeed");

	ret = rb_push(2);
	zassert_equal(ret, 0,
		      "Push 2 must succeed");

	ret = rb_push(3);
	zassert_equal(ret, 0,
		      "Push 3 must succeed");

	ret = rb_push(4);
	zassert_equal(ret, 0,
		      "Push 4 must succeed");

	zassert_true(rb_is_full(),
		     "Buffer must be full after four pushes");

	ret = rb_push(99);
	zassert_equal(ret, -ENOSPC,
		      "Push into full buffer must return -ENOSPC");

	zassert_equal(rb_count(), 4,
		      "Rejected push must not change count");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 * ============================================================================
 */

ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int ret;
	int value = 0;

	ret = rb_push(7);
	zassert_equal(ret, 0,
		      "Push of 7 must succeed");

	ret = rb_peek(&value);
	zassert_equal(ret, 0,
		      "First peek must succeed");
	zassert_equal(value, 7,
		      "First peek must return 7");

	ret = rb_peek(&value);
	zassert_equal(ret, 0,
		      "Second peek must succeed");
	zassert_equal(value, 7,
		      "Second peek must return 7");

	zassert_equal(rb_count(), 1,
		      "Peek must not consume the value");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	int ret;

	ret = rb_pop(NULL);

	zassert_equal(ret, -EINVAL,
		      "rb_pop(NULL) must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	int ret;

	ret = rb_push(1);
	zassert_equal(ret, 0,
		      "Push 1 must succeed");

	ret = rb_push(2);
	zassert_equal(ret, 0,
		      "Push 2 must succeed");

	ret = rb_push(3);
	zassert_equal(ret, 0,
		      "Push 3 must succeed");

	ret = rb_push(4);
	zassert_equal(ret, 0,
		      "Push 4 must succeed");

	zassert_true(rb_is_full(),
		     "Buffer must be full after four pushes");

	zassert_equal(rb_count(), 4,
		      "Full buffer count must be 4");
}
