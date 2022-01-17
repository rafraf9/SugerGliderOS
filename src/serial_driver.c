#include "io.h"

#define SERIAL_COM1_BASE                0X3F8   /*COM1 base port*/

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/*The I/O Serial port commands*/
#define SERIAL_LINE_ENABLE_DLAB     0X80

//Check weather the serial port was initialized yet
static int serial_initialized = 0;

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. the defualt speed of a serial
 *  port is 115200 bits/s. the argument is a divisor of that number, so
 *  the resulting speed is (115200 / divisor) bits/s
 * @param com       The COM port to configure
 * @param divisor   The divisor  
*/
void serial_configure_bit_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port. the port is set to have a
 * data length of 8 bits, no parity bits, one stop bit and break control
 * disabled.
 * 
 * @param com The serial port to configure
*/
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
    * Content: | d | b | prty  | s | dl  |
    * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
    */
   outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_modem:
 *  Configures the model of the given serial port. 
 *  the modem is set to have RTS/DTR = 1, which means
 *  we are ready to send data
 * 
 * @param com the serial port to configure
*/
void serial_configure_modem(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   |  1  |  0  |
    * Content:  | r | r | af | lb | a02 | a01 | rts | dtr |
    * Value:    | 0 | 0 | 0  | 0  |  0  |  0  |  1  |  1  | = 0x03
    */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO buffer is empty
 *  for a given COM port.
 * 
 * @param com The COM port to check
 * @returns 1 if empty, otherwise returns 0
*/
int serial_is_transmit_fifo_empty(unsigned short com)
{
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** test_serial_port:
 *  Checks whether the serial port is working by testing 
 *  the loopback feature
 * 
 *  @param com The COM port to test
 *  @returns 1 if the COM port is working, otherwise 0
*/
int test_serial_port(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0X12);  // Set the port in loopback mode

    //Test the serial port(send 0xAB and see if it returns.) 
    outb(SERIAL_DATA_PORT(com), 0xAB);
    if(inb(SERIAL_DATA_PORT(com)) != 0xAB)
        return 0;

    //If the serial port is working set it in normal operation mode
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x3);
    return 1;           
}

/** serial_port_send_byte
 *  Sends a byte over the serial port specified
 * 
 *  @param com The COM port to send data over
 *  @param c   The byte to send over the serial port
 */
void serial_port_send_byte(unsigned short com, unsigned char c)
{
    while(!serial_is_transmit_fifo_empty(com));

    outb(SERIAL_DATA_PORT(com), c);
}

/** init_serial:
 *  initializes a serial port and runs a quick test to see if its working
 * 
 * @param com The COM port to initialize
 * @returns 1 if initialized succesfully, otherwise 0
 */
int init_serial(unsigned short com)
{
    serial_configure_bit_rate(com, 3);      //Configure the bitrate with divisor of 3
    serial_configure_line(com);             //Configure the COM line
    serial_configure_modem(com);            //Configure the COM modem
    return test_serial_port(com);           //Return the test results
}

/** serial_write:
 *  Writes a buffer of length len to the serial port
 * 
 *  @param buf The buffer being writen to the serial port
 *  @param len The length of the buffer being written
 *  @returns The number of characters written to the serial port, returns -1 if failed
*/
int serial_write(const char* buf, unsigned int len)
{
    if(buf == 0)
        return -1;
    if(!serial_initialized)
    {
        if(!init_serial(SERIAL_COM1_BASE))
            return -1;
        serial_initialized = 1;
    }

    for(unsigned int i = 0; i < len; ++i)
    {
        serial_port_send_byte(SERIAL_COM1_BASE, buf[i]);
    }

    return len;
}