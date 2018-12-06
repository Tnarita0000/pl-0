typedef enum kindT {               /* Indetifierの種類 */
  varId, funcId, parId, constId
} KindT;

typedef struct relAddr {           /* 変数, 引数, 関数のアドレスの型 */
  int level;
  int addr;
} RelAddr;
