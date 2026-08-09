#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

/* Initialize LCD */
void LCD_voidInit(void);

/* Send a Command to LCD */
void LCD_voidSendCOM(u8 Copy_u8Instruction);

/* Write a Character */
void LCD_voidWriteCHAR(u8 Copy_u8DATA);

/* Write a String "ARRAY OF CHARs" */
void LCD_voidWriteSTRING(u8* Copy_u8String);

/* Go To a Specific Location (ROW 0-1, COL 0-15) */
void LCD_voidGoToXY(u8 Copy_u8ROW, u8 Copy_u8COL);

/* Clear Your Display */
void LCD_voidClearDisplay(void);

/* Create Special Character and display it */
void LCD_voidCreateCustomChar(u8* Copy_u8Pattern, u8 Copy_u8Location);

#endif /* LCD_INTERFACE_H_ */
