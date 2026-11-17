#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_LINE 20000

typedef struct free_block {
  int index;
  struct free_block *next;
  struct free_block *prev;
} free_block;

typedef struct file_node {
  char name[MAX_NAME + 1];
  int is_directory;
  int size;
  int block_count;
  int *block_pointers;
  struct file_node *parent;
  struct file_node *children;
  struct file_node *next;
  struct file_node *prev;
} file_node;

char virtual_disk[NUM_BLOCKS][BLOCK_SIZE];
free_block *free_head = NULL;
free_block *free_tail = NULL;
int free_count = 0;
file_node *root;
file_node *cwd;


void add_free_block(int index) {

  free_block *block = malloc(sizeof(free_block));

  if (block == NULL) {
    printf("Memory allocation failed.\n");
    return;
  }

  block->index = index;
  block->next = NULL;
  block->prev = free_tail;

  if (free_tail == NULL) {
    free_head = block;
  }

  else {
    free_tail->next = block;
  }

  free_tail = block;
  free_count++;

}

int take_free_block() {

  free_block *block = free_head;
  int index = block->index;

  free_head = block->next;

  if (free_head == NULL) {
    free_tail = NULL;
  }

  else {
    free_head->prev = NULL;
  }

  free(block);
  free_count--;

  return index;

}

int bytes_in_block(int size, int block_number) {

  int bytes = size - block_number * BLOCK_SIZE;

  if (bytes > BLOCK_SIZE) {
    bytes = BLOCK_SIZE;
  }

  return bytes;

}

file_node *create_node(const char name[], int is_directory, file_node *parent) {

  file_node *node = calloc(1, sizeof(file_node));

  if (node == NULL) {
    return NULL;
  }

  strcpy(node->name, name);
  node->is_directory = is_directory;
  node->parent = parent;

  return node;

}

void insert_node(file_node *directory, file_node *node) {

  file_node *head = directory->children;

  if (head == NULL) {
    node->next = node;
    node->prev = node;
    directory->children = node;
  }

  else {
    node->next = head;
    node->prev = head->prev;
    head->prev->next = node;
    head->prev = node;
  }

}

void remove_node(file_node *directory, file_node *node) {

  if (node->next == node) {
    directory->children = NULL;
    return;
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;

  if (directory->children == node) {
    directory->children = node->next;
  }

}

file_node *find_child(file_node *directory, const char name[]) {

  file_node *node = directory->children;

  if (node == NULL) {
    return NULL;
  }

  do {

    if (strcmp(node->name, name) == 0) {
      return node;
    }

    node = node->next;

  } while (node != directory->children);

  return NULL;

}

void print_path(file_node *node) {

  if (node->parent == NULL) {
    printf("/");
    return;
  }

  print_path(node->parent);

  if (node->parent->parent != NULL) {
    printf("/");
  }

  printf("%s", node->name);

}

void add_entry(const char name[], int is_directory) {

  file_node *node;

  if (name[0] == '\0' || strlen(name) > MAX_NAME || strcmp(name, "..") == 0) {
    printf("Invalid name. Use 1 to 50 characters.\n");
    return;
  }

  if (find_child(cwd, name) != NULL) {
    printf("'%s' already exists.\n", name);
    return;
  }

  node = create_node(name, is_directory, cwd);

  if (node == NULL) {
    printf("Memory allocation failed.\n");
    return;
  }

  insert_node(cwd, node);

  if (is_directory) {
    printf("Directory '%s' created successfully.\n", name);
  }

  else {
    printf("File '%s' created successfully.\n", name);
  }

}

void change_directory(const char name[]) {

  file_node *node;

  if (strcmp(name, "..") == 0) {
    if (cwd->parent != NULL) {
      cwd = cwd->parent;
    }
  }

  else {
    node = find_child(cwd, name);

    if (node == NULL || !node->is_directory) {
      printf("Directory not found.\n");
      return;
    }

    cwd = node;
  }

  printf("Moved to ");
  print_path(cwd);
  printf("\n");

}

void list_entries() {

  file_node *node = cwd->children;

  if (node == NULL) {
    printf("(empty)\n");
    return;
  }

  do {

    printf("%s", node->name);

    if (node->is_directory) {
      printf("/");
    }

    printf("\n");
    node = node->next;

  } while (node != cwd->children);

}

void write_file(const char name[], const char text[]) {

  file_node *file = find_child(cwd, name);
  int size = strlen(text);
  int needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
  int *pointers = NULL;
  int index;

  if (file == NULL || file->is_directory) {
    printf("File not found.\n");
    return;
  }

  if (needed > free_count + file->block_count) {
    printf("Write failed: not enough free blocks on the disk.\n");
    return;
  }

  if (needed > 0) {
    pointers = malloc(needed * sizeof(int));

    if (pointers == NULL) {
      printf("Memory allocation failed.\n");
      return;
    }
  }

  for (index = 0; index < file->block_count; index++) {
    add_free_block(file->block_pointers[index]);
  }

  free(file->block_pointers);

  for (index = 0; index < needed; index++) {
    pointers[index] = take_free_block();
    memcpy(virtual_disk[pointers[index]], text + index * BLOCK_SIZE, bytes_in_block(size, index));
  }

  file->block_pointers = pointers;
  file->block_count = needed;
  file->size = size;

  printf("Data written successfully (size=%d bytes).\n", size);

}

void read_file(const char name[]) {

  file_node *file = find_child(cwd, name);
  int index;

  if (file == NULL || file->is_directory) {
    printf("File not found.\n");
    return;
  }

  if (file->size == 0) {
    printf("(empty)\n");
    return;
  }

  for (index = 0; index < file->block_count; index++) {
    printf("%.*s", bytes_in_block(file->size, index), virtual_disk[file->block_pointers[index]]);
  }

  printf("\n");

}

void delete_file(const char name[]) {

  file_node *file = find_child(cwd, name);
  int index;

  if (file == NULL || file->is_directory) {
    printf("File not found.\n");
    return;
  }

  remove_node(cwd, file);

  for (index = 0; index < file->block_count; index++) {
    add_free_block(file->block_pointers[index]);
  }

  free(file->block_pointers);
  free(file);

  printf("File deleted successfully.\n");

}

void remove_directory(const char name[]) {

  file_node *directory = find_child(cwd, name);

  if (directory == NULL || !directory->is_directory) {
    printf("Directory not found.\n");
    return;
  }

  if (directory->children != NULL) {
    printf("Directory is not empty.\n");
    return;
  }

  remove_node(cwd, directory);
  free(directory);

  printf("Directory removed successfully.\n");

}

void display_disk_usage() {

  int used_blocks = NUM_BLOCKS - free_count;

  printf("Total Blocks: %d\n", NUM_BLOCKS);
  printf("Used Blocks: %d\n", used_blocks);
  printf("Free Blocks: %d\n", free_count);
  printf("Disk Usage: %.2f%%\n", used_blocks * 100.0 / NUM_BLOCKS);

}

void free_tree(file_node *node) {

  file_node *child = node->children;
  file_node *next_child;

  if (child != NULL) {
    child->prev->next = NULL;
  }

  while (child != NULL) {
    next_child = child->next;
    free_tree(child);
    child = next_child;
  }

  free(node->block_pointers);
  free(node);

}

void release_memory() {

  free_block *block;

  free_tree(root);

  while (free_head != NULL) {
    block = free_head;
    free_head = free_head->next;
    free(block);
  }

  printf("Memory released. Exiting program...\n");

}


int main() {

  char line[MAX_LINE];
  char command[10];
  char name[100];
  char *text;
  int offset;
  int length;
  int index;

  root = create_node("/", 1, NULL);

  if (root == NULL) {
    printf("Memory allocation failed.\n");
    return 1;
  }

  cwd = root;

  for (index = 0; index < NUM_BLOCKS; index++) {
    add_free_block(index);
  }

  printf("Compact VFS - ready. Type 'exit' to quit.\n");

  while (1) {

    print_path(cwd);
    printf(" > ");

    if (fgets(line, sizeof(line), stdin) == NULL) {
      break;
    }

    line[strcspn(line, "\r\n")] = '\0';
    command[0] = '\0';
    name[0] = '\0';
    offset = 0;

    sscanf(line, "%9s %99s%n", command, name, &offset);

    if (command[0] == '\0') {
      continue;
    }

    if (strcmp(command, "exit") == 0) {
      break;
    }

    else if (strcmp(command, "mkdir") == 0) {
      add_entry(name, 1);
    }

    else if (strcmp(command, "create") == 0) {
      add_entry(name, 0);
    }

    else if (strcmp(command, "cd") == 0) {
      change_directory(name);
    }

    else if (strcmp(command, "ls") == 0) {
      list_entries();
    }

    else if (strcmp(command, "write") == 0) {
      text = line + offset;

      while (*text == ' ') {
        text++;
      }

      length = strlen(text);

      if (length >= 2 && (text[0] == '"' || text[0] == '\'') && text[length - 1] == text[0]) {
        text[length - 1] = '\0';
        text++;
      }

      write_file(name, text);
    }

    else if (strcmp(command, "read") == 0) {
      read_file(name);
    }

    else if (strcmp(command, "delete") == 0) {
      delete_file(name);
    }

    else if (strcmp(command, "rmdir") == 0) {
      remove_directory(name);
    }

    else if (strcmp(command, "pwd") == 0) {
      print_path(cwd);
      printf("\n");
    }

    else if (strcmp(command, "df") == 0) {
      display_disk_usage();
    }

    else {
      printf("Invalid command.\n");
    }

  }

  release_memory();

  return 0;

}