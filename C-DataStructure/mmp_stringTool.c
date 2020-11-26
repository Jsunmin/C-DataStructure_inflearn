//
//  mmp_stringTool.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/11/26.
//

#include <stdio.h>
#include "mmp_stringTool.h" // 자기 자신의 헤더파일

int read_line(FILE *fp, char str[], int n) {
    int ch, i = 0;
    while((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (i < n - 1) { // NULL 고려
            str[i++] = ch;
        }
    }
    str[i] = '\0';
    return i;
}
