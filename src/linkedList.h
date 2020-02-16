#include <stdbool.h>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct ll_node ll_node;
typedef struct linked_list linked_list;

typedef struct ll_node {
  unsigned int bytecode_index;
  char *id;
  int type;
  ll_node *next;
} ll_node;

typedef struct linked_list {
  ll_node *head;
  ll_node *tail;
  size_t size;
} linked_list;


//  linked_list_empty : Renvoie un pointeur sur une liste chaînée, NULL en cas
//    d'échec.
extern linked_list *linked_list_empty(void);

//  linked_list_search: Renvoie l'index dans le constant pool où id est définie,
//    -1 si id n'existe pas.
extern int linked_list_search(const linked_list *s, char *id);

//  linked_list_type_of_int: Renvoie le type (INT, BOOL, STRING...) de l'id dans
//    le constant pool, -1 si id n'existe pas.
extern int linked_list_type_of_id(const linked_list *s, char *id);

//  linked_list_insert: Insère dans la liste chaînée pointée par s, un id de
//    type type où sa définition se trouve à l'index index du constant pool.
//    Renvoie 0 en cas de succès, -1 sinon.
extern int linked_list_insert(linked_list *s, unsigned int index, char *id, int type);

//  linked_list_dispose : Libère les ressources allouées à *sptr ainsi qu'à la
//    liste chaînée associée puis affecte la valeur NULL à *sptr.
extern void linked_list_dispose(linked_list **sptr);


#endif                          //LINKEDLIST_H
