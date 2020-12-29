//
// Created by sheep on 2020/12/29.
//

// What I cannot create, I do not understand.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
    NODE_INTERNAL,
    NODE_LEAF
} NodeType;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
    u_int32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

const u_int32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
// const u_int32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
// const u_int32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
// sizeof(((Row*)0)->id) 将NULL转换成结构体并求大小 弔
const u_int32_t ID_SIZE = size_of_attribute(Row, id);
const u_int32_t USERNAME_SIZE = size_of_attribute(Row, username);
const u_int32_t EMAIL_SIZE = size_of_attribute(Row, email);
const u_int32_t ID_OFFSET = 0;
const u_int32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const u_int32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const u_int32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

// Common Node Header Layout
const u_int32_t NODE_TYPE_SIZE = sizeof(u_int8_t);
const u_int32_t NODE_TYPE_OFFSET = 0;
const u_int32_t IS_ROOT_SIZE = sizeof(u_int8_t);
const u_int32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const u_int32_t PARENT_POINTER_SIZE = sizeof(u_int32_t);
const u_int32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const u_int8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

// Leaf Node Header Layout
const u_int32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(u_int32_t);
const u_int32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
// const u_int32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;
const u_int32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(u_int32_t);
const u_int32_t LEAF_NODE_NEXT_LEAF_OFFSET =
        LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const u_int32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE +
                                        LEAF_NODE_NUM_CELLS_SIZE +
                                        LEAF_NODE_NEXT_LEAF_SIZE;

// Leaf Node Body Layout
const u_int32_t LEAF_NODE_KEY_SIZE = sizeof(u_int32_t);
const u_int32_t LEAF_NODE_KEY_OFFSET = 0;
const u_int32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const u_int32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const u_int32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const u_int32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const u_int32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

const u_int32_t LEAF_NODE_RIGHT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) / 2;
const u_int32_t LEAF_NODE_LEFT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) - LEAF_NODE_RIGHT_SPLIT_COUNT;

// Internal Node Header Layout
const u_int32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(u_int32_t);
const u_int32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const u_int32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(u_int32_t);
const u_int32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET =
        INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE;
const u_int32_t INTERNAL_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE +
                                            INTERNAL_NODE_NUM_KEYS_SIZE +
                                            INTERNAL_NODE_RIGHT_CHILD_SIZE;

// Internal Node Body Layout
const u_int32_t INTERNAL_NODE_KEY_SIZE = sizeof(u_int32_t);
const u_int32_t INTERNAL_NODE_CHILD_SIZE = sizeof(u_int32_t);
const u_int32_t INTERNAL_NODE_CELL_SIZE = INTERNAL_NODE_KEY_SIZE + INTERNAL_NODE_CHILD_SIZE;
const u_int32_t INTERNAL_NODE_MAX_CELLS = 500;

u_int32_t *leaf_node_next_leaf(void *node) {
    return (u_int32_t*) (node + LEAF_NODE_NEXT_LEAF_OFFSET);
}

void set_node_type(void *node, NodeType type) {
    u_int8_t value = type;
    *((u_int8_t*)(node + NODE_TYPE_OFFSET)) = value;
}

NodeType get_node_type(void *node) {
    u_int8_t value = *((u_int8_t*)(node + NODE_TYPE_OFFSET));
    return (NodeType)value;
}

u_int32_t *leaf_node_num_cells(void *node) {
    return (u_int32_t*) (node + LEAF_NODE_NUM_CELLS_OFFSET);
}

void *leaf_node_cell(void* node, u_int32_t cell_num) {
    return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

u_int32_t *leaf_node_key(void *node, u_int32_t cell_num) {
    return (u_int32_t*) (leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_OFFSET);
}

void *leaf_node_value(void *node, u_int32_t cell_num) {
    return leaf_node_cell(node, cell_num) + LEAF_NODE_VALUE_OFFSET;
}

u_int32_t *internal_node_num_keys(void *node) {
    return (u_int32_t*)(node + INTERNAL_NODE_NUM_KEYS_OFFSET);
}

u_int32_t *internal_node_right_child(void *node) {
    return (u_int32_t*)(node + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}

u_int32_t *internal_node_cell(void *node, u_int32_t cell_num) {
    return (u_int32_t*)(node + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE);
}

u_int32_t *internal_node_key(void *node, u_int32_t key_num) {
    return (u_int32_t*)((void *)internal_node_cell(node, key_num) + INTERNAL_NODE_CHILD_SIZE);
}

u_int32_t *node_parent(void *node) {
    return (u_int32_t*) (node + PARENT_POINTER_OFFSET);
}

u_int32_t *internal_node_child(void *node, u_int32_t child_num) {
    u_int32_t num_keys = *internal_node_num_keys(node);
    if (child_num > num_keys) {
        printf("Tried to access child_num %d > num_keys %d\n", child_num, num_keys);
        exit(EXIT_FAILURE);
    } else if (child_num == num_keys) {
        return internal_node_right_child(node);
    } else {
        return internal_node_cell(node, child_num);
    }
}

u_int32_t get_node_max_key(void *node) {
    switch (get_node_type(node)) {
        case NODE_INTERNAL:
            // shouldn`t minus 1? got to figure it out later
            return *internal_node_key(node, *internal_node_num_keys(node) - 1);

        case NODE_LEAF:
            return *leaf_node_key(node, *leaf_node_num_cells(node) - 1);
    }
}

bool is_node_root(void *node) {
    u_int8_t value = *((u_int8_t*)(node + IS_ROOT_OFFSET));
    return (bool) value;
}

void set_node_root(void *node, bool is_root) {
    u_int8_t value = is_root;
    *((u_int8_t*)(node + IS_ROOT_OFFSET)) = value;
}

void print_constants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELL: %d\n", LEAF_NODE_MAX_CELLS);
}

void initialize_leaf_node(void *node) {
    set_node_type(node, NODE_LEAF);
    set_node_root(node, false);
    *leaf_node_num_cells(node) = 0;
    *leaf_node_next_leaf(node) = 0;
}

void initialize_internal_node(void *node) {
    set_node_type(node, NODE_INTERNAL);
    set_node_root(node, false);
    *internal_node_num_keys(node) = 0;
}

void print_leaf_node(void *node) {
    u_int32_t num_cells = *leaf_node_num_cells(node);
    printf("leaf (size %d)\n", num_cells);
    for (u_int32_t i = 0; i < num_cells; i++) {
        u_int32_t key = *leaf_node_key(node, i);
        printf("  - %d : %d\n", i, key);
    }
}

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef struct {
    int file_descriptor;
    u_int32_t file_length;
    u_int32_t num_pages;
    void *pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    u_int32_t root_page_num;
    Pager *pager;
} Table;

typedef struct {
    Table *table;
    u_int32_t page_num;
    u_int32_t cell_num;
    bool end_of_table;
} Cursor;

// 序列化
void serialize_row(Row *source, void *destination) {
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    strncpy((char *)destination + USERNAME_OFFSET, source->username, USERNAME_SIZE);
    strncpy((char *)destination + EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

//反序列化
void deserialize_row(void *source, Row *destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void print_row(Row *row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void *get_page(Pager *pager, u_int32_t page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out if bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        //Cache miss. Allocate memory and load from file.
        void* page = malloc(PAGE_SIZE);
        u_int32_t num_pages = pager->file_length / PAGE_SIZE;

        //might have saved partial page at the end of the file
        if (pager->file_length % PAGE_SIZE) {
            num_pages++;
        }

        if (page_num <= num_pages) {
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            // 参数offset即为读写位置
            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1) {
                printf("Error reading file: %d\n", errno); // errno for lst error code
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;

        if (page_num >= pager->num_pages) {
            pager->num_pages = page_num + 1;
        }
    }

    return pager->pages[page_num];
}

void indent(u_int32_t level) {
    for (u_int32_t i = 0; i < level; i++) {
        printf("  ");
    }
}

void print_tree(Pager *pager, u_int32_t page_num, u_int32_t indentation_level) {
    void *node = get_page(pager, page_num);
    u_int32_t num_keys, child;
    switch (get_node_type(node)) {
        case NODE_LEAF:
            num_keys = *leaf_node_num_cells(node);
            indent(indentation_level);
            printf("- leaf(size %d)\n", num_keys);
            for (u_int32_t i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                printf("- %d\n", *leaf_node_key(node, i));
            }
            break;

        case NODE_INTERNAL:
            num_keys = *internal_node_num_keys(node);
            indent(indentation_level);
            printf("- internal(size %d)\n", num_keys);
            for (u_int32_t i = 0; i < num_keys; i++) {
                child = *internal_node_child(node, i);
                print_tree(pager, child, indentation_level + 1);

                indent(indentation_level + 1);
                printf("- key %d\n", *internal_node_key(node, i));
            }
            child = *internal_node_right_child(node);
            print_tree(pager, child, indentation_level + 1);
            break;
    }
}

// Until we start to recycling free pages, new pages will always
// go onto the end of the database file
u_int32_t get_unused_page_num(Pager *pager) {
    return pager->num_pages;
}

// Cursor *table_start(Table *table) {
//     Cursor *cursor = (Cursor*) malloc(sizeof(Cursor));
//     cursor->table = table;
//     cursor->page_num = table->root_page_num;        //在根节点对应的page的第一个cell就是第一个元素
//     cursor->cell_num = 0;

//     void *root_node = get_page(table->pager, table->root_page_num);
//     u_int32_t num_cells = *leaf_node_num_cells(root_node);
//     cursor->end_of_table = (num_cells == 0);

//     return cursor;
// }

// Cursor *table_end(Table *table) {
//     Cursor *cursor = (Cursor*) malloc(sizeof(Cursor));
//     cursor->table = table;
//     cursor->page_num = table->root_page_num;

//     void *root_node = get_page(table->pager, table->root_page_num);
//     u_int32_t num_cells = *leaf_node_num_cells(root_node);
//     cursor->cell_num = num_cells;
//     cursor->end_of_table = true;

//     return cursor;
// }

Cursor *leaf_node_find(Table *table, u_int32_t page_num, u_int32_t key) {
    void *node = get_page(table->pager, page_num);
    u_int32_t num_cells = *leaf_node_num_cells(node);

    Cursor *cursor = (Cursor*) malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->page_num = page_num;

    //Binary search
    u_int32_t min_index = 0;
    u_int32_t one_past_max_index = num_cells;
    while (one_past_max_index != min_index) {
        u_int32_t index = (min_index + one_past_max_index) / 2;
        u_int32_t key_at_index = *leaf_node_key(node, index);
        if (key == key_at_index) {
            cursor->cell_num = index;
            return cursor;
        }
        if (key < key_at_index) {
            one_past_max_index = index;
        } else {
            min_index = index + 1;
        }
    }

    cursor->cell_num = min_index;
    return cursor;
}

// Cursor *internal_node_find(Table *table, u_int32_t page_num, u_int32_t key) {
//     void *node = get_page(table->pager, page_num);
//     u_int32_t num_keys = *internal_node_num_keys(node);

//     // Binary search to find index of child to search
//     u_int32_t min_index = 0;
//     u_int32_t max_index = num_keys;
//     // one more child than key;

//     while (min_index != max_index) {
//         u_int32_t index = (min_index + max_index) / 2;
//         u_int32_t key_to_right = *internal_node_key(node, index);
//         if (key_to_right >= key) {
//             max_index = index;
//         } else {
//             min_index = index + 1;
//         }
//     }

//     u_int32_t child_num = *internal_node_child(node, min_index);
//     void *child = get_page(table->pager, child_num);
//     switch(get_node_type(child)) {
//         case NODE_LEAF:
//             return leaf_node_find(table, child_num, key);

//         case NODE_INTERNAL:
//             return internal_node_find(table, child_num, key);

//     }
// }

u_int32_t internal_node_find_child(void *node, u_int32_t key) {
    // Return the index of the child which should contain the given key

    u_int32_t num_keys = *internal_node_num_keys(node);

    // Binary search to find index of child to search
    u_int32_t min_index = 0;
    u_int32_t max_index = num_keys;
    // one more child than key;

    while (min_index != max_index) {
        u_int32_t index = (min_index + max_index) / 2;
        u_int32_t key_to_right = *internal_node_key(node, index);
        if (key_to_right >= key) {
            max_index = index;
        } else {
            min_index = index + 1;
        }
    }

    return min_index;
}

Cursor *internal_node_find(Table *table, u_int32_t page_num, u_int32_t key) {
    void *node = get_page(table->pager, page_num);

    u_int32_t child_index = internal_node_find_child(node, key);
    u_int32_t child_num = *internal_node_child(node, child_index);

    void *child = get_page(table->pager, child_num);
    switch(get_node_type(child)) {
        case NODE_LEAF:
            return leaf_node_find(table, child_num, key);

        case NODE_INTERNAL:
            return internal_node_find(table, child_num, key);

    }
}

void internal_node_insert(Table *table, u_int32_t parent_page_num, u_int32_t child_page_num) {
    // Add a new child/key pair to parent that corresponds to child

    void *parent = get_page(table->pager, parent_page_num);
    void *child = get_page(table->pager, child_page_num);
    u_int32_t child_max_key = get_node_max_key(child);
    u_int32_t index = internal_node_find_child(parent, child_max_key);

    u_int32_t original_num_keys = *internal_node_num_keys(parent);
    *internal_node_num_keys(parent) = original_num_keys + 1;

    if (original_num_keys >= INTERNAL_NODE_MAX_CELLS) {
        printf("split internal node\n");
        exit(EXIT_FAILURE);
    }

    u_int32_t right_child_page_num = *internal_node_right_child(parent);
    void *right_child = get_page(table->pager, right_child_page_num);

    if (child_max_key > get_node_max_key(right_child)) {
        // Replace right child
        *internal_node_child(parent, original_num_keys) = right_child_page_num;
        *internal_node_key(parent, original_num_keys) = get_node_max_key(right_child);
        *internal_node_right_child(parent) = child_page_num;
    } else {
        // Make room for new cell
        for (u_int32_t i = original_num_keys; i > index; i--) {
            void *destination = internal_node_cell(parent, i);
            void *source = internal_node_cell(parent, i - 1);
            memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);
        }
        *internal_node_child(parent, index) = child_page_num;
        *internal_node_key(parent, index) = child_max_key;
    }
}

// Return the position of the given key.
// if the key is not present, return the position
// where it should be inserted
Cursor *table_find(Table *table, u_int32_t key) {
    u_int32_t root_page_num = table->root_page_num;
    void *root_node = get_page(table->pager, root_page_num);

    if (get_node_type(root_node) == NODE_LEAF) {
        return leaf_node_find(table, root_page_num, key);
    } else {
        return internal_node_find(table, root_page_num, key);
    }
}

Cursor *table_start(Table *table) {
    Cursor *cursor = table_find(table, 0);
    void *node = get_page(table->pager, cursor->page_num);
    u_int32_t num_cells = *leaf_node_num_cells(node);
    cursor->end_of_table = (num_cells == 0);

    return cursor;
}

void create_new_root(Table *table, u_int32_t right_child_page_num) {
    // Handle splitting the root.
    // Old root copied to new page, becomes left child.
    // Address of right child passed in.
    // Re-initialize root page to contain the new root node.
    // New root node points to two children.

    void *root = get_page(table->pager, table->root_page_num);
    void *right_child = get_page(table->pager, right_child_page_num);
    u_int32_t left_child_page_num = get_unused_page_num(table->pager);
    void *left_child = get_page(table->pager, left_child_page_num);

    // left child has data copied from old root
    memcpy(left_child, root, PAGE_SIZE);
    set_node_root(left_child, false);

    // root node is a new internal node with one key and two children
    initialize_internal_node(root);
    set_node_root(root, true);
    *internal_node_num_keys(root) = 1;
    *internal_node_child(root, 0) = left_child_page_num;
    u_int32_t left_child_max_key = get_node_max_key(left_child);
    *internal_node_key(root, 0) = left_child_max_key;
    *internal_node_right_child(root) = right_child_page_num;

    *node_parent(left_child) = table->root_page_num;
    *node_parent(right_child) = table->root_page_num;
}


void update_internal_node_key(void *node, u_int32_t old_key, u_int32_t new_key) {
    u_int32_t old_child_index = internal_node_find_child(node, old_key);
    *internal_node_key(node, old_child_index) = new_key;
}

void leaf_node_split_and_insert(Cursor *cursor, u_int32_t key, Row *value) {
    // Create a new node and move half the cells over
    // Insert the new value in one of the two nodes.
    // Update parent of create a new parent.

    void *old_node = get_page(cursor->table->pager, cursor->page_num);
    u_int32_t old_max = get_node_max_key(old_node);
    u_int32_t new_page_num = get_unused_page_num(cursor->table->pager);
    void *new_node = get_page(cursor->table->pager, new_page_num);
    initialize_leaf_node(new_node);
    *node_parent(new_node) = *node_parent(old_node);
    *leaf_node_next_leaf(new_node) = *leaf_node_next_leaf(old_node);
    *leaf_node_next_leaf(old_node) = new_page_num;

    // All existing keys plus new key should be divided
    // evenly between old(left) and new(right) nodes.
    // Starting from the right, move each key to correct position.
    for (int i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
        void *destination_node;
        if (i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            destination_node = new_node;
        } else {
            destination_node = old_node;
        }

        u_int32_t index_within_node = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void *destination = leaf_node_cell(destination_node, index_within_node);

        if (i == cursor->cell_num) {
            serialize_row(value, leaf_node_value(destination_node, index_within_node));
            *leaf_node_key(destination_node, index_within_node) = key;
        } else if (i > cursor->cell_num) {
            memcpy(destination, leaf_node_cell(old_node, i - 1), LEAF_NODE_CELL_SIZE);
        } else {
            memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    if (is_node_root(old_node)) {
        return create_new_root(cursor->table, new_page_num);
    } else {
        u_int32_t parent_page_num = *node_parent(old_node);
        u_int32_t new_max = get_node_max_key(old_node);
        void *parent = get_page(cursor->table->pager, parent_page_num);

        update_internal_node_key(parent, old_max, new_max);
        internal_node_insert(cursor->table, parent_page_num, new_page_num);
        return;
    }
}

void leaf_node_insert(Cursor *cursor, u_int32_t key, Row *value) {
    void *node = get_page(cursor->table->pager, cursor->page_num);

    u_int32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        //Node full
        leaf_node_split_and_insert(cursor, key, value);
        return;
    }

    if (cursor->cell_num < num_cells) {
        //Make room for new cell
        for (u_int32_t i = num_cells; i > cursor->cell_num; i--) {
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(node)) += 1;
    // printf("%d\n", *(leaf_node_num_cells(node)));
    *(leaf_node_key(node, cursor->cell_num)) = key;
    serialize_row(value, leaf_node_value(node, cursor->cell_num));
}

void pager_flush(Pager *pager, u_int32_t page_num) {
    if (pager->pages[page_num] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written =
            write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);

    if (bytes_written == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

void* cursor_value(Cursor *cursor) {
    u_int32_t page_num = cursor->page_num;
    void *page = get_page(cursor->table->pager, page_num);
    return leaf_node_value(page, cursor->cell_num);
}

void cursor_advance(Cursor *cursor) {
    u_int32_t page_num = cursor->page_num;
    void *node = get_page(cursor->table->pager, page_num);
    // why node not page? strange name
    cursor->cell_num += 1;
    if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
        u_int32_t next_page_num = *leaf_node_next_leaf(node);
        if (next_page_num == 0) {
            // rightmost
            cursor->end_of_table = true;
        } else {
            cursor->page_num = next_page_num;
            cursor->cell_num = 0;
        }
    }
}

void db_close(Table *table) {
    Pager *pager = table->pager;

    for (int i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }
        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    // no needed after switch to B-tree
    // u_int32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
    // if (num_additional_rows > 0) {
    //     u_int32_t page_num = num_ful_pages;
    //     if (pager->pages[page_num] != NULL) {
    //         pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
    //         free(pager->pages[page_num]);
    //         pager->pages[page_num] = NULL;
    //     }
    // }

    int result = close(pager->file_descriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        void *page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        db_close(table);
        exit(EXIT_SUCCESS);
    } else if (strcmp(input_buffer->buffer, ".constants") == 0) {
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;
    } else if (strcmp(input_buffer->buffer, ".btree") == 0) {
        printf("Tree:\n");
        // print_leaf_node(get_page(table->pager, 0));
        print_tree(table->pager, 0, 0);
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

PrepareResult prepare_insert(InputBuffer *input_buffet, Statement *statement) {
    statement->type = STATEMENT_INSERT;

    char *keyword = strtok(input_buffet->buffer, " ");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        return prepare_insert(input_buffer, statement);
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    // TODO find

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement *statement, Table *table) {
    void *node = get_page(table->pager, table->root_page_num);
    u_int32_t num_cells = *leaf_node_num_cells(node);
    // if (num_cells >= LEAF_NODE_MAX_CELLS) {
    //     return EXECUTE_TABLE_FULL;
    // }

    Row* row_to_insert = &(statement->row_to_insert);

    u_int32_t key_to_insert = row_to_insert->id;
    Cursor* cursor = table_find(table, key_to_insert);

    if (cursor->cell_num < num_cells) {
        u_int32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
        if (key_at_index == key_to_insert) {
            return EXECUTE_DUPLICATE_KEY;
        }
    }

    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);

    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) {
    Row row;
    Cursor *cursor = table_start(table);

    while (!cursor->end_of_table) {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }

    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);

        case STATEMENT_SELECT:
            return execute_select(statement, table);
    }
}

Pager *pager_open(const char *filename) {
    int fd = open(filename,
                  O_RDWR |        // Read/Write Mode
                  O_CREAT,    // Create file if it does not exist
                  S_IWUSR |       // User write permission
                  S_IRUSR     // User read permission
    );

    if (fd == -1) {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t file_length = lseek(fd, 0, SEEK_END);

    Pager *pager = (Pager *) malloc(sizeof(Pager));
    pager->file_descriptor = fd;
    pager->file_length = file_length;
    pager->num_pages = (file_length / PAGE_SIZE);

    if (file_length % PAGE_SIZE != 0) {
        printf("Db file is not a whole number of pages. Corrupt file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

Table* db_open(const char* filename) {
    Pager *pager = pager_open(filename);

    Table *table = (Table *) malloc(sizeof(Table));

    table->pager = pager;
    table->root_page_num = 0;

    if (pager->num_pages == 0) {
        // New database file. Initialize page 0 as leaf node.
        void *root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
        set_node_root(root_node, true);
    }

    return table;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read =
            getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    Table *table = db_open(filename);

    InputBuffer* input_buffer = new_input_buffer();

    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case META_COMMAND_SUCCESS:
                    continue;
                    break;

                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'.\n", input_buffer->buffer);
                    continue;
                    break;

                default:
                    break;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;

            case PREPARE_SYNTAX_ERROR:
                printf("Synyax error. Could not parse statement.\n");
                continue;
                break;

            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
                break;

            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
                break;

            case PREPARE_NEGATIVE_ID:
                printf("ID must be positive.\n");
                continue;
                break;

            default:
                break;
        }

        switch (execute_statement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;

            case EXECUTE_DUPLICATE_KEY:
                printf("Error: Duplicate key.\n");
                break;

            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;

            default:
                break;
        }
    }
    return 0;
}