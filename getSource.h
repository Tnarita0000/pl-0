#include <stdio.h>
#ifndef TBL
#define TBL
#include "table.h"
#endif

#define MAXNAME 31

typedef enum keys {
  Begin, End,       /* 予約後 */
  If, Then,
  While, Do,
  Ret, Func,
  Var, Const, Odd,
  Write, WriteLn,
  end_of_KeyWd,

  Plus, Minus,      /* 演算記号, 区切り記号 */
  Mult, Div,
  Lparen, Rparen,
  Equal, Lss, Gtr,
  NotEq, LssEq, GtrEq,
  Comma, Period, Semicolon,
  Assign,
  end_of_KeySym,

  Id, Num, nul,     /* トークンの種類 */
  end_of_Token,

  letter, digit, colon, others, /* その他の文字の種類 */
} KeyId;


typedef struct token {     /* トークンの型 */
  KeyId kind;              /* トークンの種類かkeyの名前 */
  union {
    char id[MAXNAME];      /* Identifierの時, その名前 */
    int value;
  } u;
} Token;
// /* int hoge = 1;
//  *     id     value
//  */
// 
// Token nextToken();        /* 次のトークンを読んで返す */
// Token checkGet(Token t, KeyId k);  /* t.kind == kのチェック */

int openSource(char fileName[]);   /* ソースファイルを開く処理 */
// void closeSource();
void initSource();                 /* テーブルの初期設定 */
// void finalSource();                /* ソースの最後のチェック, texファイルん初期設定 */
// 
// void errorType(char *m);           /* 型エラーを.textファイルに出力 */
// void errorInsert(KeyId k);         /* keyString(k)を.texファイルに挿入 */
// void errorMissingId();             /* 名前がないとのメッセージを.texファイルに挿入 */
// void errorMissingOp();             /* 演算子がないとのメッセージを.texファイルに挿入 */
// void errorDelete();                /* 今読んだトークンを読み捨て(.texファイルに挿入) */
// void errorMessage(char *m);        /* エラーメッセージを.texファイルに出力 */
// void errorF(char *m);              /* エラーメッセージを出力し, コンパイル狩猟 */
// int errorN();                      /* エラーの個数を返す */

// void setIdKind(KindT k);           /* 現トークン(Id)の種類をセット(.texファイル出力のため) */
