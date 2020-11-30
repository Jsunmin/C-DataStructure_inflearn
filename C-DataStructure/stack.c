//
//  stack.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/30.
//

#include <stdlib.h>
#include "stack.h"

#define MAX_CAPACITY 100

// 배열로 구현
char stack[MAX_CAPACITY];
int top = -1; // index of top ele

int is_empty() {
    return top == -1;
}

int is_full() {
    return top == MAX_CAPACITY-1;
}

void push(char ch) {
    // 가득차면 넣을 수 없음
    if(is_full()) {
        return;
    }
    top++;
    stack[top] = ch;
}

char pop() {
    char tmp = stack[top];
    top--;
    return tmp;
}

char peek() {
    return stack[top];
}

// 연결리스트로 구현
struct lnode {
    char *data;
    struct lnode *next;
};
typedef struct lnode LNode;

LNode *ltop = NULL;

int lIs_empty() {
    return ltop == NULL;
}

char *lpeek() {
    if(lIs_empty()) {
        return NULL;
    }
    return ltop->data;
}

void lpush(char *item) {
    LNode *p = (LNode *)malloc( sizeof(LNode) );
    p->data = item;
    p->next = ltop;
    ltop = p;
}

char *lpop() {
    if(lIs_empty()) {
        return NULL;
    }
    char *result = ltop->data;
    ltop = ltop->next;
    return result;
}
