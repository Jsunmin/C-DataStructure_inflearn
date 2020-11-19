//
//  phoneBook.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/19.
//

#include <stdio.h>
#include <string.h>

#define CAPACITY 100
#define BUFFER_SIZE 100
char * names[CAPACITY]; // names
char * numbers[CAPACITY]; // phone numbers
int n = 0; // number of people in phone directory

void addOne (void);
void findOne (void);
void status (void);
void removeOne (void);

int phoneBook() {
    char command[BUFFER_SIZE];
    while(1) {
        printf("$ ");
        scanf("%s", command);
        
        if (strcmp(command, "add") == 0) {
            addOne();
        } else if (strcmp(command, "find") == 0) {
            findOne();
        } else if (strcmp(command, "status") == 0) {
            status();
        } else if (strcmp(command, "delete") == 0) {
            removeOne();
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}

void addOne() {
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    scanf("%s", buf1);
    scanf("%s", buf2);
    names[n] = strdup(buf1);
    /* strdup: 새로운 배열을 만들어 인자로 들어온배열을 카피해 넣고. (length = 기존배열 len + 1)
        새로운 배열의 시작 주소를 리턴함.
       strcpy: 2인자로 들어온 배열을 1인자에 그대로 복사해 넣음. (1,2인자 모두 미리 선언되어
     
       함수내에서 선언된 배열 주소를 넣어버리면 함수 끝날때 반환되면서 사라짐.. -> strdup 쓰는 이유!
       Stack 메모리 (지역변수 할당공간)이 아닌 곳에 넣고자!!
       ( cf. 전역변수 = data section / 동적할당 = Heap / 코드 = code / 지역변수 = Stack ~ 에 메모리 할당 )
     */
    numbers[n] = strdup(buf2);
    n++;
    printf("%s was added successfully.\n", buf1);
}

void findOne() {
    char buf[BUFFER_SIZE];
    scanf("%s", buf);
    
    for (int i = 0; i < n; i++) {
        if (strcmp(buf, names[i]) == 0) {
            printf("%s\n", numbers[i]);
            return;
        }
    }
    printf("No person named '%s' exists.\n", buf);
}

void status() {
    for (int i = 0; i < n; i++) {
        printf("%s  %s\n", names[i], numbers[i]);
    }
    printf("Total %d persons.\n", n);
};

void removeOne() {
    char buf[BUFFER_SIZE];
    scanf("%s", buf);
    
    for (int i = 0; i < n; i++) {
        if (strcmp(buf, names[i]) == 0) {
            names[i] = names[n-1];
            numbers[i] = numbers[n-1];
            n--;
            printf("'%s' was deleted successfully. \n", buf);
            return;
        }
    }
    printf("No person named '%s' exists.\n", buf);
};
