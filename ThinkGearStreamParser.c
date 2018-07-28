/*
 * @(#)ThinkGearStreamParser.c    2.0    Mar 04, 2008
 *
 * Copyright (c) 2008 NeuroSky, Inc. All Rights Reserved
 * NEUROSKY PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * @file ThinkGearStreamParser.c
 *
 * @author Kelvin Soo
 * @version 2.0 Mar 04, 2008 Kelvin Soo
 *   - Renamed to ThinkGearStreamParser from ThinkGearStreamDecoder.
 *   - Revised to call a callback function instead of stuffing arriving
 *     data values into a ThinkGearData object.
 *   - Renamed symbols according to updated Packet vocabulary.
 * @version 1.0 Nov 27, 2007 Kelvin Soo
 *   - Initial version (ThinkGearStreamDecoder).
 */

#include "ThinkGearStreamParser.h"

/* Include libraries required specifically by this implementation of this
 * library and not already included by this library's header
 */
/*包括这个实现所需要的库，这个库的标题还没有包括库*/
#include <stdio.h>

/* Decoder states (Packet decoding) *//*解码状态（包解码*/
#define PARSER_STATE_NULL           0x00  /* NULL state */
#define PARSER_STATE_SYNC           0x01  /* Waiting for SYNC byte */
#define PARSER_STATE_SYNC_CHECK     0x02  /* Waiting for second SYNC byte */
#define PARSER_STATE_PAYLOAD_LENGTH 0x03  /* Waiting for payload[] length */
#define PARSER_STATE_PAYLOAD        0x04  /* Waiting for next payload[] byte */
#define PARSER_STATE_CHKSUM         0x05  /* Waiting for chksum byte */

/* Decoder states (2-byte raw decoding) */
#define PARSER_STATE_WAIT_HIGH      0x06  /* Waiting for high byte */
#define PARSER_STATE_WAIT_LOW       0x07  /* High r'cvd.  Expecting low part */

/* Other constants */
#define PARSER_SYNC_BYTE            0xAA  /* Syncronization byte */
#define PARSER_EXCODE_BYTE          0x55  /* EXtended CODE level byte */

/* Declare private function prototypes */
int
parsePacketPayload( ThinkGearStreamParser *parser );
int
parseDataRow( ThinkGearStreamParser *parser, unsigned char *rowPtr );

/*
 * See header file for interface documentation.
 */
//有关接口文档的头文件
int
THINKGEAR_ini#include <stdio.h>
#define SYNC 0xAA
#define EXCODE 0x55
int parsePayload( unsigned char *payload, unsigned char pLength ) {
unsigned char bytesParsed = 0;
unsigned char code;
unsigned char length;
unsigned char extendedCodeLevel;
int i;
/* Loop until all bytes are parsed from the payload[] array... */
//循环，直到从有效负载数组中解析所有字节。
while( bytesParsed < pLength )
 {
/* Parse the extendedCodeLevel, code, and length */
//解析extendedCodeLevel、代码和长度
extendedCodeLevel = 0;
while( payload[bytesParsed] == EXCODE )
 {
extendedCodeLevel++;
bytesParsed++;
}
code = payload[bytesParsed++];
if( code & 0x80 ) length = payload[bytesParsed++];
else length = 1;
/* TODO: Based on the extendedCodeLevel, code, length,
* and the [CODE] Definitions Table, handle the next
* "length" bytes of data from the payload as
* appropriate for your application.
*/
/*待办事项：基于扩展的codelevel、代码、长度和代码定义表，
根据您的应用程序，处理来自有效负载的下一个“长度”字节数据。*/
printf( "EXCODE level: %d CODE: 0x%02X length: %d\n",
extendedCodeLevel, code, length );
printf( "Data value(s):" );
for( i=0; i<length; i++ ) {
printf( " %02X", payload[bytesParsed+i] & 0xFF );
}
printf( "\n" );
/* Increment the bytesParsed by the length of the Data Value */
//增加由数据值长度解析的字节
bytesParsed += length;
}
return( 0 );
}
int main( int argc, char **argv ) {
int checksum;
unsigned char payload[256];
unsigned char pLength;
unsigned char c;
unsigned char i;
/* TODO: Initialize 'stream' here to read from a serial data
* stream, or whatever stream source is appropriate for your
* application. See documentation for "Serial I/O" for your
* platform for details.
*/

/*待办事项：初始化“流”，从串行数据流中读取，或者任何适合您的应用程序的流源。
有关详细信息，请参阅文档“串行输入/输出”文档。*/
FILE *stream = 0;
stream = fopen( "COM4", "r" );
/* Loop forever, parsing one Packet per loop... */
//持续循环，每循环解析一个数据包。
while( 1 ) {
/* Synchronize on [SYNC] bytes */
//同步字节
fread( &c, 1, 1, stream );
if( c != SYNC ) continue;
fread( &c, 1, 1, stream );
if( c != SYNC ) continue;
/* Parse [PLENGTH] byte */
//解析字节
while( true ) {
fread( &pLength, 1, 1, stream );
if( pLength ~= 170 ) break;
}
if( pLength > 169 ) continue;
/* Collect [PAYLOAD...] bytes */
//收集字节
fread( payload, 1, pLength, stream );
/* Calculate [PAYLOAD...] checksum */
checksum = 0;
for( i=0; i<pLength; i++ ) checksum += payload[i];
checksum &= 0xFF;
checksum = ~checksum & 0xFF;
/* Parse [CKSUM] byte */
//计算[负载……)校验和
fread( &c, 1, 1, stream );
/* Verify [CKSUM] byte against calculated [PAYLOAD...] checksum */
//验证CKSUM字节与计算有效负载……)校验和
if( c != checksum ) continue;
/* Since [CKSUM] is OK, parse the Data Payload */
//因为CKSUM已经完成，解析数据有效负载
parsePayload( payload, pLength );
}
return( 0 );
}tParser( ThinkGearStreamParser *parser,
                      unsigned char parserType,
                      void (*handleDataValueFunc)(
                          unsigned char extendedCodeLevel,
                          unsigned char code, unsigned char numBytes,
                          const unsigned char *value, void *customData),
                      void *customData ) {

    if( !parser ) return( -1 );

    /* Initialize the parser's state based on the parser type */
    //根据解析器类型初始化解析器的状态
    switch( parserType ) {
        case( PARSER_TYPE_PACKETS ):
            parser->state = PARSER_STATE_SYNC;
            break;
        case( PARSER_TYPE_2BYTERAW ):
            parser->state = PARSER_STATE_WAIT_HIGH;
            break;
        default: return( -2 );
    }

    /* Save parser type */
    //保存解析器类型
    parser->type = parserType;

    /* Save user-defined handler function and data pointer */
    //保存用户定义的handler函数和数据指针
    parser->handleDataValue = handleDataValueFunc;
    parser->customData = customData;

    return( 0 );
}

/*
 * See header file for interface documentation.
 */
//有关接口文档的头文件
int
THINKGEAR_parseByte( ThinkGearStreamParser *parser, unsigned char byte ) {

    int returnValue = 0;

    if( !parser ) return( -1 );

    /* Pick handling according to current state... */
    //根据当前状态选择处理。
    switch( parser->state ) {

        /* Waiting for SyncByte */
        //等待SyncByte
        case( PARSER_STATE_SYNC ):
            if( byte == PARSER_SYNC_BYTE ) {
                parser->state = PARSER_STATE_SYNC_CHECK;
            }
            break;

        /* Waiting for second SyncByte */
        //等待下一个个SyncByte
        case( PARSER_STATE_SYNC_CHECK ):
            if( byte == PARSER_SYNC_BYTE ) {
                parser->state = PARSER_STATE_PAYLOAD_LENGTH;
            } else {
                parser->state = PARSER_STATE_SYNC;
            }
            break;

        /* Waiting for Data[] length */
       //等待数据[]的长度
        case( PARSER_STATE_PAYLOAD_LENGTH ):
            parser->payloadLength = byte;
            if( parser->payloadLength > 170 ) {
                parser->state = PARSER_STATE_SYNC;
                returnValue = -3;
            } else if( parser->payloadLength == 170 ) {
                returnValue = -4;
            } else {
                parser->payloadBytesReceived = 0;
                parser->payloadSum = 0;
                parser->state = PARSER_STATE_PAYLOAD;
            }
            break;

        /* Waiting for Payload[] bytes */
        //等待有效载荷字节
        case( PARSER_STATE_PAYLOAD ):
            parser->payload[parser->payloadBytesReceived++] = byte;
            parser->payloadSum = (unsigned char)(parser->payloadSum + byte);
            if( parser->payloadBytesReceived >= parser->payloadLength ) {
                parser->state = PARSER_STATE_CHKSUM;
            }
            break;

        /* Waiting for CKSUM byte */
        //等待校验和字节
        case( PARSER_STATE_CHKSUM ):
            parser->chksum = byte;
            parser->state = PARSER_STATE_SYNC;
            if( parser->chksum != ((~parser->payloadSum)&0xFF) ) {
                returnValue = -2;
            } else {
                returnValue = 1;
                parsePacketPayload( parser );
            }
            break;

        /* Waiting for high byte of 2-byte raw value */
        //等待高字节的2字节原始值
        case( PARSER_STATE_WAIT_HIGH ):

            /* Check if current byte is a high byte */
            //检查当前字节是否是一个高字节
            if( (byte & 0xC0) == 0x80 ) {
                /* High byte recognized, will be saved as parser->lastByte */
                //高字节识别，将被保存为解析器-lastByte
                parser->state = PARSER_STATE_WAIT_LOW;
            }
            break;

        /* Waiting for low byte of 2-byte raw value */
        //等待低字节的2字节原始值
        case( PARSER_STATE_WAIT_LOW ):

            /* Check if current byte is a valid low byte */
            //检查当前字节是否是有效的低字节
            if( (byte & 0xC0) == 0x40 ) {

                /* Stuff the high and low part of the raw value into an array */
                //将原始值的高和低部分放入一个数组中
                parser->payload[0] = parser->lastByte;
                parser->payload[1] = byte;

                /* Notify the handler function of received raw value */
                //通知收到的原始值的处理程序函数
                if( parser->handleDataValue ) {
                    parser->handleDataValue( 0, PARSER_CODE_RAW_SIGNAL, 2,
                                             parser->payload,
                                             parser->customData );
                }

                returnValue = 1;
            }

            /* Return to start state waiting for high */
            //返回到开始状态等待高
            parser->state = PARSER_STATE_WAIT_HIGH;

            break;

        /* unrecognized state */
        //未识别的状态
        default:
            parser->state = PARSER_STATE_SYNC;
            returnValue = -5;
            break;
    }

    /* Save current byte */
    //保存当前字节
    parser->lastByte = byte;

    return( returnValue );
}

/**
 * Parses each row of data from the @c packet's Data[] block,
 * updating the fields of @c data as appropriate.
 */
//从@c包的数据块中解析每一行数据，适当地更新@c数据的字段。
int
parsePacketPayload( ThinkGearStreamParser *parser ) {

    unsigned char i = 0;
    unsigned char extendedCodeLevel = 0;
    unsigned char code = 0;
    unsigned char numBytes = 0;

    /* Parse all bytes from the payload[] */
    //从有效负载中解析所有字节
    while( i < parser->payloadLength ) {

        /* Parse possible EXtended CODE bytes */
        //解析可能的扩展码字节
        while( parser->payload[i] == PARSER_EXCODE_BYTE ) {
            extendedCodeLevel++;
            i++;
        }

        /* Parse CODE */
        //解析代码
        code = parser->payload[i++];

        /* Parse value length */
        //解析值长度
        if( code >= 0x80 ) numBytes = parser->payload[i++];
        else               numBytes = 1;

        /* Call the callback function to handle the DataRow value */
        //调用回调函数来处理数据行值
        if( parser->handleDataValue ) {
            parser->handleDataValue( extendedCodeLevel, code, numBytes,
                                     parser->payload+i, parser->customData );
        }
        i = (unsigned char)(i + numBytes);
    }

    return( 0 );
}
