/******************************************************************************
 *   Heap DST: Heap or priority queue.                                        *
 *             Implemented as a pairing heap (in a half-ordered binary tree)  *
 *   2011-2015 Victor Eduardo Cardoso Nungaray                                *
 *   Twitter: @victore_cardoso                                                *
 *   email: victorc@cimat.mx                                                  *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "heap_tree.h"
#include "heap_dst.h"
#include "heap_struct.h"

static void* malloc_heap(void);
static htree_t* cut_root(heap_t *heap);
static bool is_not_empty(const heap_t *const heap);

inline uint16_t heap_get_memsize(void)
{
	return sizeof(heap_t);
}

void heap_init(void *heap_ptr)
{
	heap_t *heap = heap_ptr;
	heap->length = 0;
	heap->root = NULL;
}

void heap_copy(void *heap_ptr, const void *src_heap_ptr,
	       void* (*clone)(const void*))
{
	const heap_t *src_heap = src_heap_ptr;
	if (is_not_empty(src_heap)) {
		heap_t *heap = heap_ptr;
		heap->root = htree_clone(src_heap->root, clone);
		heap->length = src_heap->length;
	}
}


void heap_clear(void *heap_ptr,
		void (*destroy)(void*))
{
	heap_t *heap = heap_ptr;
	if (is_not_empty(heap)) {
		htree_destroy_recursively(heap->root, destroy);
		heap->root = NULL;
		heap->length = 0;
	}
}

void* heap_create(void)
{
	void *heap = malloc_heap();
	heap_init(heap);
	return heap;
}

static void* malloc_heap(void)
{
	uint16_t size = sizeof(heap_t);
	return malloc(size);
}

void* heap_clone(const void *const heap_ptr,
		 void* (*clone)(const void*))
{
	void *heap = malloc_heap();
	heap_init(heap);
	heap_copy(heap, heap_ptr, clone);
	return heap;
}

inline void heap_destroy(void *heap_ptr,
			 void (*destroy)(void*))
{
	heap_clear(heap_ptr, destroy);
	free(heap_ptr);
}


void heap_merge(void *heap1_ptr, void *heap2_ptr,
		uint32_t (*key)(const void *const),
		int8_t (*compare)(const void*, const void*))
{
	heap_t *heap1 = heap1_ptr;
	heap_t *heap2 = heap2_ptr;
	if (is_not_empty(heap2)) {
		heap1->length += heap2->length;
		heap1->root = htree_link(heap1->root, heap2->root, key);
		heap2->root = NULL;
		heap2->length = 0;
	}
}

static inline htree_t* cut_root(heap_t *restrict heap)
{
	htree_t *root = heap->root;
	heap->root = NULL;
	return root;
}
static inline bool is_not_empty(const heap_t *const restrict heap)
{
	return (NULL != heap->root);
}

bool heap_insert(void *heap_ptr, const void *const  val,
		 uint32_t (*key)(const void *const),
		 int8_t (*compare)(const void*, const void*))
{
	heap_t *heap = heap_ptr;
	htree_t* new_tree = htree_create();
	new_tree->val = (void*) val;
	if (is_not_empty(heap))
		heap->root = htree_link(heap->root, new_tree, key);
	else
		heap->root = new_tree;
	heap->length += 1;
	return true;
}

inline void* heap_get_first(const void *const heap_ptr)
{
	const heap_t *const restrict heap = heap_ptr;
	void *val = NULL;
	if (is_not_empty(heap))
		val = heap->root->val;
	return val;
}

void* heap_delete_first(void *heap_ptr,
			uint32_t (*key)(const void *const))
{
	heap_t *heap = heap_ptr;
	void *deleted_val = NULL;
	if (is_not_empty(heap)) {
		deleted_val = heap->root->val;
		heap->root = htree_delete_and_get_new_root(heap->root, key);
		heap->length -= 1;
	}
	return deleted_val;
}


void* heap_exist(const void *const heap_ptr, const void *val,
		 uint32_t (*key)(const void*),
		 int8_t (*compare)(const void*, const void*))
{
	const heap_t *const heap = heap_ptr;
	void *existing_val = NULL;
	if (is_not_empty(heap)) {
		htree_t *tree = htree_containing_val(heap->root, val,
						     key, compare);
		if (NULL != tree)
			existing_val = tree->val;
	}
	return existing_val;
}

void* heap_delete(void *heap_ptr, const void *val,
		  uint32_t (*key)(const void*),
		  int8_t (*compare)(const void*, const void*))
{
	heap_t *heap = heap_ptr;
	void *deleted_val = NULL;
	if (is_not_empty(heap)) {
		htree_t *tree = htree_containing_val(heap->root, val,
						     key, compare);
		if (NULL != tree) {
			deleted_val = tree->val;
			htree_delete(tree, key);
			heap->length -= 1;
		}
	}
	return deleted_val;
}

inline uint32_t heap_get_length(const void *const heap_ptr)
{
	const heap_t *const restrict heap = heap_ptr;
	return heap->length;
}

inline bool heap_is_empty(const void *const heap_ptr)
{
	const heap_t *const restrict heap = heap_ptr;
	return (NULL == heap->root);
}

inline bool heap_is_not_empty(const void *const heap_ptr)
{
	return is_not_empty(heap_ptr);
}
