#include "definitions_space.h"

/* Funcao interna usada para copiar um item em outro. */
int copy_item(t_item* dest, t_item orig);

/* Funcao interna para inicializar as sentinelas da lista. */
int initialize_list(t_list *l);

int empty_list(t_list *l);

void destroy_list(t_list *l);

int insert_beginning_list(t_item item, t_list *l);

int insert_end_list(t_item item, t_list *l);

int size_list(int* size, t_list *l);

int initialize_current_first(t_list *l);

int initialize_current_end(t_list *l);

int check_current_item(t_item *item, t_list *l);

void *increment_current(t_list *l);

void *decrement_current(t_list *l);

/* Atualiza as informacoes do item atual. */
int update_current_item(t_item item, t_list *l);

int remove_current_item(t_item *item, t_list *l);
