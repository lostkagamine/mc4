#include<stdio.h>
#include<stdlib.h>
#include "bus.h"
#include<stdint.h>

void MC4_initBus(Bus* bus) {
    bus->readIntercept = NULL;
    bus->writeIntercept = NULL;
}

void MC4_setBusReadIntercept(Bus* bus, MC4_busReadIntercept intercept) {
    bus->readIntercept = intercept;
}

void MC4_setBusWriteIntercept(Bus* bus, MC4_busWriteIntercept intercept) {
    bus->writeIntercept = intercept;
}

void MC4_readBus(Bus* bus, uint16_t address, uint8_t* data) {
    if (bus->readIntercept == NULL) {
        fprintf(stderr, "Warning: no bus read intercept defined! Bus reads will return 00!\n");
        *data = 0x00;
    }
    else {
        bus->readIntercept(address, data);
    }
}

void MC4_writeBus(Bus* bus, uint16_t address, uint8_t data) {
    if (bus->writeIntercept == NULL) {
        fprintf(stderr, "Warning: no bus write intercept defined! Bus writes will do nothing!\n");
    }
    else {
        bus->writeIntercept(address, data);
    }
}