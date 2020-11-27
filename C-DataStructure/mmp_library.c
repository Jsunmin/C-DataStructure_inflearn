//
//  mmp_library.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/26.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mmp_library.h"
#include "mmp_stringTool.h"

#define NUM_CHARS 256
#define BUFFER_LENGTH 200
int num_index = 0;

Artist *artist_directory[NUM_CHARS]; // 모든 UTF-8 값으로 구성할 수 있다! (2^8 = 256)

void initialize () {
    for( int i = 0; i < NUM_CHARS; i++) {
        artist_directory[i] = NULL;
    }
}

void mmpLoad (FILE *fp) {
    char buffer[BUFFER_LENGTH];
    char *name, *title, *path;
    while(1) {
        printf("$ ");
        if ( read_line(fp, buffer, BUFFER_LENGTH) <= 0 ) {
            break;
        }
        name = strtok(buffer, "#");
        if (strcmp(name, " ") == 0) {
            name = NULL;
        } else {
            name = strdup(name);
        }
        title = strtok(NULL, "#");
        if (strcmp(title, " ") == 0) {
            title = NULL;
        } else {
            title = strdup(title);
        }
        path = strtok(NULL, "#");
        if (strcmp(path, " ") == 0) {
            path = NULL;
        } else {
            path = strdup(path);
        }
        printf("%s %s %s\n", name, title, path);
    }
}

// malloc은 하나의 모듈로 관리하는게 좋다! (메모리 터칭 함수는)
Artist *create_artist_instance(char *name) {
    Artist *ptr_artist = (Artist *)malloc(sizeof(Artist));
    ptr_artist->name = name;
    ptr_artist->head = NULL;
    ptr_artist->tail = NULL;
    ptr_artist->next = NULL;
    return ptr_artist;
}
Song *create_song_instance(Artist *ptr_artist, char *title, char *path) {
    Song *ptr_song = (Song *)malloc(sizeof(Song));
    ptr_song->artist = ptr_artist;
    ptr_song->title = title;
    ptr_song->path = path;
    ptr_song->index = num_index;
    num_index++;
    return ptr_song;
}
void insert_node(Artist *ptr_artist, SNode *ptr_snode) {
    SNode *p = ptr_artist->head;
    // head -> 넣으려는 ptr_snode의 title을 비교하면서 직후 노드 위치(p)를 설정한다. (while 이니까, false 떨어지면, title이 더 뒤라는 것!)
    while(p != NULL && strcmp( p->song->title, ptr_snode->song->title ) < 0 ) {
        p = p->next;
    }
    
    // add ptr_snode before p
    // 1. empty 2. at the front 3. at the end 4. in the middle
    if (ptr_artist->head == NULL) { // 1 이미 artist 위치 파악되어 있다. 바로 쓴다! ( p == NULL은 끝까지 다 돌았을 때에도.. )
        // 빈 노드 작업
        ptr_artist->head = ptr_snode;
        ptr_artist->tail = ptr_snode;
    } else if (p == ptr_artist->head) { // 2 p가 while을 돌지 못하고, 바로 멈춤!
        ptr_snode->next = ptr_artist->head;
        ptr_artist->head->prev = ptr_snode; // 기존 head의 prev를 생성한 노드로 설정
        ptr_artist->head = ptr_snode; // head node 변경
    } else if (p == NULL) { // 3
        ptr_snode->prev = ptr_artist->tail;
        ptr_artist->tail->next = ptr_snode;
        ptr_artist->tail = ptr_snode;
    } else { // 4 - p 앞에 삽입
        ptr_snode->next = p;
        ptr_snode->prev = p->prev;
        p->prev->next = ptr_snode;
        p->prev = ptr_snode;
    }
}
void print_song(Song *ptr_song) {
    printf("    %d: %s, %s\n", ptr_song->index, ptr_song->title, ptr_song->path);
}
void print_artist(Artist *p) {
    printf("%s\n", p->name);
    SNode *ptr_snode = p->head;
    while (ptr_snode != NULL) {
        print_song(ptr_snode->song);
        ptr_snode = ptr_snode->next;
    }
}

Artist* find_artist(char *name) {
    // UTF-8로 정리한 배열에서 이름 색인 접근!
    // unsigned char 형변환 - 128을 넘어가는 char의 경우 (ex 한글), 음수로 치환될 수 있음(맨앞 1이면 부호로).. 그럼 에러..
    Artist *p = artist_directory[(unsigned char) name[0] ]; // first artist with initial name[0]
    
    // artist 연결리스트는 name 사전식 정렬 -> 내가 찾는 이름이 사전식으로 뒤에 위치할때만 반복문을 돈다. ( [0].name != name && 찾는 이름이 사전식으로 앞이면, 아예 없다는 뜻 )
    while(p != NULL && strcmp(p->name, name) < 0) {
        p = p->next;
    }
    if (p != NULL && strcmp(p->name, name) == 0) {
        return p;
    } else {
        return NULL;
    }
}

Artist* add_artist (char *name) {
    Artist *ptr_artist = create_artist_instance(name);
    // 생성한 artist의 이름 인덱스 head node
    Artist *p = artist_directory[(unsigned char)name[0]];
    // p의 한칸 전 노드
    Artist *q = NULL;
    while(p != NULL && strcmp(p->name, name) < 0) { // 정렬에 따른 위치 파악
        q = p;
        p = p->next;
    }
    
    if (p == NULL && q == NULL) { // 리스트의 신규 node
        artist_directory[(unsigned char) name[0] ] = ptr_artist;
    } else if (q == NULL) { // 리스트의 first node로 추가
        ptr_artist->next = artist_directory[(unsigned char)name[0]]; // == p; (다음 노드 링크)
        artist_directory[(unsigned char)name[0]] = ptr_artist; // 헤드노드 변경
    } else { // 리스트의 후속으로 node 추가
        ptr_artist->next = p;
        q->next = ptr_artist;
    }
    return ptr_artist;
};
void add_song(char *artist, char *title, char *path) {
    // artist 존재 여부 파악 ~ return NULL if not
    Artist *ptr_artist = find_artist(artist);
    if (ptr_artist == NULL) {
        ptr_artist = add_artist(artist);
    }
    // song add
    Song *ptr_song = create_song_instance(ptr_artist, title, path);
    // snode add
    SNode *ptr_snode = (SNode *)malloc(sizeof(SNode));
    ptr_snode->song = ptr_song;
    ptr_snode->next = NULL;
    ptr_snode->prev = NULL;
    
    // insert node
    insert_node(ptr_artist, ptr_snode);
}

void handle_status() {
    for (int i = 0; i < NUM_CHARS; i++) {
        Artist *p = artist_directory[i];
        while( p != NULL ) {
            print_artist(p);
            p = p->next;
        }
    }
}
