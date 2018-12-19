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
