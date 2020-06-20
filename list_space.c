#include <stdlib.h>
#include <string.h>
#include "list_space.h"

int copy_item(t_item* dest, t_item orig) {
    dest->row=orig.row;
    dest->col=orig.col;
    dest->type=orig.type;
    dest->state=orig.state;
    dest->dir=orig.dir;
    dest->body= (char*) malloc((strlen(orig.body)+1)*sizeof(char));
    if (!dest->body)
        return 0;
    strcpy(dest->body, orig.body);
    return 1;
}

int initialize_sentinels(t_item *input) {
    input->body = (char*) malloc(sizeof(char)*2);
    if (!input->body)
        return 0;
    strcpy(input->body, "n");
    input->row = 0;
    input->col = 0;
    input->dir = NONE;
    input->type = 0;
    input->state = DEAD;
    return 1;
}

int initialize_list(t_list *l) {
    l->ini = (t_node*) malloc(sizeof(t_node));
    if (!l->ini)
        return 0;
    l->end = (t_node*) malloc(sizeof(t_node));
    if (!l->end)
        return 0;
    l->ini->prox=l->end;
    l->ini->prev=NULL;
    l->end->prev=l->ini;
    l->end->prox=NULL;
    l->current=NULL; /* Apontador atual inicia em NULL, por mera convenção. */
    l->size=0;
    l->end->item = (t_item*) malloc(sizeof(t_item));
    if (!l->end->item)
        return 0;
    initialize_sentinels(l->end->item);
    l->ini->item = (t_item*) malloc(sizeof(t_item));
    if (!l->ini->item)
        return 0;
    initialize_sentinels(l->ini->item);
    return 1;
}

int empty_list(t_list *l) {
    if (l->size == 0)
        return 1;
    else
        return 0;
}

void destroy_list(t_list *l) {
    t_node* aux=l->ini;
    l->current=l->ini;
    int i;
    for (i=-2; i!=l->size; i++) {
        l->current=aux->prox;
        free(aux->item->body);
        free(aux->item);
        free(aux);
        aux=l->current;
    }
    l->ini=NULL;
    l->end=NULL;
    l->current=NULL;
    l->size=0;
}

int insert_beginning_list(t_item item, t_list *l) {
    t_node *new;
    new = (t_node*) malloc(sizeof(t_node));
    if (!new)
        return 0;
    new->prox=l->ini->prox;
    new->prev=l->ini;
    new->item = (t_item*) malloc(sizeof(t_item));
    if (!new->item)
        return 0;
    copy_item(new->item, item);
    l->ini->prox->prev=new;
    l->ini->prox=new;
    l->size++;
    return 1;
}

int insert_end_list(t_item item, t_list *l) {
    t_node *new;
    new = (t_node*) malloc(sizeof(t_node));
    if (!new)
        return 0;
    new->prox=l->end;
    new->prev=l->end->prev;
    new->item = (t_item*) malloc(sizeof(t_item));
    if (!new->item)
        return 0;
    l->end->prev->prox=new;
    l->end->prev=new;
    copy_item(new->item, item);
    l->size++;
    return 1;
}

int size_list(int* size, t_list *l) {
    if (empty_list(l)) {
        *size=l->size;
        return 0;
    }
    *size=l->size;
    return 1;
}

int initialize_current_first(t_list *l) {
    if (empty_list(l))
        return 0;
    l->current=l->ini->prox;
    return 1;
}

int initialize_current_end(t_list *l) {
    if (empty_list(l))
        return 0;
    l->current=l->end->prev;
    return 1;
}

int check_current_item(t_item *item, t_list *l) {
    if (l->current && l->current->prox && l->current->prev) {
        copy_item(item, *l->current->item);
        return 1;
    }
    return 0;
}

void *increment_current(t_list *l) {
    if (l->current && l->current->prox) {
        l->current=l->current->prox;
        return l->current;
    }
    l->current=NULL;
    return NULL;
}

void *decrement_current(t_list *l) {
    if (l->current && l->current->prev) {
        l->current=l->current->prev;
        return l->current;
    }
    l->current=NULL;
    return NULL;
}

int update_current_item(t_item item, t_list *l) {
    if (l->current == NULL || empty_list(l))
        return 0;
    copy_item(l->current->item, item);
    return 1;
}

int remove_current_item(t_item *item, t_list *l) {
    if (l->current && l->current->prox && l->current->prev) {
        copy_item(item, *l->current->item);
        l->size--;
        t_node*aux = l->current->prev;
        l->current->prev->prox=l->current->prox;
        l->current->prox->prev=l->current->prev;
        free(l->current);
        l->current=aux->prev;
        return 1;
    }
    return 0;
}
