/* マシン命令  */
typedef enum codes {
  lit,
  opr,
  lod,
  sto,
  cal,
  ret,
  ict,
  jmp,
  jpc
} OpCode;

typedef enum ops {
  add,
  sub,
  mul,
  divi,
  odd,
  eq,
  ls,
  gr,
  neq,
  lseq,
  greq,
  wrt,
  wrl,
} Operation;

int genCodeV(OpCode op, int v);  /* マシン語の生成. アドレス部にv */
int genCodeT(OpCode op, int ti); /* マシン語の生成. アドレスは名前表から */
int genCodeO(Operation p);       /* マシン語の生成. アドレス部に演算命令 */
int genCodeR();                  /* ret命令語の生成 */
