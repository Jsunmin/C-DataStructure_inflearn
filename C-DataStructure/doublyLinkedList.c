//
//  DoublyLinkedList.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    char *data;
    struct node *next;
    struct node *prev;
};

typedef struct node Node;

Node *head; // 순행
Node *tail; // 역순행
int size = 0;

// 노드 추가시 4개의 링크 작업이 필요하다. q(앞노드) prev next & p(뒷노드) prev next
void dllAdd(Node* p) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = "test";
    newNode->next = p;
    newNode->prev = p->prev; // q
    p->prev->next = newNode; // q's next
    p->prev = newNode; // p's prev
}

// 노드 삭제 (p 노드) ~ 앞노드 q 뒷노드 w
void dllDelete(Node* p) {
    p->prev->next = p->next; // q -> w 연결
    p->next->prev = p->prev; // q <- w 연결
    free(p);
}
// 4가지 케이스를 고려해야 한다!
void add_after(Node *pre, char *item) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = item;
    new_node->prev = NULL;
    new_node->next = NULL;
    // 첫 노드 세팅 ( 전노드 및 head가 빔.. )
    if (pre == NULL && head == NULL) {
        head = new_node;
        tail = new_node;
    // 노드는 있고, 맨 앞에 삽입
    } else if (pre == NULL) {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    // 맨 끝에 삽입
    } else if (pre == tail) {
        new_node->prev = tail;
        pre->next = new_node;
        tail = new_node;
    // 특정 노드들의 사이 삽입
    } else {
        new_node->prev = pre;
        new_node->next = pre->next;
        pre->next->prev = new_node;
        pre->next = new_node;
    }
    size++;
}

void add_order_list(char *item) {
    Node *p = tail;
    while (p != NULL && strcmp(item, p->data) < 0) { // p data 값이 뒤에 위치하면
        p = p->prev;
    }
    add_after(p, item); // 4가지 경우의 수를 다 처리해줌
}
