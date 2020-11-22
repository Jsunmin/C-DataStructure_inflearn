//
//  phoneBook.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_CAPACITY 100
#define BUFFER_LENGTH 100

// 구조체 struct person을 정의 & 이걸 'struct person' -> 'Person'으로 renaming함!
typedef struct person {
    char *name;
    char *number;
    char *email;
    char *group;
} Person;

Person **directory; // -> dirctory (포인터) direcotry[i] (포인터) ~ 2번 포인터 타면 Person 참조

int capacity;
int n = 0;

void init() {
    directory = (Person **)malloc(INIT_CAPACITY * sizeof(Person *));
    capacity = INIT_CAPACITY;
    n = 0;
}

void readAll (char* fileName);
void handleAdd (char* name);
void addOne (char* name, char* number, char* email, char* group);
void findOne (char* name);
void status (void);
void removeOne (char* name);
void saveAll (char* fileName);
int composeName (char str[], int limit);
// line 단위 입력은 fgets, getline 등의 함수를 이용할 수도 있다.
// 만들 문자열과 문자열의 최대 사이즈 제한을 받아온다.
int readLine (FILE* fp, char str[], int n) {
    int ch, i = 0;
    // 키보드(stdin) || 파일(fp)로부터 한자씩 읽는다! ~ newline('\0') || EOF(파일 마지막) 전까지
    while( (ch = fgetc(fp)) != '\n' && ch != EOF ) {
        if( i < n ) {
            str[i++] = ch;
        }
    }
    str[i] = '\0';
    // 총 사이즈를 반환
    return i;
};

int phoneBook() {
    init();
    char commandLine[BUFFER_LENGTH]; // 한 라인 전체 버퍼
    char *command, *argument;
    char nameStr[BUFFER_LENGTH];
    
    while(1) {
        printf("$ ");
        
        if (readLine(stdin, commandLine, BUFFER_LENGTH) <= 0) {
            // 바로 newline을 치면, while 재개
            continue;
        }
        // 넘어가면, 줄에 유효한 데이터가 있다는 것! ~ 문자열을 토크나이징해서 데이터를 받아들인다!
        
        /*
           문자열 토크나이저: 구분문자(delimiter)를 이용해 하나의 긴 문자열을 작은 문자열들로 자르는 일
            잘라진 작은 문자열을 보통 토큰이라 함 ( C ~ strtok function 사용하기도 한다 )
         
           cf strtok: 원본 문자열을 변화시킴 (토큰 끝마다'\0'을 삽입함) -> 원본 복사한 것에 시행해야 손실이 없다.
             strtok시행 후, 문자열 프린트하면, 1번째 토큰만 나타남 ('\0' 때문..)
            - 새로운 배열을 생성하지 않는다. token = strtok( str, delim ); token = strtok( NULL, delim );
            - 원본 문자열의 토큰 (xx'\0') 까지의 메모리주소를 리턴한다.
         */
        command = strtok(commandLine, " "); // 1번째 토큰 생성

        if (strcmp(command, "read") == 0) {
            argument = strtok(NULL, " "); // 2번째 토큰 생성
            if (argument == NULL) {
                printf("File name required.\n");
                continue;
            }
            readAll(argument); // 파일명 넘기면서 호출
        } else if (strcmp(command, "add") == 0) {
            if(composeName(nameStr, BUFFER_LENGTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            handleAdd(nameStr);
            printf("%s was added successfully.\n", nameStr);
        } else if (strcmp(command, "find") == 0) {
            if(composeName(nameStr, BUFFER_LENGTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            findOne(nameStr);
        } else if (strcmp(command, "status") == 0) {
            status();
        } else if (strcmp(command, "delete") == 0) {
            if(composeName(nameStr, BUFFER_LENGTH) <= 0) {
                printf("Invalid arguments.\n");
                continue;
            }
            removeOne(nameStr);
        } else if (strcmp(command, "save") == 0) {
            // $ save as documents.txt
            argument = strtok(NULL, " "); // 토큰2
            if (strcmp("as", argument) != 0) {
                printf("Invalid arguments.\n");
                continue;
            }
            argument = strtok(NULL, " ");
            if (argument == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            saveAll(argument);
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}

int composeName(char str[], int limit) {
    char* ptr;
    int length = 0;

    ptr = strtok(NULL," "); // 토크나이저 이어받아서 처리
    if (ptr == NULL) {
        return 0;
    }
    strcpy(str, ptr); // 자동으로 \0 붙음
    length += strlen(ptr);
    
    while( (ptr = strtok(NULL, " ")) != NULL ) {
        if (length + strlen(ptr) + 1 < limit) {
            str[length++] = ' '; // 기존에 이어 붙이는 str에 ' ' 추가
            str[length] = '\0'; // 문자열 함수 이용을 위해 마감 처리
            strcat(str, ptr); // str에 토큰 쪼갠 ptr을 붙임 ( 자동으로 \0 붙음 )
            length += strlen(ptr);
        }
    }
    return length;
}

int search(char* name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(name, directory[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}

void reallocate() {
    capacity *= 2;
    Person **tmp = (Person **)malloc(capacity*sizeof(Person *));
    for (int i = 0; i < n; i++) {
        tmp[i] = directory[i]; // Person 객체의 주소를 복사한다! (효율 GOOD!)
    }
    free(directory);
    directory = tmp;
}

void printPerson(Person *p) {
    printf("%s:\n", p->name);
    printf("    Phone: %s\n", p->number);
    printf("    Email: %s\n", p->email);
    printf("    Group: %s\n", p->group);
}

void realsePerson(Person *p) {
    free(p->name);
    if(p->number != NULL) free(p->number);
    if(p->email != NULL) free(p->email);
    if(p->group != NULL) free(p->group);
}

void handleAdd (char* name) {
    char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
    
    printf("  Phone:  ");
    readLine(stdin, number, BUFFER_LENGTH);
    
    printf("  Email:  ");
    readLine(stdin, email, BUFFER_LENGTH);
    
    printf("  Group:  ");
    readLine(stdin, group, BUFFER_LENGTH);
 
    addOne(
        strdup(name),
        (char *)(strlen(number) > 0 ? strdup(number) : NULL),
        (char *)(strlen(number) > 0 ? strdup(email) : NULL),
        (char *)(strlen(number) > 0 ? strdup(group) : NULL)
    );
};

void readAll (char* fileName) {
    char buffer[BUFFER_LENGTH];
    char *name, *number, *email, *group;
    char *token;
    // 파일 처리를 위한 파일 포인터 받아옴 (파일 스트림 오픈)
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    // 파일 끝에 도달할 때 까지 (EOF) 읽고 배열에 저장!
    while (1) {
        if (readLine(fp, buffer, BUFFER_LENGTH) <= 0) {
            break;
        }
        // 파일은 # 를 구분자로 함 (이름은 필수)
        name = strtok(buffer, "#");
        token = strtok(NULL, "#"); // 이 후 값은 없을수도 있다.
        if (strcmp(token, " ") == 0) {
            number = NULL; // 없으면 NULL로 설정함
        } else {
            number = strdup(token);
        }
        token = strtok(NULL, "#");
        if (strcmp(token, " ") == 0) {
            email = NULL;
        } else {
            email = strdup(token);
        }
        token = strtok(NULL, "#");
        if (strcmp(token, " ") == 0) {
            group = NULL;
        } else {
            group = strdup(token);
        }
        addOne(strdup(name), number, email, group);
    }
    // 파일 처리 후 스트림 종료
    fclose(fp);
};

void addOne(char* name, char* number, char* email, char* group) {
    if ( n >= capacity) {
        reallocate();
    }
    int i = n - 1; // 마지막 원소부터 시작해 돌면서, 비교 & 한칸씩 미룸
    // 구조체 포인터에 접근하는 문법! ->
    while(i >= 0 && strcmp(directory[i]->name, name) > 0) { // strcmp: 같으면 0 / 전자가 사전문자식에서 후자보다 더 크면(뒤에위치해야함) + / 작으면 -
        directory[i+1] = directory[i];
        i--;
    }
    directory[i+1] = (Person *)malloc(sizeof(Person));
    directory[i+1]->name = name;
    directory[i+1]->number = number;
    directory[i+1]->email = email;
    directory[i+1]->group = group;
    n++;
}

void findOne(char* name) {
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    } else {
        printPerson(directory[index]);
    }
}

void status() {
    for (int i = 0; i < n; i++) {
        printPerson(directory[i]);
    }
    printf("Total %d persons.\n", n);
};

void removeOne(char* name) {
    int index = search(name); // return -1 if not exists
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }
    Person *p = directory[index];
    for (int i = index; i < n-1; i++) {
        directory[i] = directory[i+1]; // 구조체 자체 치환이 가능하다!
    }
    n--;
    realsePerson(p); // p에 맵핑된 구조체 메모리 반환
    printf("'%s' was deleted successfully. \n", name);
};

void saveAll (char* fileName) {
    FILE *fp = fopen(fileName, "w"); // 없으면 만들고 있으면 덮어쓴다!
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        printf("%s %s %s %s\n",directory[i]->name, directory[i]->number, directory[i]->email, directory[i]->group);
        fprintf(fp, "%s#", ( directory[i]->name != NULL ) ? directory[i]->name : " ");
        fprintf(fp, "%s#", ( directory[i]->number != NULL ) ? directory[i]->number : " ");
        fprintf(fp, "%s#", ( directory[i]->email != NULL ) ? directory[i]->email : " ");
        fprintf(fp, "%s#\n", ( directory[i]->group != NULL ) ? directory[i]->group : " ");
    }
    fclose(fp);
};

// cf. 저장한 파일은 /Users/jeongseonmin/Library/Developer/Xcode/DerivedData/프로젝트이름/Build/Products/Debug ~ 에 프로덕트와 함께 위치한다!
// 정확히는 /Users/jeongseonmin/Library/Developer/Xcode/DerivedData/C-DataStructure-ciqmpnskopxfeueltlrmriytwrqv/Build/Products/Debug
