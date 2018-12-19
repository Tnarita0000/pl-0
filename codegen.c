#include <stdio.h>
#include "codegen.h"

#ifndef TBL
#define TBL
#include "table.h"
#endif

#ifndef GET_SOURCE
#include "getSource.c"
#endif

#define MAXCODE  200   /* 目的コードの最大長 */
#define MAXMEM   2000  /* 実行時のスタックの最大長 */
#define MAXREG   20    /* 演算レジスタのスタックの最大長 */
#define MAXLEVEL 5     /* ブロックの最大のネスト数 */

typedef struct inst {
  OpCode opCode;
  union {
    RelAddr addr;
    int value;
    Operation optr;
  } u;
} Inst;

static Inst code[MAXCODE];      /* 目的コードが入る */
static int cIndex = -1;
static void checkMax();

int genCodeV(OpCode op, int v) {
  checkMax();
  code[cIndex].opCode = op;
  code[cIndex].u.value = v;

  return cIndex;
}

void checkMax() {
  if (++cIndex < MAXCODE) {
    return;
  }

  /* TODO: throw error */
}

void execute() { /* 目的コードの実行 */
  int stack[MAXMEM];          /* 実行時スタック */
  int display[MAXLEVEL];      /* 現在見える各ブロックの先頭番地のディスプレイ */

  int pc, top, lev, temp;
  Inst i;                     /* 実行する命令語 */
  printf("start execution\n");
  top = 0; pc = 0;              /* top: 次にスタックに入れる場所, pc: 命令語のカウンタ */
  stack[0] = 0; stack[1] = 0; /* stack[0]はcalleeで壊すディスプレイの退避場所, stack[top+1]はcallerへの戻り番地 */
  display[0] = 0;             /* 主ブロックの先頭番地は0 */

  do {
    i = code[pc++];           /* これから実行する命令語 */
    switch(i.opCode) {
      case lit:
        stack[top++] = i.u.value;
        break;
      case lod:
        stack[top++] = stack[display[i.u.addr.level] + i.u.addr.addr];
        break;
      case sto:
        stack[display[i.u.addr.level] + i.u.addr.addr] = stack[--top];
        break;
      case cal:
        lev = i.u.addr.level + 1;    /* i.u.addr.levelはcalleeの名前のレベル */
        stack[top] = display[lev];   /* diplay[lev]の退避 */
        stack[top+1] = pc;
        display[lev] = top;
        pc = i.u.addr.addr;
        break;
      case ret:
        temp = stack[--top];                  /* スタックのトップにあるものが返す値 */
        top = display[i.u.addr.level];        /* topを呼ばれた時の値に戻す */
        display[i.u.addr.level] = stack[top]; /* 壊したディスプレイの回復 */
        pc = stack[top+1];
        top -= i.u.addr.addr;                 /* 実引数の分だけトップを返す */
        stack[top++] = temp;                  /* 返す値をスタックのトップへ */
        break;
      case ict:
        top += i.u.value;
        if (top >= MAXMEM - MAXREG) {
          // TODO: errorF("stack overflow");
        }
        break;
      case jmp:
        pc = i.u.value;
        break;
      case jpc:
        if (stack[--top] == 0) {
          pc = i.u.value;
        }
        break;
      case opr:
        switch(i.u.optr) {
          case neg:
            stack[top-1] = -stack[top-1];
            continue;
          case add:
            --top;
            stack[top-1] += stack[top];
            continue;
          case sub:
            --top;
            stack[top-1] -= stack[top];
            continue;
          case mul:
            --top;
            stack[top-1] *= stack[top];
            continue;
          case divi:
            --top;
            stack[top-1] /= stack[top];
            continue;
          case odd:
            stack[top-1] = stack[top-1] & 1;
            continue;
          case eq:
            --top;
            stack[top-1] = (stack[top - 1] == stack[top]);
            continue;
          case ls:
            --top;
            stack[top-1] = (stack[top-1] < stack[top]);
            continue;
          case gr:
            --top;
            stack[top-1] = (stack[top-1] > stack[top]);
            continue;
          case neq:
            --top;
            stack[top-1] = (stack[top-1] != stack[top]);
            continue;
          case lseq:
            --top;
            stack[top-1] = (stack[top-1] <= stack[top]);
            continue;
          case greq:
            --top;
            stack[top-1] = (stack[top-1] >= stack[top]);
            continue;
          case wrt:
            printf("%d", stack[--top]);
            continue;
          case wrl:
            printf("\n");
            continue;
        }
    }
  } while (pc !=0);

}
