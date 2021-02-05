#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#define INS_NOOP    (0x00)
#define INS_HALT    (0xF0)
#define INS_LDA_LIT (0x01)
#define INS_LDA_PTR (0x02)
#define INS_LDB_LIT (0x11)
#define INS_LDB_PTR (0x12)
#define INS_JMP_LIT (0xB0)
#define INS_JMP_PTR (0xB1)
#define INS_JMP_REG (0xB2)
#define INS_JZS_LIT (0xB3)
#define INS_JZS_PTR (0xB4)
#define INS_JZS_REG (0xB5)
#define INS_JNZ_LIT (0xB6)
#define INS_JNZ_PTR (0xB7)
#define INS_JNZ_REG (0xB8)
#define INS_INA     (0xA0)
#define INS_INB     (0xA1)
#define INS_DEA     (0xA2)
#define INS_DEB     (0xA3)

#endif