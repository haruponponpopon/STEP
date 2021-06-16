////////////////////////////////////////////////////////////////////////////////
/*                 (๑＞◡＜๑)  Malloc Challenge!!  (◍＞◡＜◍)                   */
////////////////////////////////////////////////////////////////////////////////

//
// Welcome to Malloc Challenge!! Your job is to invent a smart malloc algorithm.
//
// Rules:
//
// 1. Your job is to implement my_malloc(), my_free() and my_initialize().
//   *  my_initialize() is called only once at the beginning of each challenge.
//      You can initialize the memory allocator.
//   *  my_malloc(size) is called every time an object is allocated. In this
//      challenge, |size| is guaranteed to be a multiple of 8 bytes and meets
//      8 <= size <= 4000.
//   * my_free(ptr) is called every time an object is freed.
//   * Additionally, my_finalize() is called only once at the end of each
//   challenge,
//     so you can use this function for doing some clean ups if you want.
// 2. The only library functions you can use in my_malloc() and my_free() are
//    mmap_from_system() and munmap_to_system().
//   *  mmap_from_system(size) allocates |size| bytes from the system. |size|
//      needs to be a multiple of 4096 bytes. mmap_from_system(size) is a
//      system call and heavy. You are expected to minimize the call of
//      mmap_from_system(size) by reusing the returned
//      memory region as much as possible.
//   *  munmap_to_system(ptr, size) frees the memory region [ptr, ptr + size)
//      to the system. |ptr| and |size| need to be a multiple of 4096 bytes.
//      You are expected to free memory regions that are unused.
//   *  You are NOT allowed to use any other library functions at all, including
//      the default malloc() / free(), std:: libraries etc. This is because you
//      are implementing malloc itself -- if you use something that may use
//      malloc internally, it will result in an infinite recurion.
// 3. simple_malloc(), simple_free() and simple_initialize() in simple_malloc.c
//    are an example of straightforward implementation.
//    Your job is to invent a smarter malloc algorithm than the simple malloc.
// 4. There are five challenges (Challenge 1, 2, 3, 4 and 5). Each challenge
//    allocates and frees many objects with different patterns. Your malloc
//    is evaluated by two criteria.
//   *  [Speed] How faster your malloc finishes the challange compared to
//      the simple malloc.
//   *  [Memory utilization] How much your malloc is memory efficient.
//      This is defined as (S1 / S2), where S1 is the total size of objects
//      allocated at the end of the challange and S2 is the total size of
//      mmap_from_system()ed regions at the end of the challenge. You can
//      improve the memory utilization by decreasing memory fragmentation and
//      reclaiming unused memory regions to the system with munmap_to_system().
// 5. This program works on Linux and Mac but not on Windows. If you don't have
//    Linux or Mac, you can use Google Cloud Shell (See
//    https://docs.google.com/document/d/1TNu8OfoQmiQKy9i2jPeGk1DOOzSVfbt4RoP_wcXgQSs/edit#).
// 6. You need to specify an '-lm' option to compile this program.
//   *  gcc malloc_challenge.c -lm
//   *  clang malloc_challenge.c -lm
//
// Enjoy! :D
//

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
  struct my_metadata_t *previous;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t arr[4000/8+1];
  my_metadata_t dummy;
} my_heap_t;

my_heap_t my_heap;

// Add a free slot to the beginning of the free list.
void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next);  //nextに何かしら入っていたらエラー
  assert(!metadata->previous); 
  assert(metadata->size);
  assert(metadata->size%8==0);
  size_t size = metadata->size;
  if (my_heap.arr[size/8].next==NULL){ //arrの指定したsizeに何も入っていない
    my_heap.arr[size/8].next = metadata;
    metadata->previous = &my_heap.arr[size/8];
    metadata->next = NULL; 
  }else{ //arrの指定したsizeに入っていた
    metadata->next = my_heap.arr[size/8].next;
    metadata->next->previous = metadata;
    my_heap.arr[size/8].next = metadata;
    metadata->previous = &my_heap.arr[size/8];
  }
}

// Remove a free slot from the free list.
void my_remove_from_free_list(my_metadata_t *metadata) {
  assert(metadata->size);
  assert(metadata->previous);
  assert(metadata->size%8==0);
  size_t size = metadata->size;
  if (metadata->next==NULL) {  //arrに要素が一つ
    metadata->previous->next = NULL;
  } else {
    metadata->previous->next = metadata->next;
    metadata->next->previous = metadata->previous;
  }
  metadata->next = NULL;
  metadata->previous = NULL;
}

// my_initialize() is called only once at the beginning of each challenge.
void my_initialize() {
  // Implement here!
  for (int i=0; i<=4096/8; i++){
    my_heap.arr[i].size = i*8;
    my_heap.arr[i].next = NULL;
    my_heap.arr[i].previous = NULL;
  }
  my_heap.dummy.size = 0;
  my_heap.dummy.next = NULL;
}



//一番サイズの小さい領域に辞書を使って入れる
void *best_fit_malloc(size_t size){
  assert(size%8==0);
  assert(size<=4000);
  int use_size = -1;
  for (int i=(int)size/8; i<=4096/8; i++){ //fitする最小の領域
    if (my_heap.arr[i].next){
      use_size = i;
      break;
    }
  }

  if (use_size==-1) { //その容量を持つところは見当たらない
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    size_t buffer_size = 4096;
    my_metadata_t *metadata =
        (my_metadata_t *)mmap_from_system(buffer_size);
    assert((buffer_size - sizeof(my_metadata_t))%8==0); //sizeが8の倍数でなかったらエラー
    assert((buffer_size - sizeof(my_metadata_t))<=4096);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->next = NULL;
    metadata->previous = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata);
    // Now, try simple_malloc() again. This should succeed.
    return best_fit_malloc(size);
  }
  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  void *ptr =  my_heap.arr[use_size].next + 1;
  size_t remaining_size = my_heap.arr[use_size].size - size;
  my_metadata_t *metadata = my_heap.arr[use_size].next;
  assert(metadata->size);
  assert(metadata->previous);
  // Remove the free slot from the free list.
  my_remove_from_free_list(metadata);
  if (remaining_size > sizeof(my_metadata_t)) {
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    new_metadata->previous = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }
  return ptr;
}


// my_malloc() is called every time an object is allocated. |size| is guaranteed
// to be a multiple of 8 bytes and meets 8 <= |size| <= 4000. You are not
// allowed to use any library functions other than mmap_from_system /
// munmap_to_system.
void *my_malloc(size_t size) {
  return best_fit_malloc(size);
}
/*
// my_free() is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Implement here!
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  my_add_to_free_list(metadata);
  munmap_to_system(ptr, 4096);
}*/

// This is called every time an object is freed.  You are not allowed to use
// any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  my_add_to_free_list(metadata);
}

void my_finalize() {
  // Implement here!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
