#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifndef GET_SOURCE_H
#define GET_SOURCE_H
#include "getSource.h"
#endif
#ifndef TBL
#define TBL
#include "table.h"
#endif

#define MAXLINE 120
#define MAXERROR 30
#define MAXNUM 14    /* 定数の最大桁数 */
#define TAB    5     /* タブのスペース数 */

static FILE *fpi;    /* source file */
static FILE *fptex;  /* LaTex出力 */
static char line[MAXLINE];/* 1行分のバッファ*/
static int lineIndex;     /* 次に読む文字の位置 */
static char ch;           /* 最後に読んだ文字 */

static Token cToken;      /* 最後に読んだトークン */
static KindT idKind;      /* 現トークン(id)の種類 */
static int spaces;        /* そのトークンの前のスペースの個数 */
static int CR;            /* その前の改行の個数 */
static int printed;       /* トークンは印字済みか */

static int errorNo = 0;

/* prototype declaration */
static char nextChar();
static isKeySym(KeyId k);     /* 記号かどうか */
static int isKeyWd(KeyId k);  /* 予約後かどうか */
static void printSpaces();    /* トークンの前のスペースの印字 */
static void printToken();     /* トークンの印字 */

struct keyWd {
  char *word;
  KeyId keyId;
};

static struct keyWd KeyWdT[] = {
  {"begin", Begin},
  {"end", End},
  {"if", If},
  {"then", Then},
  {"while", While},
  {"do", Do},
  {"return", Ret},
  {"function", Func},
  {"var", Var},
  {"const", Const},
  {"odd", Odd},
  {"write", Write},
  {"writeln", WriteLn},
  {"$dummy1", end_of_KeyWd},
  {"+", Plus},
  {"-", Minus},
  {"*", Mult},
  {"/", Div},
  {"(", Lparen},
  {")", Rparen},
  {"=", Equal},
  {"<", Lss},
  {">", Gtr},
  {"<>", NotEq},
  {"<=", LssEq},
  {">=", GtrEq},
  {",", Comma},
  {".", Period},
  {";", Semicolon},
  {":=", Assign},
  {"$dummy2", end_of_KeySym}
};

static openSourceDebug = 0;

char nextChar() {
  char ch;
  if (lineIndex == -1) {
    if (fgets(line, MAXLINE, fpi) != NULL) {
      // printf("%s", line);
      lineIndex = 0;
    } else {
      // printf("<EOF>\n");
      putchar('\n');
      exit(1);
    }
  }

  if ((ch = line[lineIndex++]) == '\n') {
    lineIndex = -1;
    return ' ';
  }
  return ch;
}

void outputSourceCode() {
  while (true) {
    printf("%c", nextChar());
  }
}

int openSource(char fileName[]) {
  char fileNameO[30];
  if ( (fpi = fopen(fileName, "r")) == NULL ) {
    printf("can't open %s\n", fileName);
    return 0;
  }

  strcpy(fileNameO, fileName);
  strcat(fileNameO, ".tex");

  if ( (fptex = fopen(fileNameO, "w") ) == NULL ) {
    printf("can't open %s\n", fileNameO);
    return 0;
  }

  if (openSourceDebug) {
    outputSourceCode();
  }

  return 1;
}

static KeyId charClassT[256];  /* 文字の種類を表にする */
static void initCharClassT() { /* 文字の種類を示す表を作る */
  int i;
  for (i=0; i<256; i++)
    charClassT[i] = others;
  
  // '0': 48, '9': 57
  for(i='0'; i<'9'; i++)
    charClassT[i] = digit;
  for (i='A'; i<='Z'; i++)
    charClassT[i] = letter;
  for (i='a'; i<='z'; i++)
    charClassT[i] = letter;

  charClassT['+'] = Plus;   charClassT['-'] = Minus;
  charClassT['*'] = Mult;   charClassT['/'] = Div;
  charClassT['('] = Lparen; charClassT[')'] = Rparen;
  charClassT['='] = Equal;  charClassT['<'] = Lss;
  charClassT['>'] = Gtr;    charClassT[','] = Comma;
  charClassT['.'] = Period; charClassT[';'] = Semicolon;
  charClassT[':'] = colon;
}

void initSource() {
  lineIndex = -1;
  ch = '\n';

  printed = 1;
  initCharClassT();

  fprintf(fptex, "\\documentstyle[12pt]{article}\n");
  fprintf(fptex, "\\begin{document}\n");
  fprintf(fptex, "\\fboxsep=0pt\n");
  fprintf(fptex, "\\def\\insert#1{$\\fbox{#1}$}\n");
  fprintf(fptex, "\\def\\delete#1{$\\fboxrule=.5mm\\fbox{#1}$}\n");
  fprintf(fptex, "\\rm\n");
}

static void printSpaces() {
  while (CR-- > 0)
    fprintf(fptex, "\\ \\par\n");
  while (spaces-- > 0)
    fprintf(fptex, "\\ ");
  CR = 0; spaces = 0;
}

void printcToken() {
  int i = (int)cToken.kind;
  if (printed) {
    printed = 0; return;
  }

  printed = 1;
  printSpaces();

  if (i < end_of_KeyWd) {
    fprintf(fptex, "{\\bf %s}", KeyWdT[i].word);
    fprintf(fptex, "{\\bf %s}", KeyWdT[i].word);
  } else if (i < end_of_KeySym) {
    fprintf(fptex, "$%s$", KeyWdT[i].word);
  } else if(i == (int)Id) {
    switch (idKind) {
      case varId:
        fprintf(fptex, "%s", cToken.u.id); return;
      case parId:
        fprintf(fptex, "{\\sl %s}", cToken.u.id); return;
      case funcId:
        fprintf(fptex, "{\\it %s}", cToken.u.id); return;
      case constId:
        fprintf(fptex, "{\\sf %s}", cToken.u.id); return;
    }
  } else if (i == (int)Num)
    fprintf(fptex, "%d", cToken.u.value); return;
}
