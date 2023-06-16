#include "UART0.h"

//------------UART0_Init------------
// Initialize the UART for 115,200 baud rate (assuming 12 MHz SMCLK clock),
// 8 bit word length, no parity bits, one stop bit
// Input: none
// Output: none
void UART0_Init(void)
{
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;                   // hold the USCI module in reset mode

    // set the baud rate
    // N = clock/baud rate = 12,000,000/115,200 = 104.1667
    EUSCI_A0->BRW = 104;           // UCBR = baud rate = int(N) = 104
    EUSCI_A0->MCTLW &= ~0xFFF1;    // clear first and second modulation stage bit fields
    P1->SEL0 |= 0x0C;
    P1->SEL1 &= ~0x0C;             // configure P1.3 and P1.2 as primary module function

    EUSCI_A0->CTLW0 |= (EUSCI_A_CTLW0_SSEL__SMCLK);
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // enable the USCI module

    // disable interrupts (transmit ready, start received, transmit empty, receive full)
    EUSCI_A0->IE &= ~(EUSCI_A_IFG_TXCPTIFG | EUSCI_A_IFG_STTIFG | EUSCI_A_IE_TXIE | EUSCI_A_IE_RXIE);
}

//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART0_InChar(void)
{
    while((EUSCI_A0->IFG&0x01) == 0);
    return((char)(EUSCI_A0->RXBUF));
}

//------------UART0_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(char letter)
{
    while((EUSCI_A0->IFG&0x02) == 0);
    EUSCI_A0->TXBUF = letter;
}

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART0_OutString(char *pt)
{
    while(*pt){
        UART0_OutChar(*pt);
        pt++;
    }
}

//------------UART0_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_InUDec(void)
{
    uint32_t number=0, length=0;
    char character;
    character = UART0_InChar();
    while(character != CR){ // accepts until <enter> is typed
        // The next line checks that the input is a digit, 0-9.
        // If the character is not 0-9, it is ignored and not echoed
        if((character>='0') && (character<='9')) {
            number = 10*number+(character-'0');   // this line overflows if above 4294967295
            length++;
            UART0_OutChar(character);
        }
        // If the input is a backspace, then the return number is
        // changed and a backspace is output to the screen
        else if((character==BS) && length){
            number /= 10;
            length--;
            UART0_OutChar(character);
        }
        character = UART0_InChar();
    }
    return number;
}

//-----------------------UART0_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_OutUDec(uint32_t n)
{
    // This function uses recursion to convert decimal number
    //   of unspecified length as an ASCII string
    if(n >= 10){
        UART0_OutUDec(n/10);
        n = n%10;
    }
    UART0_OutChar(n+'0'); /* n is between 0 and 9 */
}

uint32_t Messageindexb;
char Messageb[8];

static void fillmessageb(uint32_t n)
{
    // This function uses recursion to convert decimal number
    //   of unspecified length as an ASCII string
    if(n >= 10){
        fillmessageb(n/10);
        n = n%10;
    }
    Messageb[Messageindexb] = (n+'0'); /* n is between 0 and 9 */
    if(Messageindexb<7)Messageindexb++;
}

static void fillmessage4b(uint32_t n)
{
    if(n>=1000){  // 1000 to 9999
        Messageindexb = 0;
    } else if(n>=100){  // 100 to 999
        Messageb[0] = ' ';
        Messageindexb = 1;
    }else if(n>=10){ //
        Messageb[0] = ' '; /* n is between 10 and 99 */
        Messageb[1] = ' ';
        Messageindexb = 2;
    }else{
        Messageb[0] = ' '; /* n is between 0 and 9 */
        Messageb[1] = ' ';
        Messageb[2] = ' ';
        Messageindexb = 3;
    }
    fillmessageb(n);
}

static void fillmessage5b(uint32_t n)
{
    if(n>99999)n=99999;
    if(n>=10000){  // 10000 to 99999
        Messageindexb = 0;
    } else if(n>=1000){  // 1000 to 9999
        Messageb[0] = ' ';
        Messageindexb = 1;
    }else if(n>=100){  // 100 to 999
        Messageb[0] = ' ';
        Messageb[1] = ' ';
        Messageindexb = 2;
    }else if(n>=10){ //
        Messageb[0] = ' '; /* n is between 10 and 99 */
        Messageb[1] = ' ';
        Messageb[2] = ' ';
        Messageindexb = 3;
    }else{
        Messageb[0] = ' '; /* n is between 0 and 9 */
        Messageb[1] = ' ';
        Messageb[2] = ' ';
        Messageb[3] = ' ';
        Messageindexb = 4;
    }
    fillmessageb(n);
}

//-----------------------UART0_OutUDec4-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits with no space before or after
void UART0_OutUDec4(uint32_t n)
{
    if(n>9999){
        UART0_OutString("****");
    }else{
        Messageindexb = 0;
        fillmessage4b(n);
        Messageb[Messageindexb] = 0; // terminate
        UART0_OutString(Messageb);
    }
}

//-----------------------UART0_OutUDec5-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 5 digits with no space before or after
void UART0_OutUDec5(uint32_t n)
{
    if(n>99999){
        UART0_OutString("*****");
    }else{
        Messageindexb = 0;
        fillmessage5b(n);
        Messageb[Messageindexb] = 0; // terminate
        UART0_OutString(Messageb);
    }
}

//-----------------------UART0_OutUFix1-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// fixed format <digit>.<digit> with no space before or after
void UART0_OutUFix1(uint32_t n)
{
    UART0_OutUDec(n/10);
    UART0_OutChar('.');
    UART0_OutUDec(n%10);
}

//-----------------------UART0_OutUFix2-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// fixed format <digit>.<digit><digit> with no space before or after
void UART0_OutUFix2(uint32_t n)
{
    UART0_OutUDec(n/100);
    UART0_OutChar('.');
    n = n%100;
    UART0_OutUDec(n/10);
    UART0_OutUDec(n%10);
}

//---------------------UART0_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to upper-case A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_InUHex(void)
{
    uint32_t number=0, digit, length=0;
    char character;
    character = UART0_InChar();
    while(character != CR){
        digit = 0x10; // assume bad
        if((character>='0') && (character<='9')){
            digit = character-'0';
        }
        else if((character>='A') && (character<='F')){
            digit = (character-'A')+0xA;
        }
        else if((character>='a') && (character<='f')){
            digit = (character-'a')+0xA;
        }
        // If the character is not 0-9 or A-F, it is ignored and not echoed
        if(digit <= 0xF){
            number = number*0x10+digit;
            length++;
            UART0_OutChar(character);
        }
        // Backspace outputted and return value changed if a backspace is inputted
        else if((character==BS) && length){
            number /= 0x10;
            length--;
            UART0_OutChar(character);
        }
        character = UART0_InChar();
    }
    return number;
}

//--------------------------UART0_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART0_OutUHex(uint32_t number)
{
    // This function uses recursion to convert the number of
    //   unspecified length as an ASCII string
    if(number >= 0x10){
        UART0_OutUHex(number/0x10);
        UART0_OutUHex(number%0x10);
    }
    else{
        if(number < 0xA){
            UART0_OutChar(number+'0');
        }
        else{
            UART0_OutChar((number-0x0A)+'A');
        }
    }
}

//--------------------------UART0_OutUHex2----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 2 digits with no space before or after
void outnibble(uint32_t n)
{
    if(n < 0xA){
        UART0_OutChar(n+'0');
    }
    else{
        UART0_OutChar((n-0x0A)+'A');
    }
}

void UART0_OutUHex2(uint32_t number)
{
    outnibble(number/0x10); // ms digit
    outnibble(number%0x10); // ls digit
}

//------------UART0_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is input.
// If a backspace is input, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART0_InString(char *bufPt, uint16_t max)
{
    int length=0;
    char character;
    character = UART0_InChar();
    while(character != CR){
        if(character == BS){
            if(length){
                bufPt--;
                length--;
                UART0_OutChar(BS);
            }
        }
        else if(length < max){
            *bufPt = character;
            bufPt++;
            length++;
            UART0_OutChar(character);
        }
        character = UART0_InChar();
    }
    *bufPt = 0;
}

// Get input from UART, echo
int fgetc (FILE *f)
{
    char ch = UART0_InChar();  // receive from keyboard
    UART0_OutChar(ch);            // echo
    return ch;
}

// Function called when file error occurs.
//int ferror(FILE *f){
//  /* Your implementation of ferror */
//  return EOF;
//}

// Abstraction of general output device
// Volume 2 section 3.4.5

//-----CCS implementation
#include "file.h"
int uart_open(const char *path, unsigned flags, int llv_fd)
{
    UART0_Init();
    return 0;
}

int uart_close( int dev_fd)
{
    return 0;
}

int uart_read(int dev_fd, char *buf, unsigned count)
{
    char ch;
    ch = UART0_InChar();    // receive from keyboard
    ch = *buf;         // return by reference
    UART0_OutChar(ch);  // echo
    return 1;
}

int uart_write(int dev_fd, const char *buf, unsigned count)
{
    unsigned int num=count;
    while(num){
        if(*buf == 10){
            UART0_OutChar(13);
        }
        UART0_OutChar(*buf);
        buf++;
        num--;
    }
    return count;
}

off_t uart_lseek(int dev_fd, off_t ioffset, int origin)
{
    return 0;
}

int uart_unlink(const char * path)
{
    return 0;
}

int uart_rename(const char *old_name, const char *new_name)
{
    return 0;
}

//------------UART0_Initprintf------------
// Initialize the UART for 115,200 baud rate (assuming 48 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit
// Input: none
// Output: none
void UART0_Initprintf(void)
{
    int ret_val; FILE *fptr;
    UART0_Init();
    ret_val = add_device("uart", _SSA, uart_open, uart_close, uart_read, uart_write, uart_lseek, uart_unlink, uart_rename);
    if(ret_val) return; // error
    fptr = fopen("uart","w");
    if(fptr == 0) return; // error
    freopen("uart:", "w", stdout); // redirect stdout to uart
    setvbuf(stdout, NULL, _IONBF, 0); // turn off buffering for stdout
}

/*
// Keil uVision Code
// Print a character to UART.
//------------UART0_Initprintf------------
// Initialize the UART0_Initprintf for 115,200 baud rate (assuming 3 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit
// Input: none
// Output: none
void UART0_Initprintf(void){
  UART0_Init();
}
int fputc(int ch, FILE *f){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    UART0_OutChar(13);
    UART0_OutChar(10);
    return 1;
  }
  UART0_OutChar(ch);
  return 1;
}
 */

