//
//  maze.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/12/02.
//

#include <stdio.h>
#include "stack.h"
#include "queue.h"

#define MAX 100
#define PATH 0 // 지나갈 수 있는 위치
#define WALL 1 // 지나갈 수 없는 위치
#define VISITED 2 // 이미 방문한 위치
#define BACKTRACKED 3 // 방문했다가 나온 위치

typedef struct pos {
    int x, y;
} Position;

// DFS / BFS 각각 스택과 큐로 풀어보자!!

Position move_to(Position pos, int dir);

int maze[MAX][MAX];
int n // 미로 크기

void read_maze();
void print_maze();
bool movalble(POS pos, int dir);

int offset[4][2] = {
    {-1, 0}, // N
    {0, 1}, // E
    {1, 0}, // S
    {0, -1}, // W
}
Position move_to(Position pos, int dir) {
    Position next;
    next.x = pos.x + offset[dir][0];
    next.y = pos.y + offset[dir][1];
    return next;
}

void mazeProg() {
    read_maze();
    
    Stack s = create(); // 현 위치 저장할 스택
    Position cur;
    cur.x = 0;
    cur.y = 0;
    
    while(1) {
        maze[cur.x][cur.y] = VISITED; // 현위치 방문 표시
        if(cur.x == n-1 && cur.y == n-1) {
            printf("Found the path.\n"); // 현위치가 출구일 때
            break;
        }
        bool forwarded = false; // 4방향 중 한 곳으로 전진하는데 성공 표시
        for (int dir = 0; dir < 4; dir++) { // 순서대로 NESW
            if (movable(cur, dir)) { // dir로 이동할 수 있는지 검사
                push(s, cur); // 현재위치를 스택에 저장
                cur = move_to(cur, dir); // dir 방향으로 한칸 이동한 위치로 cur 갱신
                forwarded = ture;
                break;
            }
        }
        // 4방향중 어느 곳도 못 갔을 때
        if ( !forwarded ) {
            maze[cur.x][cur.y] = BACKTRACKED; // 왔다 돌아간 위치 표시
            if ( is_empty(s) ) { // 되돌아갈 위치 없다면 원래 길이 없는 미로
                printf("No path exists.\n");
                break;
            }
            cur = pop(s); // 스택에서 pop한 위치가 (왔던데) 현위치가 됨 - 롤백
        }
    }
    print_maze();
}


// 큐를 통한 넓이 우선 탐색
// 방문 표시를 음수 표시( -1 ... -n )로 둬서, 0, 1 ~ 벽과 길을 구분함.

void mazeProgQ() {
    read_maze();

    Queue queue = create_queue();
    Position cur;
    cur.x = 0;
    cur.y = 0;

    enqueue(queue, cur); // 현재 위치를 넣는다.
    maze[0][0] = -1; // -1부터 점점 -n으로.. 최단 거리를 찾는다.
    bool found = false;
    
    while(!is_empty_q(queue)) { // q가 비었으면 종료 ~ 미로 길이 없다?!
        Position cur = dequeue(queue);
        for (int dir = 0; dir < 4; dir++) {
            if (movalble(cur, dir)) {
                Position p = move_to(cur, dir);
                maze[p.x][p.y] = maze[cur.x][cur.y] - 1; // 미로에서 이동할 위치는, 그전 위치 - 1 처리!
                if (p.x == n-1 && p.y == n-1) { // 마지막 지점에 도착했으면 종료
                    printf("Found the path.\n");
                    found = true;
                    break;
                }
                enqueue(queue, p);
            }
        }
    }
}
