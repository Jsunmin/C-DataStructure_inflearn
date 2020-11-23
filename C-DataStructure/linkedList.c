//
//  linkedList.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/23.
//

#include <stdio.h>
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
