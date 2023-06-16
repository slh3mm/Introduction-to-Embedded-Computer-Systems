#include "msp.h"
#include <stdio.h>

// standard ASCII symbols
/**
 * \brief Carriage return character
 */
#define CR   0x0D
/**
 * \brief Line feed character
 */
#define LF   0x0A
/**
 * \brief Back space character
 */
#define BS   0x08
/**
 * \brief escape character
 */
#define ESC  0x1B
/**
 * \brief space character
 */
#define SP   0x20
/**
 * \brief delete character
 */
#define DEL  0x7F

/**
 * @details   Initialize EUSCI_A0 for UART operation
 * @details   115,200 baud rate (assuming 12 MHz SMCLK clock),
 * @details   8 bit word length, no parity bits, one stop bit
 * @param  none
 * @return none
 * @note   assumes 48 MHz bus and 12 MHz SMCLK
 * @brief  Initialize EUSCI A0
 */
void UART0_Init(void);


/**
 * @details   Initializes C standard library, enables printf to work
 * @details   Initialize EUSCI_A0 for UART operation
 * @details   115,200 baud rate (assuming 12 MHz SMCLK clock),
 * @details   8 bit word length, no parity bits, one stop bit
 * @param  none
 * @return none
 * @note   assumes 48 MHz bus and 12 MHz SMCLK
 * @note   This initialization calls UART0_Init
 * @brief  Initialize EUSCI A0 and printf
 */
void UART0_Initprintf(void);

/**
 * @details   Receive a character from EUSCI_A0 UART
 * @details   Busy-wait synchronization,
 * @details   blocking, wait for new serial port input
 * @param  none
 * @return ASCII code for key typed
 * @note   UART0_Init must be called once prior
 * @brief  Receive byte into MSP432
 */
char UART0_InChar(void);


/**
 * @details   Transmit a character to EUSCI_A0 UART
 * @details   Busy-wait synchronization,
 * @details   blocking, wait for UART to be ready
 * @param  letter is the ASCII code for key to send
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit byte out of MSP432
 */
void UART0_OutChar(char letter);


/**
 * @details   Transmit a string to EUSCI_A0 UART
 * @param  pt is pointer to null-terminated ASCII string to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit string out of MSP432
 */
void UART0_OutString(char *pt);


/**
 * @details   Receive an unsigned number from EUSCI_A0 UART
 * @details   Accepts ASCII input in unsigned decimal format and converts to a 32-bit unsigned number
 * @details   Valid range is 0 to 4294967295 (2^32-1)
 * @warning  If you enter a number above 4294967295, it will return an incorrect value
 * @param  none
 * @return 32-bit unsigned number that was received
 * @note   UART0_Init must be called once prior
 * @note   Backspace will remove last digit typed
 * @brief  Receive a number into MSP432
 */
uint32_t UART0_InUDec(void);


/**
 * @details   Transmit a number as ASCII characters in unsigned decimal format
 * @details   Variable format 1-10 digits with no space before or after
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit an unsigned number out of MSP432
 */
void UART0_OutUDec(uint32_t n);


/**
 * @details   Receive an unsigned number from EUSCI_A0 UART
 * @details   Accepts ASCII input in unsigned hexadecimal format and converts to a 32-bit unsigned number
 * @details   Valid range is 0 to FFFFFFFF (2^32-1)
 * @warning  If you enter a number above FFFFFFFF, it will return an incorrect value
 * @param  none
 * @return 32-bit unsigned number that was received
 * @note   UART0_Init must be called once prior
 * @note   No '$' or '0x' need be entered, just the 1 to 8 hex digits
 * @note   Backspace will remove last digit typed
 * @brief  Receive a number into MSP432
 */
uint32_t UART0_InUHex(void);


/**
 * @details   Transmit a number as ASCII characters in unsigned hexadecimal format
 * @details   Variable format 1-8 digits with no space before or after
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit an unsigned number out of MSP432
 */
void UART0_OutUHex(uint32_t n);


/**
 * @details   Transmit a number as ASCII characters in unsigned hexadecimal format
 * @details   Fixed format 2 digits with no space before or after
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @warning only numbers from 0x00 to 0xFF will be properly displayed
 * @brief  Transmit an unsigned number out of MSP432
 */
void UART0_OutUHex2(uint32_t n);


/**
 * @details    Receive an ASCII string from EUSCI_A0 UART
 * @details    Accepts ASCII input until <enter>
 * @details    It echoes each character as it is input.
 * @warning    If more than max-1 characters are received, subsequent characters are discarded until the <enter> is received
 * @param[out] bufPt is a pointer to an empty buffer into which characters are stored
 * @param[in]  max is the size of the buffer
 * @return     32-bit unsigned number that was received
 * @note       UART0_Init must be called once prior
 * @note       Backspace will remove last digit typed
 * @brief      Receive a string into MSP432
 */
void UART0_InString(char *bufPt, uint16_t max);


/**
 * @details   Transmit a number as ASCII characters in unsigned decimal format
 * @details   Fixed format 4 digits with no space before or after
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @note   Characters are aligned to right
 * @warning only numbers from 0 to 9999 will be properly displayed
 * @brief  Transmit an unsigned number out of MSP432
 */
void UART0_OutUDec4(uint32_t n);

/**
 * @details   Transmit a number as ASCII characters in unsigned decimal format
 * @details   Fixed format 5 digits with no space before or after
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @note   Characters are aligned to right
 * @warning only numbers from 0 to 99999 will be properly displayed
 * @brief  Transmit an unsigned number out of MSP432
 */
void UART0_OutUDec5(uint32_t n);

/**
 * @details   Transmit a number as ASCII characters in unsigned decimal fixed-point format
 * @details   Fixed-format 3 or more character output with no space before or after
 * @details   n    sends
 * @details   0    "0.0"
 * @details   5    "0.5"
 * @details   12   "1.2"
 * @details   999  "99.9"
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit an unsigned decimal fixed-point number out of MSP432
 */
void UART0_OutUFix1(uint32_t n);

/**
 * @details   Transmit a number as ASCII characters in unsigned decimal fixed-point format
 * @details   Fixed-format 4 or more character output with no space before or after
 * @details   n     sends
 * @details   0     "0.00"
 * @details   5     "0.05"
 * @details   123   "1.23"
 * @details   9999  "99.99"
 * @param  n is a unsigned 32-bit number to be transferred
 * @return none
 * @note   UART0_Init must be called once prior
 * @brief  Transmit an unsigned decimal fixed-point number out of MSP432
 */
void UART0_OutUFix2(uint32_t n);
