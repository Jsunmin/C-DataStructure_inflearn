//
//  postFixExp.c
//  C-DataStructure
//
//  Created by 정선민 on 2020/12/01.
//

// 스택 예시
#include "postFixExp.h"
#include "stack.h"
#include <string.h>
#include <stdlib.h>

static char OPERATORS[] = "+-*/";

// 스택관련
Stack operand_stack_post;
Stack createPost ( void ); // 스택 생성
void pushPost ( Stack s, int item ); // 스택 푸시
int popPost ( Stack s ); // 스택 팝
int is_emptyPost ( Stack s );

void handle_exception_post (char *str) {
    printf("%s\n", str);
    exit(1);
}

int is_operator(char ch) { // 연산자인지 검사
    for (int i = 0; i < strlen(OPERATORS); i++) {
        if (OPERATORS[i] == ch) {
            return i;
        }
    }
    return -1;
}

int eval_op(char op) {
    if(is_emptyPost(operand_stack_post)) {
        handle_exception_post("Syntax Error: Stack empty in eval_op.");
    }
    int rhs = popPost(operand_stack_post);
    if(is_emptyPost(operand_stack_post)) {
        handle_exception_post("Syntax Error: Stack empty in eval_op.");
    }
    int lhs = popPost(operand_stack_post);
    int result = 0;
    switch (op) {
        case '+': result = lhs + rhs; break;
        case '-': result = lhs - rhs; break;
        case '*': result = lhs * rhs; break;
        case '/': result = lhs / rhs; break;
    }
    return result;
}

int eval(char *expr) { // 후위표기식 인자
    operand_stack_post = createPost(); // 스택 생성
    char *token = strtok(expr, " "); // 모든 연산자와 피연산자가 공백문자로 구분되어 있다고 가정
    while( token != NULL ) {
        if (token[0] >= '0' && token[0] <= '9') {
            int value = atoi(token);
            pushPost(operand_stack_post, value); // 스택에 숫자 넣음
        } else if (is_operator(token[0]) > -1) {
            int result = eval_op(token[0]);
            pushPost(operand_stack_post, result); // 스택에서 숫자를 빼서 계산
        } else {
            handle_exception_post("Syntax Error: invalid character encountered");
        }
        token = strtok(NULL, " ");
    }
    if(is_emptyPost(operand_stack_post)) { // 정답 하나만 존재해야 함
        handle_exception_post("Syntax Error: stack empty in eval_op.");
    }
    int answer = popPost(operand_stack_post);
    if (is_emptyPost(operand_stack_post)) {
        return answer;
    } else {
        handle_exception_post("Syntax Error: stack should be empty.");
        return -1;
    }
}

