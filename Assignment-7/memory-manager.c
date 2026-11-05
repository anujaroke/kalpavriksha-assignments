#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POOL_SIZE 1000000

#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3

typedef struct {
  int start;
  int size;
  int allocation_id;
  int is_allocated;
} MemoryBlock;

char *memory_pool;
MemoryBlock *blocks;
int pool_size;
int block_count = 1;
int block_capacity = 4;
int current_strategy = 0;
int next_allocation_id = 1;


int read_number(const char message[], int *value) {

  printf("%s", message);

  return scanf("%d", value) == 1;

}

int insert_block(int index, MemoryBlock block) {

  MemoryBlock *temporary;
  int position;

  if (block_count == block_capacity) {
    block_capacity = block_capacity * 2;
    temporary = realloc(blocks, block_capacity * sizeof(MemoryBlock));

    if (temporary == NULL) {
      printf("Expansion failed.\n");
      return 0;
    }

    blocks = temporary;
  }

  for (position = block_count; position > index; position--) {
    blocks[position] = blocks[position - 1];
  }

  blocks[index] = block;
  block_count++;

  return 1;

}

void remove_block(int index) {

  int position;

  for (position = index; position < block_count - 1; position++) {
    blocks[position] = blocks[position + 1];
  }

  block_count--;

}

void merge_free_blocks() {

  int index = 0;

  while (index < block_count - 1) {

    if (!blocks[index].is_allocated && !blocks[index + 1].is_allocated) {
      blocks[index].size += blocks[index + 1].size;
      remove_block(index + 1);
    }

    else {
      index++;
    }

  }

}

int find_first_fit(int size) {

  int index;

  for (index = 0; index < block_count; index++) {
    if (!blocks[index].is_allocated && blocks[index].size >= size) {
      return index;
    }
  }

  return -1;

}

int find_best_fit(int size) {

  int index;
  int best_index = -1;

  for (index = 0; index < block_count; index++) {
    if (!blocks[index].is_allocated && blocks[index].size >= size &&
        (best_index == -1 || blocks[index].size < blocks[best_index].size)) {
      best_index = index;
    }
  }

  return best_index;

}

int find_worst_fit(int size) {

  int index;
  int worst_index = -1;

  for (index = 0; index < block_count; index++) {
    if (!blocks[index].is_allocated && blocks[index].size >= size &&
        (worst_index == -1 || blocks[index].size > blocks[worst_index].size)) {
      worst_index = index;
    }
  }

  return worst_index;

}

int find_suitable_block(int size) {

  switch (current_strategy) {
    case FIRST_FIT:
      return find_first_fit(size);

    case BEST_FIT:
      return find_best_fit(size);

    default:
      return find_worst_fit(size);
  }

}

int read_allocation_index() {

  int allocation_id;
  int index;

  if (!read_number("Allocation ID: ", &allocation_id) || allocation_id < 1) {
    printf("Invalid allocation ID.\n");
    return -1;
  }

  for (index = 0; index < block_count; index++) {
    if (blocks[index].is_allocated && blocks[index].allocation_id == allocation_id) {
      return index;
    }
  }

  if (allocation_id < next_allocation_id) {
    printf("This block is already free.\n");
  }

  else {
    printf("Unknown allocation ID.\n");
  }

  return -1;

}

void allocate_memory() {

  int requested_size;
  int index;
  int start;
  int size;
  MemoryBlock remaining_block;

  if (!read_number("Requested size: ", &requested_size) || requested_size < 1 || requested_size > pool_size) {
    printf("Invalid size.\n");
    return;
  }

  index = find_suitable_block(requested_size);

  if (index == -1) {
    printf("Allocation failed: no contiguous free block is large enough.\n");
    return;
  }

  start = blocks[index].start;
  size = blocks[index].size;

  if (size > requested_size) {
    remaining_block = (MemoryBlock){start + requested_size, size - requested_size, -1, 0};

    if (!insert_block(index + 1, remaining_block)) {
      return;
    }
  }

  blocks[index] = (MemoryBlock){start, requested_size, next_allocation_id, 1};

  printf("Allocation successful. ID: %d, Start: %d, Size: %d\n", next_allocation_id, start, requested_size);

  next_allocation_id++;

}

void release_memory() {

  int index = read_allocation_index();

  if (index == -1) {
    return;
  }

  blocks[index].is_allocated = 0;
  blocks[index].allocation_id = -1;

  merge_free_blocks();

  printf("Allocation released.\n");

}

void resize_allocation() {

  int old_index = read_allocation_index();
  int new_index;
  int new_size;
  int old_start;
  int old_size;
  int new_start;
  int remaining_size;
  MemoryBlock remaining_block;

  if (old_index == -1) {
    return;
  }

  if (!read_number("New size: ", &new_size) || new_size < 1 || new_size > pool_size) {
    printf("Invalid size.\n");
    return;
  }

  old_start = blocks[old_index].start;
  old_size = blocks[old_index].size;

  if (new_size == old_size) {
    printf("Allocation already has this size.\n");
    return;
  }

  if (new_size < old_size) {
    remaining_block = (MemoryBlock){old_start + new_size, old_size - new_size, -1, 0};

    if (insert_block(old_index + 1, remaining_block)) {
      blocks[old_index].size = new_size;
      merge_free_blocks();
      printf("Allocation shrunk.\n");
    }

    return;
  }

  if (old_index + 1 < block_count && !blocks[old_index + 1].is_allocated &&
      blocks[old_index + 1].size >= new_size - old_size) {
    blocks[old_index + 1].start += new_size - old_size;
    blocks[old_index + 1].size -= new_size - old_size;
    blocks[old_index].size = new_size;

    if (blocks[old_index + 1].size == 0) {
      remove_block(old_index + 1);
    }

    printf("Allocation expanded in place.\n");
    return;
  }

  new_index = find_suitable_block(new_size);

  if (new_index == -1) {
    printf("Resize failed: no contiguous free block is large enough.\n");
    return;
  }

  new_start = blocks[new_index].start;
  remaining_size = blocks[new_index].size - new_size;

  if (remaining_size > 0) {
    remaining_block = (MemoryBlock){new_start + new_size, remaining_size, -1, 0};

    if (!insert_block(new_index + 1, remaining_block)) {
      return;
    }

    if (new_index < old_index) {
      old_index++;
    }
  }

  memmove(memory_pool + new_start, memory_pool + old_start, old_size);

  blocks[new_index] = (MemoryBlock){new_start, new_size, blocks[old_index].allocation_id, 1};
  blocks[old_index].is_allocated = 0;
  blocks[old_index].allocation_id = -1;

  merge_free_blocks();

  printf("Allocation moved and resized.\n");

}

void display_memory_map() {

  int index;

  printf("\nMemory Map (%d bytes):\n", pool_size);

  for (index = 0; index < block_count; index++) {

    if (blocks[index].is_allocated) {
      printf("[Allocated #%d: %d bytes, Start: %d]\n", blocks[index].allocation_id,
             blocks[index].size, blocks[index].start);
    }

    else {
      printf("[Free: %d bytes, Start: %d]\n", blocks[index].size, blocks[index].start);
    }

  }

}

void display_statistics() {

  int index;
  int allocated_bytes = 0;
  int free_bytes = 0;
  int free_count = 0;
  int largest_free = 0;

  for (index = 0; index < block_count; index++) {

    if (blocks[index].is_allocated) {
      allocated_bytes += blocks[index].size;
    }

    else {
      free_bytes += blocks[index].size;
      free_count++;

      if (blocks[index].size > largest_free) {
        largest_free = blocks[index].size;
      }
    }

  }

  printf("\nTotal memory: %d bytes", pool_size);
  printf("\nAllocated: %d bytes", allocated_bytes);
  printf("\nFree: %d bytes", free_bytes);
  printf("\nFree blocks: %d", free_count);
  printf("\nLargest free block: %d bytes\n", largest_free);

}

void change_strategy() {

  int strategy;

  if (!read_number("Select strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ", &strategy) ||
      strategy < FIRST_FIT || strategy > WORST_FIT) {
    printf("Invalid strategy.\n");
    return;
  }

  current_strategy = strategy;

  printf("Strategy changed.\n");

}

void display_menu() {

  printf("\n1. Allocate memory");
  printf("\n2. Release memory");
  printf("\n3. Resize an allocation");
  printf("\n4. Display memory map");
  printf("\n5. Display memory statistics");
  printf("\n6. Change allocation strategy");
  printf("\n7. Exit\n");

}


int main() {

  int choice = 0;

  if (!read_number("Enter memory pool size (1-1000000): ", &pool_size) ||
      pool_size < 1 || pool_size > MAX_POOL_SIZE) {
    printf("Invalid pool size.\n");
    return 1;
  }

  memory_pool = malloc(pool_size);
  blocks = malloc(block_capacity * sizeof(MemoryBlock));

  if (memory_pool == NULL || blocks == NULL) {
    printf("Memory allocation failed.\n");
    free(memory_pool);
    free(blocks);
    return 1;
  }

  blocks[0] = (MemoryBlock){0, pool_size, -1, 0};

  change_strategy();

  if (current_strategy == 0) {
    free(memory_pool);
    free(blocks);
    return 1;
  }

  while (choice != 7) {

    display_menu();

    if (!read_number("Choice: ", &choice)) {
      printf("Invalid input.\n");
      break;
    }

    switch (choice) {
      case 1:
        allocate_memory();
        break;

      case 2:
        release_memory();
        break;

      case 3:
        resize_allocation();
        break;

      case 4:
        display_memory_map();
        break;

      case 5:
        display_statistics();
        break;

      case 6:
        change_strategy();
        break;

      case 7:
        break;

      default:
        printf("Invalid menu choice.\n");
    }

  }

  free(memory_pool);
  free(blocks);

  return 0;

}