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
  struct my_metadata_t *parent;
  struct my_metadata_t *left;
  struct my_metadata_t *right;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

my_heap_t my_heap;

// Add a free slot to the beginning of the free list.
void my_add_to_free_list(my_metadata_t *metadata, my_metadata_t *prev, int child) {
  if (prev){
    assert(!metadata->parent);
    metadata->parent = prev;
    if (child==1){
      assert(!metadata->left);
      prev->left = metadata;
    }else if (child==2){
      assert(!metadata->right);
      prev->right = metadata;
    }else{
      assert(1==2);
    }
  }else{
    my_heap.free_head = metadata;
  }
}

// Remove a free slot from the free list.
void my_remove_from_free_list(my_metadata_t *metadata,
                                  my_metadata_t *prev) {
  if (prev) {
    if (prev->left==metadata){
      if (metadata->left&&metadata->right){
        my_metadata_t *current = metadata->right;
      }
      //////////////////ミス

    }
    prev->left = metadata->left;
    prev->right = metadata->right;
  } else {
    my_heap.free_head = &my_heap.dummy;
  }
  metadata->left = NULL;
  metadata->right = NULL;
}

// my_initialize() is called only once at the beginning of each challenge.
void my_initialize() {
  // Implement here!
  my_heap.free_head = &my_heap.dummy;
  my_heap.dummy.size = 0;
  my_heap.dummy.left = NULL;
  my_heap.dummy.right = NULL;
}

// //最初に見つかった領域に入れる
// void *first_fit_malloc(size_t size){
//   // Implement here!
//   my_metadata_t *metadata = my_heap.free_head;
//   my_metadata_t *prev = NULL;
//   // First-fit: Find the first free slot the object fits.
//   while (metadata && metadata->size < size) {
//     prev = metadata;
//     metadata = metadata->next;
//   }
//   if (!metadata) {
//     // There was no free slot available. We need to request a new memory region
//     // from the system by calling mmap_from_system().
//     //
//     //     | metadata | free slot |
//     //     ^
//     //     metadata
//     //     <---------------------->
//     //            buffer_size
//     size_t buffer_size = 4096;
//     my_metadata_t *metadata =
//         (my_metadata_t *)mmap_from_system(buffer_size);
//     metadata->size = buffer_size - sizeof(my_metadata_t);
//     metadata->next = NULL;
//     // Add the memory region to the free list.
//     my_add_to_free_list(metadata);
//     // Now, try simple_malloc() again. This should succeed.
//     return first_fit_malloc(size);
//   }
//   // |ptr| is the beginning of the allocated object.
//   //
//   // ... | metadata | object | ...
//   //     ^          ^
//   //     metadata   ptr
//   void *ptr = metadata + 1;
//   size_t remaining_size = metadata->size - size;
//   metadata->size = size;
//   // Remove the free slot from the free list.
//   my_remove_from_free_list(metadata, prev);

//   if (remaining_size > sizeof(my_metadata_t)) {
//     // Create a new metadata for the remaining free slot.
//     //
//     // ... | metadata | object | metadata | free slot | ...
//     //     ^          ^        ^
//     //     metadata   ptr      new_metadata
//     //                 <------><---------------------->
//     //                   size       remaining size
//     my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
//     new_metadata->size = remaining_size - sizeof(my_metadata_t);
//     new_metadata->next = NULL;
//     // Add the remaining free slot to the free list.
//     my_add_to_free_list(new_metadata);
//   }
//   return ptr;
// }

// //一番サイズの大きい領域に入れる
// void *worst_fit_malloc(size_t size){
//   my_metadata_t *metadata = NULL;
//   my_metadata_t *prev = NULL;
//   my_metadata_t *current_data = my_heap.free_head;
//   my_metadata_t *current_prev = NULL;

//   size_t max_size = 0;
//   while (current_data) {
//     if (current_data->size >= size && current_data->size>max_size){
//       prev = current_prev;
//       metadata = current_data;
//       max_size = current_data->size;
//     }
//     current_prev = current_data;
//     current_data = current_data->next;
//   }


//   if (!metadata) {
//     size_t buffer_size = 4096;
//     my_metadata_t *metadata =
//         (my_metadata_t *)mmap_from_system(buffer_size);
//     metadata->size = buffer_size - sizeof(my_metadata_t);
//     metadata->next = NULL;
//     // Add the memory region to the free list.
//     my_add_to_free_list(metadata);
//     // Now, try simple_malloc() again. This should succeed.
//     return worst_fit_malloc(size);
//   }
//   void *ptr = metadata + 1;
//   size_t remaining_size = metadata->size - size;
//   metadata->size = size;
//   // Remove the free slot from the free list.
//   my_remove_from_free_list(metadata, prev);

//   if (remaining_size > sizeof(my_metadata_t)) {
//     my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
//     new_metadata->size = remaining_size - sizeof(my_metadata_t);
//     new_metadata->next = NULL;
//     // Add the remaining free slot to the free list.
//     my_add_to_free_list(new_metadata);
//   }
//   return ptr;
// }

// //一番サイズの小さい領域に入れる
// void *best_fit_malloc(size_t size){
//   my_metadata_t *metadata = NULL;
//   my_metadata_t *prev = NULL;
//   my_metadata_t *current_data = my_heap.free_head;
//   my_metadata_t *current_prev = NULL;
//   size_t min_size = 5000;
//   while (current_data) {
//     if (current_data->size >= size && current_data->size<min_size){
//       metadata = current_data;
//       prev = current_prev;
//       min_size = current_data->size;
//     }
//     current_prev = current_data;
//     current_data = current_data->next;
//   }


//   if (!metadata) {
//     // There was no free slot available. We need to request a new memory region
//     // from the system by calling mmap_from_system().
//     //
//     //     | metadata | free slot |
//     //     ^
//     //     metadata
//     //     <---------------------->
//     //            buffer_size
//     size_t buffer_size = 4096;
//     my_metadata_t *metadata =
//         (my_metadata_t *)mmap_from_system(buffer_size);
//     metadata->size = buffer_size - sizeof(my_metadata_t);
//     metadata->next = NULL;
//     // Add the memory region to the free list.
//     my_add_to_free_list(metadata);
//     // Now, try simple_malloc() again. This should succeed.
//     return best_fit_malloc(size);
//   }
//   // |ptr| is the beginning of the allocated object.
//   //
//   // ... | metadata | object | ...
//   //     ^          ^
//   //     metadata   ptr
//   void *ptr = metadata + 1;
//   size_t remaining_size = metadata->size - size;
//   metadata->size = size;
//   // Remove the free slot from the free list.
//   my_remove_from_free_list(metadata, prev);

//   if (remaining_size > sizeof(my_metadata_t)) {
//     // Create a new metadata for the remaining free slot.
//     //
//     // ... | metadata | object | metadata | free slot | ...
//     //     ^          ^        ^
//     //     metadata   ptr      new_metadata
//     //                 <------><---------------------->
//     //                   size       remaining size
//     my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
//     new_metadata->size = remaining_size - sizeof(my_metadata_t);
//     new_metadata->next = NULL;
//     // Add the remaining free slot to the free list.
//     my_add_to_free_list(new_metadata);
//   }
//   return ptr;
// }

//一番サイズの小さい領域に入れる
void *best_fit_malloc2(size_t size){
  my_metadata_t *metadata = NULL;
  my_metadata_t *prev = NULL;
  my_metadata_t *current_prev = NULL;
  my_metadata_t *current_data = my_heap.free_head;
  size_t min_size = 5000;
  int direct = -1;
  int child = -1;

  while (current_data) {
    if (size>current_data->size){
      current_prev = current_data;
      current_data = current_data->right;
      direct = 2;
    }else{
      if (current_data->size<min_size){
        min_size = current_data->size;
        metadata = current_data;
        prev = current_prev;
        child = direct;
      }
      current_prev = current_data;
      current_data = current_data->left;
      direct = 1;
    }
  }
  


  if (!metadata) {
    size_t buffer_size = 4096;
    my_metadata_t *metadata =
        (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->right = NULL;
    metadata->left = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata, prev, direct);
    // Now, try simple_malloc() again. This should succeed.
    return best_fit_malloc2(size);
  }
  void *ptr = metadata + 1;
  size_t remaining_size = metadata->size - size;
  metadata->size = size;
  // Remove the free slot from the free list.
  my_remove_from_free_list(metadata, prev);

  if (remaining_size > sizeof(my_metadata_t)) {
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->left = NULL;
    new_metadata->right = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata, prev, child);
  }
  return ptr;
}
void print_tree(my_metadata_t *node){
  if (node){
    printf("%zd ", node->size);
    if (node->right)print_tree(node->right);
    if (node->left)print_tree(node->left);
  }
}


// my_malloc() is called every time an object is allocated. |size| is guaranteed
// to be a multiple of 8 bytes and meets 8 <= |size| <= 4000. You are not
// allowed to use any library functions other than mmap_from_system /
// munmap_to_system.
//ここの関数を書き換えると別の挙動を示すようになる
void *my_malloc(size_t size) {
  // if (my_heap.free_head->right){
  //   printf("------\n");
  //   print_tree(my_heap.free_head);
  // }

  // return first_fit_malloc(size);
  // return best_fit_malloc(size);
  // return worst_fit_malloc(size);
  return best_fit_malloc2(size);
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
  //prevを探す
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  my_metadata_t *current_prev = NULL;
  my_metadata_t *current_data = my_heap.free_head;
  int direct = -1;
  while (current_data) {
    if (metadata->size>current_data->size){
      current_prev = current_data;
      current_data = current_data->right;
      direct = 2;
    }else if (metadata->size<=current_data->size){
      current_prev = current_data;
      current_data = current_data->left;
      direct = 1;
    }
  }

  // Add the free slot to the free list.
  my_add_to_free_list(metadata, current_prev, direct);
}

void my_finalize() {
  // Implement here!
}



void test() {
  // Implement here!
  printf("------\n");
  print_tree(my_heap.free_head);
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
