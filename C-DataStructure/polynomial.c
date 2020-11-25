//
//  polynomial.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/25.
//

// 링크드리스트 실습
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POLYS 100
#define BUFFER_LENGTH 100

struct term {
    int coef;
    int expo;
    struct term *next;
};

typedef struct term Term;

typedef struct aPolynomial {
    char name; // 식 이름
    Term *first; // 식 링크필드
    int size; // 다항식 개수
} Polynomial;

// 다항식들에 대한 포인터 배열
// 각 원소들은 다항식 하나하나의 포인터를 갖는다.
Polynomial *polys[MAX_POLYS];

int n = 0; // 저장된 다항식 개수

void polynomial() {
    
}

Term *createTermInstance() {
    Term *t = (Term *)malloc(sizeof(Term));
    t->coef = 0;
    t->expo = 0;
    return t;
}

Polynomial *createPolynomialInstance(char name) {
    Polynomial *ptrPoly = (Polynomial *)malloc(sizeof(Polynomial));
    ptrPoly->name = name;
    ptrPoly->size = 0;
    ptrPoly->first = NULL;
    return ptrPoly;
}

void addTerm(int c, int e, Polynomial* poly) {
    if (c == 0) {
        return;
    }
    Term *p = poly->first, *q = NULL;
    // 타겟 지수의 항(p)과 직전항(q)을 찾아낸다.
    while(p != NULL && p->expo == e) {
        q = p;
        p =p->next;
    }
    // 추가하려는 항의 동일 차수가 이미 존재 -> 기존 항 계수 변경
    if (p != NULL && p->expo == e) {
        p->coef += c;
        // 항 삭제 필요한 경우 (계수가 0이 됨..)
        if (p->coef == 0) {
            if (q == NULL) { // 타겟항이 headNode
                poly->first = p->next;
            } else { // 타겟하잉 headNode X
                q->next = p->next;
            }
            poly->size--;
            // 링크 변경후 타겟항 반환
            free(p);
        }
        return;
    }
    // 새로운 항 삽입 ( q -> new term -> p )
    Term *term = createTermInstance();
    term->coef = c;
    term->expo = e;
    
    if (q == NULL) { // 다항식 포인터의 head 대체
        term->next = poly->first;
        q->next = term;
    } else { // 다항식 포인터 내부에 삽입
        term->next = p;
        q->next = term;
    }
    poly->size++;
}

int evalNode ( Term *term, int x );
int eval(Polynomial *poly, int x) {
    int result = 0;
    Term*t = poly->first;
    while(t != NULL) {
        result += evalNode(t, x);
        
        t = t->next;
    }
    return result;
}
int evalNode(Term *term, int x) {
    int result = term->coef;
    for (int i = 0; i < term->expo; i++) {
        result *= x;
    }
    return result;
}

void printTerm (Term *pTerm);
void printPoly(Polynomial* p) {
    printf("%c=", p->name);
    Term *t = p->first;
    while(t != NULL) {
        printTerm(t);
        printf("+");
        t = t->next;
    }
}
void printTerm (Term *pTerm) {
    printf("%dx^%d", pTerm->coef, pTerm->expo );
}

int readLine (FILE* fp, char str[], int n); // from phoneBook
void processCommand() {
    char commandLine[BUFFER_LENGTH];
    char copied[BUFFER_LENGTH];
    char *command, *arg1, *arg2;
    while(1) {
        printf("$ ");
        if (readLine(stdin, commandLine, BUFFER_LENGTH) <= 0) {
            continue;
        }
        strcpy(copied, commandLine);
        command = strtok(commandLine, " ");
        if (strcmp(command, "print") == 0) {
            arg1 = strtok(NULL, " ");
            if (arg1 == NULL) {
                 printf("Invalid arguments.\n");
                 continue;
             }
//             handlePrint(arg1[0]);
        } else if (strcmp(command, "calc") == 0) {
             arg1 = strtok(NULL, " ");
             if (arg1 == NULL) {
                 printf("Invalid arguments.\n");
                 continue;
             }
             arg2 = strtok(NULL, " ");
             if (arg2 == NULL) {
                 printf("Invalid arguments.\n");
                 continue;
             }
//           handleCalc(arg1[0], arg2);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
//          handleDefinition(copied);
        }
    }
}
