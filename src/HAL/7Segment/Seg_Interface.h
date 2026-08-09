#ifndef SEG_INTERFACE_H
#define SEG_INTERFACE_H

/*SEGMENT TYPES*/
#define COMMON_ANODE 0
#define COMMON_CATHODE 1

ErrorCode Seg_enumInit(u8 Copy_u8PortID);
ErrorCode Seg_enumDisplay(u8 Copy_u8PortID, u8 Copy_u8SegValue, u8 Copy_u8SegType);

#endif /* SEG_INTERFACE_H */