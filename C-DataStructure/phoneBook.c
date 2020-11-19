//
//  phoneBook.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_CAPACITY 3 // 배열 재할당 테스트를 위해 작은 사잉즈로
#define BUFFER_SIZE 50

char ** names; // char* 타입의. 배열의 이름이므로 char** 타입의 변수이다.
char ** numbers;

int capacity = INIT_CAPACITY; // size of array
int n = 0; // number of people in phone directory

char delim[] = " "; // 토크나이저에 쓰이는 구분 문자열
/*
   문자열 토크나이저: 구분문자(delimiter)를 이용해 하나의 긴 문자열을 작은 문자열들로 자르는 일
    잘라진 작은 문자열을 보통 토큰이라 함 ( C ~ strtok function 사용하기도 한다 )
 
   cf strtok: 원본 문자열을 변화시킴 (토큰 끝마다'\0'을 삽입함) -> 원본 복사한 것에 시행해야 손실이 없다.
     strtok시행 후, 문자열 프린트하면, 1번째 토큰만 나타남 ('\0' 때문..)
    - 새로운 배열을 생성하지 않는다. token = strtok( str, delim ); token = strtok( NULL, delim );
    - 원본 문자열의 토큰 (xx'\0') 까지의 메모리주소를 리턴한다.
 */

// 배열 names와 numbers 생성
void init_directory() {
    names = (char **)malloc(INIT_CAPACITY * sizeof(char *) );
    numbers = (char **)malloc(INIT_CAPACITY * sizeof(char *) );
    // malloc ~ 인자로 받는 사이즈의 byte 만큼 메모리 할당 (stdlib.h에 포함)
    // 위의 경우 3 * 4 (포인터크기) = 12 byte
}
void process_command (void);

int phoneBook() {
    init_directory();
    process_command();
    return 0;
}

void readAll (char* fileName);
void addOne(char* name, char* number);
void findOne (char* name);
void status (void);
void removeOne (char* name);
void saveAll (char* fileName);

// line 단위 입력은 fgets, getline 등의 함수를 이용할 수도 있다.
// 만들 문자열과 문자열의 최대 사이즈 제한을 받아온다.
int readLine (char str[], int limit) {
    int ch, i = 0;
    // 한자씩 stdin을 받을 때, newline 전까지 읽는다.
    while( (ch = getchar()) != '\n' ) {
        // null 을 위한 1칸
        if( i < limit - 1 ) {
            str[i++] = ch;
        }
    }
    str[i] = '\0';
    // 총 사이즈를 반환
    return i;
};

// 실제 비즈니스 로직
void process_command() {
    char commandLine[BUFFER_SIZE]; // 한 라인 전체 버퍼
    char *command, *argument1, *argument2; // 명령어, 1인자 2인자
    
    while(1) {
        printf("$ ");
        
        if (readLine(commandLine, BUFFER_SIZE) <= 0) {
            // 바로 newline을 치면, while 재개
            continue;
        }
        command = strtok(commandLine, delim); // 1번째 토큰 생성
        if (command == NULL) {
            continue;
        }
        if (strcmp(command, "read") == 0) {
            argument1 = strtok(NULL, delim); // 2번째 토큰 생성
            if (argument1 == NULL) {
                printf("File name required.\n");
                continue;
            }
            readAll(argument1); // 파일명 넘기면서 호출
        } else if (strcmp(command, "add") == 0) {
            argument1 = strtok(NULL, delim); // 토큰2
            argument2 = strtok(NULL, delim); // 토큰3
            if (argument1 == NULL || argument2 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            addOne(argument1, argument2);
            printf("%s was added successfully.\n", argument1);
        } else if (strcmp(command, "find") == 0) {
            argument1 = strtok(NULL, delim);
            if (argument1 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            findOne(argument1);
        } else if (strcmp(command, "status") == 0) {
            status();
        } else if (strcmp(command, "delete") == 0) {
            argument1 = strtok(NULL, delim);
            if (argument1 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            removeOne(argument1);
        } else if (strcmp(command, "save") == 0) {
            argument1 = strtok(NULL, delim); // 토큰2
            argument2 = strtok(NULL, delim); // 토큰3
            // $ save as documents.txt
            if (argument1 == NULL || strcmp("as", argument1) != 0 || argument2 == NULL) {
                printf("Invalid command format.\n");
                continue;
            }
            saveAll(argument2);
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
}


int search(char* name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(name, names[i]) == 0) {
            return i;
        }
    }
    return -1;
};

void reallocate() {
    capacity *= 2;
    // 기존보다 2배 크기로 할당
    char **tmp1 = (char **)malloc(capacity * sizeof(char *));
    char **tmp2 = (char **)malloc(capacity * sizeof(char *));
    
    // 원본 배열에 있는 값을, 새로 생성한 배열에 넣어줌
    for (int i = 0; i < n; i++) {
        tmp1[i] = names[i];
        tmp2[i] = numbers[i];
    }
    
    // 원본 배열들은 malloc, 동적할당에 의해 만들어졌으므로, garbage(기존배열) 놓아주는 free로 메모리공간 반환한다.
    free(names);
    free(numbers);

    // 메모리 반환 후, 새로 만든 배열 바라보도록 처리
    names = tmp1;
    numbers = tmp2;
};

void readAll (char* fileName) {
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    
    // 파일 처리를 위한 파일 포인터 받아옴 (파일 스트림 오픈)
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    // 파일 끝에 도달할 때 까지 (EOF) 읽고 배열에 저장!
    while ((fscanf(fp, "%s", buf1) != EOF)) {
        fscanf(fp, "%s", buf2);
        addOne(buf1, buf2);
    }
    // 파일 처리 후 스트림 종료
    fclose(fp);
};

void addOne(char* name, char* number) {
    if (n >= capacity) {
        reallocate(); // 배열이 다 차면 재할당처리
    }
    // 마지막 원소부터 시작해 돌면서, 비교 & 한칸씩 미룸
    int i = n - 1;
    // strcmp: 같으면 0 / 전자가 사전문자식에서 후자보다 더 크면(뒤에위치해야함) + / 작으면 -
    while(i >= 0 && strcmp(names[i], name) > 0) {
        names[i+1] = names[i];
        numbers[i+1] = numbers[i];
        i--;
    }
    names[i+1] = strdup(name);
    numbers[i+1] = strdup(number);
    /* strdup: 새로운 배열을 만들어 인자로 들어온배열을 카피해 넣고. (length = 기존배열 len + 1)
        새로운 배열의 시작 주소를 리턴함.
       strcpy: 2인자로 들어온 배열을 1인자에 그대로 복사해 넣음. (1,2인자 모두 미리 선언되어
     
       함수내에서 선언된 배열 주소를 넣어버리면 함수 끝날때 반환되면서 사라짐.. -> strdup 쓰는 이유!
       Stack 메모리 (지역변수 할당공간)이 아닌 곳에 넣고자!!
       ( cf. 전역변수 = data section / 동적할당 = Heap / 코드 = code / 지역변수 = Stack ~ 에 메모리 할당 )
     */
    n++;
}

void findOne(char* name) {
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
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

void removeOne(char* name) {
    int index = search(name); // return -1 if not exists
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }
    // 무조건 하나가 빠지므로 n-1까지만 작업한다.
    for (int i = index; i < n-1; i++) {
        names[i] = names[i+1];
        numbers[i] = numbers[i+1];
    }
    n--;
    printf("'%s' was deleted successfully. \n", name);
};

void saveAll (char* fileName) {
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
// 정확히는 c
