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

void readAll (void);
void addOne (void);
void findOne (void);
void status (void);
void removeOne (void);
void saveAll (void);
int search (char* name);

int phoneBook() {
    char command[BUFFER_SIZE];
    while(1) {
        printf("$ ");
        scanf("%s", command);
        if (strcmp(command, "read") == 0) {
            readAll();
        } else if (strcmp(command, "add") == 0) {
            addOne();
        } else if (strcmp(command, "find") == 0) {
            findOne();
        } else if (strcmp(command, "status") == 0) {
            status();
        } else if (strcmp(command, "delete") == 0) {
            removeOne();
        } else if (strcmp(command, "save") == 0) {
            saveAll();
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}

int search(char* name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(name, names[i]) == 0) {
            return i;
        }
    }
    return -1;
};

void readAll () {
    char fileName[BUFFER_SIZE], buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    // 파일 이름 입력
    scanf("%s", fileName);
    // 파일 처리를 위한 파일 포인터 받아옴 (파일 스트림 오픈)
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    // 파일 끝에 도달할 때 까지 (EOF) 읽고 배열에 저장!
    while ((fscanf(fp, "%s", buf1) != EOF)) {
        fscanf(fp, "%s", buf2);
        names[n] = strdup(buf1);
        numbers[n] = strdup(buf2);
        n++;
    }
    // 파일 처리 후 스트림 종료
    fclose(fp);
};

void addOne() {
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    scanf("%s", buf1);
    scanf("%s", buf2);
    int i = n - 1;
    // strcmp: 같으면 0 / 전자가 사전문자식에서 후자보다 더 크면(뒤에위치해야함) + / 작으면 -
    while(i >= 0 && strcmp(names[i], buf1) > 0) {
        names[i+1] = names[i];
        numbers[i+1] = numbers[i];
        i--;
    }
    names[i+1] = strdup(buf1);
    numbers[i+1] = strdup(buf2);
    /* strdup: 새로운 배열을 만들어 인자로 들어온배열을 카피해 넣고. (length = 기존배열 len + 1)
        새로운 배열의 시작 주소를 리턴함.
       strcpy: 2인자로 들어온 배열을 1인자에 그대로 복사해 넣음. (1,2인자 모두 미리 선언되어
     
       함수내에서 선언된 배열 주소를 넣어버리면 함수 끝날때 반환되면서 사라짐.. -> strdup 쓰는 이유!
       Stack 메모리 (지역변수 할당공간)이 아닌 곳에 넣고자!!
       ( cf. 전역변수 = data section / 동적할당 = Heap / 코드 = code / 지역변수 = Stack ~ 에 메모리 할당 )
     */
    n++;
    printf("%s was added successfully.\n", buf1);
}

void findOne() {
    char buf[BUFFER_SIZE];
    scanf("%s", buf);
    int index = search(buf);
    if (index == -1) {
        printf("No person named '%s' exists.\n", buf);
        return;
    } else {
        printf("%s\n", numbers[index]);
    }
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
    int index = search(buf); // return -1 if not exists
    if (index == -1) {
        printf("No person named '%s' exists.\n", buf);
        return;
    }
    // 무조건 하나가 빠지므로 n-1까지만 작업한다.
    for (int i = index; i < n-1; i++) {
        names[i] = names[i+1];
        numbers[i] = numbers[i+1];
    }
    n--;
    printf("'%s' was deleted successfully. \n", buf);
};

void saveAll () {
    char fileName[BUFFER_SIZE], temp[BUFFER_SIZE];
    // $ save as documents.txt
    scanf("%s", temp); // as 라는 가운데 문자열 씹기 위함
    scanf("%s", fileName); // 실제 파일명
    
    FILE *fp = fopen(fileName, "w"); // 없으면 만들고 있으면 덮어쓴다!
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s %s\n", names[i], numbers[i]);
    }
    fclose(fp);
};

// cf. 저장한 파일은 /Users/jeongseonmin/Library/Developer/Xcode/DerivedData/프로젝트이름/Build/Products/Debug ~ 에 프로덕트와 함께 위치한다!
// 정확히는 ~/Library/Developer/Xcode/DerivedData/C-DataStructure-ciqmpnskopxfeueltlrmriytwrqv/Build/Products/Debug
