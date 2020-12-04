//
//  queue.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/12/03.
//

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct nodeQ {
    Item data;
    struct nodeQ *next;
};
struct queue_type {
    struct nodeQ *front;
    struct nodeQ *rear;
    int size;
};

void terminate(const char *message) {
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}
int get_size_q(Queue q) {
    return q->size;
};

Queue create() {
    Queue q = (Queue)malloc(sizeof(struct queue_type));
    if ( q == NULL ) {
        terminate("Error in create: queue could not be created.");
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
};
bool is_empty_q(Queue q) {
    return q->front = NULL; // q->size == 0
};
void make_empty_q(Queue q) {
    while(!is_empty_q(q)) { // 빌때까지 프론트에서 뽑아냄
        dequeue(q);
    }
    q->size = 0; // 리셋
};
void destroy(Queue q) {
    make_empty_q(q);
    free(q);
};


void enqueue(Queue q, Item i) {
    struct nodeQ *new_node = (struct nodeQ *)malloc(sizeof(struct nodeQ));
    if ( new_node == NULL ) {
        terminate("Error in push: queue is full.");
    }
    new_node->data = i;
    new_node->next = NULL;
    if (q->front == NULL) { // first
        q->front = new_node;
        q->rear = new_node;
    } else { // tail
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
};
Item dequeue(Queue q) {
    struct nodeQ *old_front;
    Item i;
    if (is_empty_q(q)) {
        terminate("Error in dequeue: queue is empty.");
    }
    old_front = q->front;
    i = old_front->data;
    q->front = old_front->next; // 헤드 대체
    if (q->front == NULL) { // 노드가 하나였을 때에는 rear까지 처리!
        q->rear = NULL;
    }
    free(old_front);
    q->size--;
    return i;
};
Item peek_q(Queue q) {
    if(is_empty_q(q)) {
        terminate("Error in peak: queue is empty.");
    }
    return q->front->data;
};

// circular array queue
#define INIT_CAP 100
struct queue_type_arr {
    Item *contents;
    int front;
    int rear;
    int size; // 저장된 데이터 개수
    int capacity; // 배열 contents 크기
};

Queue_arr create_arr() {
    Queue_arr q = (Queue_arr)malloc(sizeof(struct queue_type_arr));
    if ( q == NULL ) {
        terminate("Error in create: queue could not be created.");
    }
    q->contents = (Item *)malloc(INIT_CAP * sizeof(Item));
    if ( q->contents == NULL ) {
        free(q);
        terminate("Error in create: queue could not be created.");
    }
    q->front = 0;
    q->rear = -1; // 로직상, 원소 추가하면 0부터 시작한다.
    q->size = 0;
    q->capacity = INIT_CAP;
    return q;
}

void destroy_arr(Queue_arr q) {
    free(q->contents); // 배열 반환
    free(q); // 구조체 반환
}
void make_empty_arr(Queue_arr q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}
bool is_empty_arr(Queue_arr q) {
    return q->size == 0;
}
bool is_full_arr(Queue_arr q) {
    return q->size == q->capacity;
}
void reallocate_arr(Queue_arr q) {
    Item *tmp = (Item *)malloc(2 * q->capacity * sizeof(Item));
    if (tmp == NULL) {
        terminate("Error in create: queue could not be expanded.");
    }
    // 그대로 복붙하고 사이즈만 키우면, 순환큐인데, 뒤에 공간낭비하는 형태가 됨..
    int j = q->front;
    for (int i = 0; i < q->size; i++) {
        tmp[i] = q->contents[j]; // 사이즈업한 신규배열 0부터, 기존배열 front 원소부터 차례로 집어 넣음.
        j = (j + 1) % q->capacity; // 기존 배열은 서큘러 큐임을 감안해서, j 조정함.
    }
    free(q->contents); // 기존 배열 반환
    // 새로운 배열에 0부터 나열하면서, front rear 위치 재조정!
    q->front = 0;
    q->rear = q->size - 1;
    q->contents = tmp;
    q->capacity *= 2;
}
void enqueue_arr(Queue_arr q, Item i) {
    if (is_full_arr(q)) {
        reallocate_arr(q);
    }
    q->rear = (q->rear + 1)%q->capacity; // 0 ~ capa 까지의 인덱스를 부여받음 - 한정된 공간에서의 후미 관리
    q->contents[q->rear] = i; // 세팅한 rear 위치에 item 추가
    q->size++;
}
Item dequeue_arr(Queue_arr q, Item i) {
    if(is_empty_arr(q)) {
        terminate("Error in dequeue: queue is empty.");
    }
    // 맨앞노드 따로 저장
    Item result = q->contents[q->front];
    // 헤드 노드가 빠져나감으로 한칸씩 땡겨짐 (다음 노드가 해드로 됨)
    q->front = (q->front + 1)%q->capacity; // front 또한 한정된 배열 내에서 위치 변경한다. ~ 자연스레 서큘러 큐
    q->size--;
    return result;
}
Item peek_arr(Queue_arr q) {
    if (is_empty_arr(q)) {
        terminate("Error in peek: queue is empty.");
    }
    return q->contents[q->front]; // 배열의 머리 리턴
}

