#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

linked_list *linked_list_empty(void)
{
  linked_list *p = malloc(sizeof(*p));
  if (p == NULL) {
    return NULL;
  }
  p -> head = NULL;
  p -> tail = NULL;
  p -> size = 0;
  return p;
}

bool linked_list_is_empty(const linked_list *s) {
  return (s -> size == 0);
}

size_t linked_list_length(const linked_list *s) {
  return s -> size;
}

int linked_list_head_value(const linked_list *s, void *ptr) {
  if (s -> head == NULL) {
    return -1;
  }

  ptr = (s -> head) -> value;
  return 0;
}

int linked_list_search(const linked_list *s, char *id) {
  if (id == NULL || s == NULL) {
    return -1;
  }
  if (s -> size == 0) {
    return -1;
  }
  ll_node *p = s -> head;
  int result = -1;
  while (p != NULL && (result = strcmp(id, p -> id)) != 0) {
    p = p -> next;
  }
  return (result == 0 ? p -> bytecode_index : -1);
}

int linked_list_type_of_id(const linked_list *s, char *id) {
  if (id == NULL || s == NULL) {
    return -1;
  }
  if (s -> size == 0) {
    return -1;
  }
  ll_node *p = s -> head;
  int result = -1;
  while (p != NULL && (result = strcmp(id, p -> id)) != 0) {
    p = p -> next;
  }
  return (result == 0 ? p -> type : -1);
}

int linked_list_insert(linked_list *s, unsigned int index, char *id, int type) {
  ll_node *p = malloc(sizeof(ll_node));
  if (p == NULL) {
    return -1;
  }
  if (linked_list_search(s, id) != -1) {
    return -1;
  }
  p -> bytecode_index = index;
  p -> type = type;
  p -> id = id;
  p -> next = NULL;
  s -> size += 1;
  if (s -> head == NULL) {
    s -> head = p;
    s -> tail = p;
    return 0;
  }
  else {
    (s -> tail) -> next = p;
    s -> tail = p;
  }
  return 0;
}

void *linked_list_remove(linked_list *s, void *ptr) {
  if (ptr == NULL) {
    return NULL;
  }
  if (s -> size == 0) {
    return NULL;
  }
  ll_node *p = s -> head;
  if (p->value == ptr) {
    s->size -= 1;
    s->head = p->next;
    if (s->tail == p) {
      s->tail = NULL;
    }
    free(p);
    return ptr;
  }
  size_t i = 1;
  while (i < s->size && p->value != ptr) {
    if ((p->next)->value == ptr) {
      s->size -= 1;
      if (i == ((s->size) - 1)) {
        free(p->next);
        p->next = NULL;
        return ptr;
      }
      ll_node *temp = p->next;
      p->next = temp->next;
      free(temp);
      return ptr;
    }
    p = p->next;
    i += 1;
  }
  return ptr;
}

void linked_list_dispose(linked_list ** sptr)
{
  if (*sptr == NULL) {
    return;
  }
  ll_node *p = (*sptr) -> head;
  while (p != NULL) {
    ll_node *t = p;
    p = p -> next;
    free(t);
  }
  free(*sptr);
  *sptr = NULL;
}
