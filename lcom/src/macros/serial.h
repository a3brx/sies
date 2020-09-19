#pragma once

#define COM1_IRQ 4
#define COM2_IRQ 3

#define COM1_BASE_ADDR 0x3F8
#define COM1_LAST_ADDR 0x3FF
#define COM2_BASE_ADDR 0x2F8
#define COM2_LAST_ADDR 0x2FF

#define RECEIVED_BUF_ADDR 0         //read
#define TRANSMIT_BUF_ADDR 0         //write
#define INT_ENABLE_REG_ADDR 1
#define INT_ID_ADDR 2               //read
#define FIFO_CTRL_ADDR 2            //write
#define LINE_CTRL_ADDR 3
#define LINE_STATUS_REG_ADDR 5
#define DVSR_LATCH_LSB_ADDR 0       //DLAB in LINE_CTRL
#define DVSR_LATCH_MSB_ADDR 1       // must be 1

//LCR
#define WORDS_8BITS BIT(0) | BIT(1)
#define STOP_BITS_2 BIT(2)
#define NO_PARITY_MASK 0xf7
#define DLAB_BIT BIT(7)
#define DLAB_BIT_RATE_DIV 115200
#define BIT_RATE 9600

//LSR
#define RECEIVER_RDY BIT(0)
#define OVERRUN_ERROR BIT(1)
#define PARITY_ERROR BIT(2)
#define FRAMING_ERROR BIT(3)
#define TRANSMIT_HOLD_REG_EMPTY BIT(5)

//IER
#define ENABLE_DATA_RECEIVED_INT BIT(0)
#define ENABLE_TRANSMIT_EMPTY_INT BIT(1)
#define ENABLE_RECEIVER_LINE_STATUS_INT BIT(2) //for errors
#define NO_INTS_MASK 0xf0

//IIR
#define INT_STATUS BIT(0)
#define FIFO_STATUS_UNUSED BIT(7)
#define FIFO_STATUS_ENABLED BIT(6) | BIT(7)

//FCR
#define ENABLE_FIFO BIT(0)
#define CLEAR_RECEIVE_FIFO BIT(1)
#define CLEAR_TRANSMIT_FIFO BIT(2)
#define FIFO_INT_4BYTE_TRIGGER BIT(6) 

