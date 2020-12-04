//
//  queue.h
//  C-DataStructure
//
//  Created by 정선민 on 2020/12/03.
//

#ifndef queue_h
#define queue_h

#include <stdbool.h>

typedef int Item;

typedef struct queue_type *Queue;
typedef struct queue_type_arr *Queue_arr;

Queue create();
void destroy(Queue q);
void make_empty_q(Queue q);
bool is_empty_q(Queue q);
void enqueue(Queue q, Item i);
Item dequeue(Queue q);
Item peek_q(Queue q);
int get_size_q(Queue q);

#endif /* queue_h */
