//
//  mmp_library.h
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/26.
//

#ifndef mmp_library_h
#define mmp_library_h

typedef struct song Song;
typedef struct snode SNode;
typedef struct artist Artist;

struct song {
    Artist *artist;
    char *title;
    char *path;
    int index;
};

struct snode {
    SNode *next, *prev;
    Song *song;
};

struct artist {
    char *name;
    Artist *next; // 가수 연결리스트
    SNode *head, *tail; // 해당 가수의 song 양방향 연결리스트
};
void initialize (void);
void mmpLoad (FILE *fp);
void add_song (char *artist, char *title, char *path);
void handle_status (void);
void search_song_artist_title(char *artist, char *title);
void search_song_artist(char *artist);
void mmpRemove (int index);
void play (int index);
void save (FILE *fp);
#endif /* mmp_library_h */
