#include <stdbool.h>

#ifndef SGF_LINKEDLIST_H
#define SGF_LINKEDLIST_H

typedef struct ll_node ll_node;

typedef struct ll_node {
  unsigned int bytecode_index;
  char *id;
  void *value;
  ll_node *next;
} ll_node;

typedef struct linked_list {
  ll_node *head;
  ll_node *tail;
  size_t size;
} linked_list;

typedef struct linked_list linked_list;

//  linked_listS_empty : Créer un objet capable de gérer une liste dynamique simplement
//    chainée sur le type (void *). La liste est initialement vide. Renvoie un
//    pointeur vers l'objet en cas de succès, NULL en cas d'échec
extern linked_list *linked_list_empty(void);

//  linked_list_is_empty : Renvoie true ou false selon que la liste associée à s
//    est vide ou non
extern bool linked_list_is_empty(const linked_list * s);

//  linked_list_length : Renvoie la longueur de la liste associée à s
extern size_t linked_list_length(const linked_list * s);

//  linked_list_head_value: Si la liste associée à s n'est pas
//    vide, affecte à *ptr la valeur de l'élément de tête et renvoie zéro.
//    Renvoie une valeur non nulle sinon
extern int linked_list_head_value(const linked_list * s, void *ptr);

//  linked_list_search: Renvoie l'address de la premiére valeur égale à ptr au
//    sens de compar, en cas d'érreur ou s'il ne trouve rien renvoie NULL.
extern int linked_list_search(const linked_list * s, char *id);

//  linked_list_insert: Insère ptr a la fin de la liste associée à s. Renvoie
//    zéro en cas de succès, une valeur non nulle en cas d'échec
extern int linked_list_insert(linked_list * s,  unsigned int index, char *id, void *ptr);

//  linked_list_remove : Retire l'object pointer par ptr de la liste chaînée,
//    renvoie son addresse en cas de succès, NULL sinon.
extern void *linked_list_remove(linked_list * s, void *ptr);

//  linked_list_dispose : Si *sptr ne vaut pas NULL, libère les ressources
//    allouées à *sptr ainsi qu'à la liste associée puis affecte la valeur NULL
//    à *sptr
extern void linked_list_dispose(linked_list ** sptr);


#endif                          //SGF_LINKEDLIST_H
