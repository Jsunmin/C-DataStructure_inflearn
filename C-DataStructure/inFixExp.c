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

static char OPERATORS_IN[] = "+-*/";
static int PRECEDENCE_IN[] = {1,1, 2,2}; // 사칙연산 우선순위 게산용

// 스택관련
Stack operand_stack_in;
Stack create_in ( void ); // 스택 생성
void push_in ( Stack s, int item ); // 스택 푸시
int pop_in ( Stack s ); // 스택 팝
int is_empty_in ( Stack s );
int peek_in ( Stack s ); // 스택 팝


void handle_exception (char *str) {
    printf("%s\n", str);
    exit(1);
}
int is_operator_in(char ch) { // 연산자인지 검사
    for (int i = 0; i < strlen(OPERATORS_IN); i++) {
        if (OPERATORS_IN[i] == ch) {
            return i;
        }
    }
    return -1;
}
int precedence(char op) {
    return PRECEDENCE_IN[is_operator_in(op)]; // 연산자 인덱스에 맞춰 우선순위 리턴
}

char *process_op_in(char op, char *pos) {
    if (is_empty_in(operand_stack_in)) {
        push_in(operand_stack_in, op);
    } else {
        char top_op = peek_in(operand_stack_in);
        if (precedence(op) > precedence(top_op)) {
            push_in(operand_stack_in, op); // 현재 연산자가 stack top보다 크면, 바로 넣음
        } else {
            while (!is_empty_in(operand_stack_in) && precedence(op) <= precedence(top_op)) {
                pop_in(operand_stack_in); // top_op는 위에 정의되어있어서, 저장하지 말고 바로 뺀다! ( & 맨 밑에 top_op 재갱신함 )
                sprintf(pos, "%c ", top_op);
                pos += 2; // 연산자와 공백 더해줌
                if (!is_empty_in(operand_stack_in)) {
                    top_op = (char)peek_in(operand_stack_in);
                }
            }
            push_in(operand_stack_in, op);
        }
    }
    return pos;
}

char *convert(char *infix) { // postfix 값을 리턴하는 함수
    operand_stack_in = create_in();
    
    char *postfix = (char *)malloc(strlen(infix) + 1); // infix -> postfix되어 저장될 곳
    char *pos = postfix;
    
    char *token = strtok(infix, " ");
    while( token != NULL) {
        if (token[0] >= '0' && token[0] <= '9') {
            sprintf(pos, "%s ", token); // 문자열에 append하는 함수 ('\0' 없이) ~ 바로 붙여가며 만들기 좋음!
            pos += (strlen(token) + 1); // 숫자는 그대로 넣는다.
        } else if (is_operator_in(token[0]) > -1) {
            pos = process_op_in(token[0], pos); // 연산자를 append한 후 문자열 끝 주소를 반환 (연산자 추가한 것 감안해서! )
        } else {
            handle_exception("Syntax Error: invalid character encountered");
        }
        token = strtok(NULL, " ");
    }
    while( !is_empty_in(operand_stack_in) ) {
        char op = (char)pop_in(operand_stack_in);
        sprintf(pos, "%c ", op);
        pos += 2;
    }
    *pos = '\0';
    return postfix;
    
}

int eval_op(char op) {
    if(is_empty_in(operand_stack_in)) {
        handle_exception("Syntax Error: Stack empty in eval_op.");
    }
    int rhs = pop_in(operand_stack_in);
    if(is_empty_in(operand_stack_in)) {
        handle_exception("Syntax Error: Stack empty in eval_op.");
    }
    int lhs = pop_in(operand_stack_in);
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
    operand_stack_in = create_in(); // 스택 생성
    char *token = strtok(expr, " "); // 모든 연산자와 피연산자가 공백문자로 구분되어 있다고 가정
    while( token != NULL ) {
        if (token[0] >= '0' && token[0] <= '9') {
            int value = atoi(token);
            push_in(operand_stack_in, value); // 스택에 숫자 넣음
        } else if (is_operator_in(token[0]) > -1) {
            int result = eval_op(token[0]);
            push_in(operand_stack_in, result); // 스택에서 숫자를 빼서 계산
        } else {
            handle_exception("Syntax Error: invalid character encountered");
        }
        token = strtok(NULL, " ");
    }
    if(is_empty_in(operand_stack_in)) { // 정답 하나만 존재해야 함
        handle_exception("Syntax Error: stack empty in eval_op.");
    }
    int answer = pop_in(operand_stack_in);
    if (is_empty_in(operand_stack_in)) {
        return answer;
    } else {
        handle_exception("Syntax Error: stack should be empty.");
        return -1;
    }
}


