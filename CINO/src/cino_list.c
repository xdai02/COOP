#include "cino_list.h"

/****************************************
 *               list_t
 ****************************************/

typedef struct node_t {
    T data;
    struct node_t *prev;
    struct node_t *next;
} node_t;

typedef struct list_t {
    node_t *head;
    node_t *tail;
    data_type_t data_type;
    size_t size;
    compare_t compare;
    destroy_t destroy;
} list_t;

/**
 * @brief   Create a list node by given data.
 * @param data  data
 * @return  Returns the created node.
 */
static node_t *list_node_create(T data) {
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    return_value_if_fail(node != NULL, NULL);
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/**
 * @brief   Create cino-list.
 * @param data_type data type
 *                  valid data type includes:
 *                      - DATA_TYPE_INT
 *                      - DATA_TYPE_DOUBLE
 *                      - DATA_TYPE_CHAR
 *                      - DATA_TYPE_T (generic)
 * @param compare   User-defined callback function for comparison.
 *                  Set to:
 *                      - `compare_int` if the data type is DATA_TYPE_INT
 *                      - `compare_double` if the data type is DATA_TYPE_DOUBLE
 *                      - `compare_char` if the data type is DATA_TYPE_CHAR
 *                      - `compare_t` interface if the data type is DATA_TYPE_T, 
 *                         otherwise a default `compare_default` is applied.
 * @param destroy   User-defined callback function for destroying.
 *                  Set to:
 *                      - `destroy_int` if the data type is DATA_TYPE_INT
 *                      - `destroy_double` if the data type is DATA_TYPE_DOUBLE
 *                      - `destroy_char` if the data type is DATA_TYPE_CHAR
 *                      - `destroy_t` interface if the data type is DATA_TYPE_T, 
 *                         otherwise a default `destroy_default` is applied.
 * @return  Returns the pointer to cino-list, or `NULL` if creation failed.
 */
list_t *list_create(data_type_t data_type, compare_t compare, destroy_t destroy) {
    return_value_if_fail(is_valid_cino_data_type(data_type), NULL);

    list_t *list = (list_t *)calloc(1, sizeof(list_t));
    return_value_if_fail(list != NULL, NULL);

    list->size = 0;

    list->data_type = data_type;
    list->compare = compare ? compare : compare_default;
    list->destroy = destroy ? destroy : destroy_default;

    list->head = list_node_create(NULL);
    call_and_return_value_if_fail(list->head != NULL, list_destroy(list), NULL);
    list->tail = list_node_create(NULL);
    call_and_return_value_if_fail(list->tail != NULL, list_destroy(list), NULL);

    list->head->next = list->tail;
    list->tail->prev = list->head;

    return list;
}

/**
 * @brief   Destroy cino-list.
 * @param list  cino-list
 */
void list_destroy(list_t *list) {
    return_if_fail(list != NULL);

    list_clear(list);

    list->destroy = NULL;
    if (list) {
        free(list);
        list = NULL;
    }
}

/**
 * @brief   Determine if the cino-list is empty.
 * @param list  cino-list
 * @return  Returns `true` if the cino-list is empty, otherwise returns `false`.
 */
bool list_is_empty(const list_t *list) {
    return !list || list->size == 0;
}

/**
 * @brief   Get the number of elements in the cino-list.
 * @param list  cino-list
 * @return  Returns the number of elements in the cino-list.
 */
size_t list_size(const list_t *list) {
    return_value_if_fail(list != NULL, 0);
    return list->size;
}

/**
 * @brief   Clear all the elments in the cino-list.
 * @param list  cino-list
 * @return  Returns the modified cino-list.
 */
list_t *list_clear(list_t *list) {
    return_value_if_fail(list != NULL, NULL);

    while (!list_is_empty(list)) {
        T data = list_pop_front(list);
        list->destroy(data);
    }

    list->size = 0;

    if (list->head) {
        free(list->head);
        list->head = NULL;
    }

    if (list->tail) {
        free(list->tail);
        list->tail = NULL;
    }

    return list;
}

/**
 * @brief   Traverse cino-list.
 * @param list      cino-list
 * @param visit     user-defined callback function for visiting a single element
 * @param reverse   true = backward, false = forward
 */
void list_foreach(list_t *list, visit_t visit, bool backward) {
    return_if_fail(list != NULL && visit != NULL);

    if (!backward) {
        node_t *cur = list->head;
        while (cur && cur->next && cur->next != list->tail) {
            cur = cur->next;
            visit(cur->data);
        }
    } else {
        node_t *cur = list->tail;
        while (cur && cur->prev && cur->prev != list->head) {
            cur = cur->prev;
            visit(cur->data);
        }
    }
}

/**
 * @brief   Get the node in the cino-list by index.
 * @param list  cino-list
 * @param index index
 * @return  Returns a pointer points to the indexed node.
 */
static node_t *list_get_node(const list_t *list, int index) {
    return_value_if_fail(list != NULL && index >= 0 && index < list->size, NULL);

    node_t *node = NULL;

    if (index <= list->size / 2) {
        node = list->head->next;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
    } else {
        node = list->tail->prev;
        for (int i = 0; i < list->size - index - 1; i++) {
            node = node->prev;
        }
    }

    return node;
}

/**
 * @brief   Get the first element in the cino-list.
 * @param list  cino-list
 * @return  For primitive cino-list, this function returns a wrapper type of the first
 *          primitive. Caller should use `->data` to get the primitive value, instead
 *          of unwrapping it.
 */
T list_get_front(const list_t *list) {
    return_value_if_fail(list != NULL && list->size > 0, NULL);
    return list->head->next->data;
}

/**
 * @brief   Get the last element in the cino-list.
 * @param list  cino-list
 * @return  For primitive cino-list, this function returns a wrapper type of the last
 *          primitive. Caller should use `->data` to get the primitive value, instead
 *          of unwrapping it.
 */
T list_get_back(const list_t *list) {
    return_value_if_fail(list != NULL && list->size > 0, NULL);
    return list->tail->prev->data;
}

/**
 * @brief   Get the element of the indexed component in the cino-list.
 * @param list  cino-list
 * @param index index
 * @return  For primitive cino-list, this function returns a wrapper type of the
 *          primitive. Caller should use `->data` to get the primitive value, instead
 *          of unwrapping it.
 */
T list_get(const list_t *list, int index) {
    return_value_if_fail(list != NULL && index >= 0 && index < list->size, NULL);
    node_t *node = list_get_node(list, index);
    return node->data;
}

/**
 * @brief   Update the element of the indexed component in the cino-list.
 * @param list  cino-list
 * @param index index
 * @param data  For primitive data, a wrapper type of that primitive is needed.
 */
void list_set(list_t *list, int index, T data) {
    return_if_fail(list != NULL && index >= 0 && index < list->size);
    node_t *node = list_get_node(list, index);
    list->destroy(node->data);
    node->data = data;
}

/**
 * @brief   Find the index of the first occurrence of the specified element in the
 *          cino-list.
 * @param list      cino-list
 * @param context   - For primitive cino-list, a wrapper type of the searching data should
 *                    be passed. This function will unwrap for you.
 *                  - For T (generic) cino-list, a match_t callback function should be passed
 *                    as the matching rule.
 * @return  Returns the index of the first occurrence of the specified element in the
 *          cino-list, or `-1` if not found.
 */
int list_index_of(const list_t *list, void *context) {
    return_value_if_fail(list != NULL && context != NULL, -1);

    int index = -1;
    int i = 0;

    if (list->data_type == DATA_TYPE_T) {
        match_t match = (match_t)context;
        node_t *cur = list->head;
        while (cur && cur->next && cur->next != list->tail) {
            cur = cur->next;
            if (match(cur->data)) {
                index = i;
                break;
            }
            i++;
        }
    } else {
        node_t *cur = list->head;
        while (cur && cur->next && cur->next != list->tail) {
            cur = cur->next;
            if (list->compare(cur->data, context) == 0) {
                index = i;
                break;
            }
            i++;
        }
        list->destroy(context);
    }

    return index;
}

/**
 * @brief   Inserts the specified element at the beginning of the cino-list.
 * @param list  cino-list
 * @param data  For primitive data, a wrapper type of that primitive is needed.
 * @return  Returns the modified cino-list.
 */
list_t *list_push_front(list_t *list, T data) {
    return_value_if_fail(list != NULL && data != NULL, list);
    return list_insert(list, 0, data);
}

/**
 * @brief   Inserts the specified element at the end of the cino-list.
 * @param list  cino-list
 * @param data  For primitive data, a wrapper type of that primitive is needed
 * @return  Returns the modified cino-list.
 */
list_t *list_push_back(list_t *list, T data) {
    return_value_if_fail(list != NULL && data != NULL, list);
    return list_insert(list, list->size, data);
}

/**
 * @brief   Removes the first element from the cino-list.
 * @param list  cino-list
 * @return  For primitive cino-list, this function returns a wrapper type of the removed
 *          primitive. It is caller's responsibility to unwrap or free.
 */
T list_pop_front(list_t *list) {
    return_value_if_fail(list != NULL && list->size > 0, NULL);
    return list_remove(list, 0);
}

/**
 * @brief   Removes the last element from the cino-list.
 * @param list  cino-list
 * @return  For primitive cino-list, this function returns a wrapper type of the removed
 *          primitive. It is caller's responsibility to unwrap or free.
 */
T list_pop_back(list_t *list) {
    return_value_if_fail(list != NULL && list->size > 0, NULL);
    return list_remove(list, list->size - 1);
}

/**
 * @brief   Inserts the specified element at the indexed location of the cino-list.
 * @param list  cino-list
 * @param index index
 * @param data  For primitive data, a wrapper type of that primitive is needed.
 * @return  Returns the modified cino-list.
 */
list_t *list_insert(list_t *list, int index, T data) {
    return_value_if_fail(list != NULL && index >= 0 && index <= list->size && data != NULL, list);

    node_t *new_node = list_node_create(data);
    return_value_if_fail(new_node != NULL, list);

    if (index == list->size) {
        new_node->next = list->tail;
        new_node->prev = list->tail->prev;
        list->tail->prev = new_node;
        new_node->prev->next = new_node;
    } else {
        node_t *cur_node = list_get_node(list, index);
        new_node->prev = cur_node->prev;
        new_node->next = cur_node;
        new_node->prev->next = new_node;
        new_node->next->prev = new_node;
    }

    list->size++;
    return list;
}

/**
 * @brief   Removes the indexed element from the cino-list.
 * @param list  cino-list
 * @param index index
 * @return  For primitive cino-list, this function returns a wrapper type of the removed
 *          primitive. It is caller's responsibility to unwrap or free.
 */
T list_remove(list_t *list, int index) {
    return_value_if_fail(list != NULL && index >= 0 && index < list->size && list->size > 0, NULL);

    node_t *node = list_get_node(list, index);
    T removed = node->data;

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;

    free(node);
    node = NULL;

    list->size--;
    return removed;
}