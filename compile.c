#ifndef GET_SOURCE
#define GET_SOURCE
#include "getSource.c"
#endif

// #ifndef TBL
// #define TBL
#include "table.c"
// #endif

#ifndef CODE_GEN
#define CODE_GEN
#include "codegen.c"
#endif

#define MINEERROR 3
#define FIRSTADDR 2  /* 各ブロックの最初の変数のアドレス */

static Token token;

static void block(int pIndex);    /* ブロックのコンパイル */
// static void constDecl();
// static void varDecl();
// static void funcDecl();
// static void statement();
// static void expression();
// static void term();
// static void factor();
// static void condition();
// static isStBeginKey(Token t);

int compile() {
  int i = 0;
  printf("... Start compilation ...\n");

  initSource();

  /* compile */
  token = nextToken();
  blockBegin(FIRSTADDR);

  block(0);

  finalSource();

  i = errorN();
  if (i!=0)
    printf("%d errors\n", i);

  return i<MINEERROR;
}

void block(int pIndex) { /* pIndexはこのブロックの関数のindex */
  int backP;
  backP = genCodeV(jmp, 0);
}
