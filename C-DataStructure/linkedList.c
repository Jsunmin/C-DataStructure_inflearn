//
//  linkedList.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    // 데이터 필드
    char *data;
    // 링크 필드 ( struct node 타입의 포인터 변수 )
    struct node *next;
    // 다음 노드 주소를 저장할 필드 ( 자신과 동일한 구조체에 대해 포인터를 가져서 "자기참조형 구조체"라고 부르기도 한다. )
};

typedef struct node Node;
// 1번째 노드를 저장할 공간
Node *head = NULL;

void linkedList() {
    // 1번 노드 생성
    head = (Node *) malloc(sizeof(Node));
    head->data = "Tuesday";
    head->next = NULL;
    
    // 2번 노드 생성 (꼬리노드)
    Node *q = (Node *) malloc(sizeof(Node));
    q->data = "Thursday";
    q->next = NULL;
    head->next = q; // 1번째 링크필드에 자기 주소 맵핑
    
    // 2번 노드 생성 (머리노드)
    Node *w = (Node *) malloc(sizeof(Node));
    w->data = "Monday";
    w->next = head; // 1번째 주소를 자기 링크필드에 맵핑
    head = w; // head 포인터 변수에 자기 주소 넣음

    // print
    Node *p = head;
    while(p != NULL) {
        printf("%s\n", p->data);
        p = (*p).next; // p = p->next;
    }
}

// &head를 받음 (포인터변수의 주소값이므로 **)과 item을 인자로함
void addFirst1(Node **ptrHead, char* item) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = item;
    temp->next = *ptrHead;
    *ptrHead = temp;
}

// call by value, 새로운 노드의 주소값만 리턴
Node* addFirst2(char* item) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = item; // 데이터필드 구조체의 주소값을 넣어줌
    temp->next = head; // head Node 구조체의 주소값을 넣어줌
    return temp;
}

// 특정 노드 뒤에 새로운 노드 삽입하기
int addAfter(Node* prev, char* item) {
    if (prev == NULL) {
        return 0;
    }
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = item;
    temp->next = prev->next; // 그전 Node next를 받아옴 (다음 노드 주소)
    prev->next = temp; // 전 노드의 next 값에 temp 기입
    return 1;
}

// head 노드 삭제
Node* removeFirst() {
    if (head == NULL) {
        return NULL;
    }
    Node *tmp = head;
    head = head->next; // 헤드가 바라보는 주소에 다음 값으로 대치
    return tmp;
}
// 특정 노드 뒤 노드 삭제
Node* removeAfter(Node* prev) {
    Node *tmp = prev->next;
    if (tmp == NULL) {
        return 0;
    }
    // 앞노드가 다음다음 노드의 주소를 링크필드로 취해야 한다.
    // prev->next = prev->next->next;
    prev->next = tmp->next;
    return tmp;
}
// 노드 순회
Node* find(char *word) {
    Node *p = head;
    while ( p != NULL ) {
        if (strcmp(p->data, word) == 0) {
            return p;
        }
        // 계속 traverse
        p = p->next;
    }
    return NULL;
}
// 노드 순회2
Node* getNode(int index) {
    if (index < 0) {
        return NULL;
    }
    Node *p = head;
    for (int i = 0; i < index && p != NULL; i++) {
        p = p->next;
    }
    return p;
}

int addNode(int index, char* item) {
    if (index < 0) {
        return 0;
    }
    // 0인 경우 ~ case by case 접근이 필요!
    if (index == 0) {
        addFirst2(item);
        return 1;
    }
    // 0이 아닌 경우
    Node *prev = getNode(index - 1);
    if (prev != NULL) {
        addAfter(prev, item);
        return 1;
    }
    return 0;
}

Node* removeNode(int index) {
    if (index < 0) {
        return 0;
    }
    // 0인 경우 ~ case by case 접근이 필요!
    if (index == 0) {
        return removeFirst();
    }
    // 0이 아닌 경우
    Node *prev = getNode(index - 1);
    if (prev != NULL) {
        return removeAfter(prev);
    }
    return NULL;
}

Node* findAndRemove(char *item) {
    Node *p = head;
    Node *q = NULL; // p의 prev node 포인터
    while (p != NULL && strcmp(p->data, item) != 0) {
        q = p;
        p= p->next;
    }
    if (p == NULL) {
        return NULL;
    }
    if (q == NULL) { // p != NULL && q == NULL -> headNode를 지워야 함.
        return removeFirst();
    } else {
        return removeAfter(q);
    }
}
