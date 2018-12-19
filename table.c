#ifndef TBL
#define TBL
#include "table.h"
#endif

#ifndef GET_SOURCE_H
#define GET_SOURCE_H
#include "getSource.h"
#endif

#define MAXTABLE 100      /* 名前表の最大長 */
#define MAXNAME   31      /* 名前の最大長 */
#define MAXLEVEL  5       /* ブロックの最大入れ子数 */

static int tIndex = 0;               /* 名前表のindex */
static int nestI[MAXLEVEL];          /* nestI[i]にはネスト数iの最後のindex */
static int addr[MAXLEVEL];           /* addr[i]にはブロックレベルiの最後の変数の番地 */
static int level = -1;               /* 現在のネスト階層数 */
static int localAddr;                /* 現在のネスト数 */

void blockBegin(int firstAddr) { /* ブロックの始まりで呼ばれる */
  if (level == -1) {
    localAddr = firstAddr;
    tIndex = 0;
    level++;
    return;
  }

  if (level == MAXLEVEL-1) {
    // TODO
    /*errorF("too many nested blocks");*/
  }

  nestI[level] = tIndex;     /* 今までのブロックの情報を格納 */
  addr[level] = localAddr;
  localAddr = firstAddr;     /* 新しいブロックの最初の変数の番地 */
  level++;                   /* 新しいブロックのレベル */
  return;
}
