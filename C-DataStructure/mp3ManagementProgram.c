//
//  mp3ManagementProgram.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/26.
//

// 이중 링크드리스트 실습
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmp_stringTool.h" // 모듈 임포트
#include "mmp_library.h"

#define BUFFER_LENGTH 200
/*
 song 구조체를 참조하는 가수 이중 연결리스트
 이렇게 구분하면, 노래를 연결리스트 별로 (가수별/앨범별) 등의 분류가 가능함.
 구조체 내부에 묶어놓으면, 특정 기준별 묶음을 구성할 때 너무 힘들다..
 -> 구조체는 독립되게 존재 & 이를 참조하는 연결리스트는 많다!
*/

/*
 가수들은 artist_dir에 문자별로 그룹핑한 링크드리스트 포인터를 둠 (이름별 정렬)
  ex dir[a] = *A_artist_group_linkedlist
 
 index_dir에서 아이디별로 그룹핑한 링크드리스트 포인터를 둠 (인덱스 오름차순 정렬)
  사이즈로 나눈 나머지로, 0 ~ size -1까지 아이디를 구분함
*/

void process_command ( void );
void handle_load ( void );
void handle_add(void);
void handle_search ( void );
void handle_remove ( void );
void handle_play ( void );
void handle_save ( void );


void mp3ManagementProgram() {
    initialize();
    handle_load();
    process_command();
}

void handle_load () {
    char buffer[BUFFER_LENGTH];
    
    printf("Data file name ? ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) <= 0) {
        return;
    }
    FILE *fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("No such file exists.\n");
        return;
    }
    mmpLoad(fp);
    fclose(fp);
};

void process_command() {
    char command_line[BUFFER_LENGTH];
    char *command;
    while(1) {
        printf("$ ");
        if ( read_line(stdin, command_line, BUFFER_LENGTH) <= 0 ) {
            continue;
        }
        command = strtok(command_line, " ");
        if (strcmp(command, "add") == 0) {
            handle_add();
        } else if (strcmp(command, "search") == 0) {
            handle_search();
        } else if (strcmp(command, "remove") == 0) {
            handle_remove();
        } else if (strcmp(command, "status") == 0) {
            handle_status();
        } else if (strcmp(command, "play") == 0) {
            handle_play();
        } else if (strcmp(command, "save") == 0) {
            char *tmp = strtok(NULL, " ");
            if (strcmp( tmp, "as") != 0) {
                continue;
            }
            handle_save();
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }
}

void handle_add() {
    char buffer[BUFFER_LENGTH];
    char *artist = NULL, *title = NULL, *path = NULL;
    printf("    Artist: ");
    if ( read_line(stdin, buffer, BUFFER_LENGTH) > 0 ) {
        artist = strdup(buffer);
    };
    printf("    Title: ");
    if ( read_line(stdin, buffer, BUFFER_LENGTH) > 0 ) {
        title = strdup(buffer);
    };
    printf("    Path: ");
    if ( read_line(stdin, buffer, BUFFER_LENGTH) > 0 ) {
        path = strdup(buffer);
    };
    
    // add music lib
    add_song(artist, title, path);
}

void handle_search() {
    char name[BUFFER_LENGTH], title[BUFFER_LENGTH];
    printf("    Artist: ");
    if ( read_line(stdin, name, BUFFER_LENGTH) <= 0 ) {
        printf("    Artist name required.\n");
        return;
    };
    printf("    Title: ");
    int title_len = read_line(stdin, title, BUFFER_LENGTH);
    if (title_len <= 0) {
        search_song_artist(name);
    } else {
        search_song_artist_title(name, title);
    };
}

void handle_remove() {
    char *id_str = strtok(NULL, " ");
    int index = atoi(id_str);
    mmpRemove(index);
}

void handle_play() {
    char *id_str = strtok(NULL, " ");
    int index = atoi(id_str); // ASCII to integer 내장함수
    play(index);
}

void handle_save() {
    char *file_name = strtok(NULL, " ");
    FILE *fp = fopen(file_name, "w");
    save(fp);
    fclose(fp);
}
