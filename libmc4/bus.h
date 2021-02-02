#ifndef __BUS_H__
#define __BUS_H__

#include<stdint.h>

typedef void (*MC4_busReadIntercept)(uint16_t address, uint8_t* data);
typedef void (*MC4_busWriteIntercept)(uint16_t address, uint8_t data);

typedef struct Bus {
    MC4_busReadIntercept readIntercept;
    MC4_busWriteIntercept writeIntercept;
} Bus;

extern void MC4_initBus(Bus* bus);
extern void MC4_setBusReadIntercept(Bus* bus, MC4_busReadIntercept intercept);
extern void MC4_setBusWriteIntercept(Bus* bus, MC4_busWriteIntercept intercept);
extern void MC4_readBus(Bus* bus, uint16_t address, uint8_t* data);
extern void MC4_writeBus(Bus* bus, uint16_t address, uint8_t data);

#endif