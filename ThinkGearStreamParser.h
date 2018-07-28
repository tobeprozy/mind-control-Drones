/*
 * @(#)ThinkGearStreamParser.h    2.0    Mar 04, 2008
 *
 * Copyright (c) 2008 NeuroSky, Inc. All Rights Reserved
 * NEUROSKY PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Ensure this header is only included once */
/*确保这个标题只包含一次*/
#ifndef THINKGEAR_STREAM_PARSER_H_
#define THINKGEAR_STREAM_PARSER_H_

/**
 * @file ThinkGearStreamParser.h
 *
 * The ThinkGear Stream Parser is used to parse bytes of ThinkGear
 * data streamed from any source.  Each arriving byte is fed into
 * the Parser using the THINKGEAR_parseByte() function, and a user
 * supplied callback function is called whenever DataRows are decoded
 * from complete ThinkGear Packets.  Each Parser can be initialized
 * to recognize either: streams of ThinkGear Packets, or streams
 * of 2-byte raw values (the old 5V ThinkGear stream format).
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
/*ThinkGear Stream解析器用于解析来自任何来源的ThinkGear数据的字节数。
每一个到达的字节都使用thinkgearparsebyte（）函数输入到解析器中，当数据
从完整的ThinkGear数据包中解码时，就会调用一个用户提供的回调函数。每个解
析器都可以被初始化来识别：ThinkGear数据包流，或者2字节的原始值流（旧的
5V ThinkGear流格式）。
——从thinkgearstream解码器重新命名为ThinkGearStreamParser。
-修正为调用一个回调函数而不是填充到
数据值放入ThinkGearData对象。
-根据更新后的数据包词汇重新命名符号。
2007年11月27日，2007年
-初始版本（thinkgearstream解码器）。
* /

/* Include all external libraries required by this header */
/*包括这个标题所要求的所有外部库*/
#include <stdlib.h>

/* Disable name-mangling when compiling as C++ */
//禁用名称改编时编译c++
#ifdef __cplusplus
extern "C" {
#endif

/* Parser types */
//解析器类型
#define PARSER_TYPE_NULL       0x00
#define PARSER_TYPE_PACKETS    0x01    /* Stream bytes as ThinkGear Packets *///流字节作为ThinkGear数据包
#define PARSER_TYPE_2BYTERAW   0x02    /* Stream bytes as 2-byte raw data *///以2字节的原始数据流字节

/* Data CODE definitions */
//数据代码定义
#define PARSER_CODE_BATTERY            0x01
#define PARSER_CODE_POOR_QUALITY       0x02
#define PARSER_CODE_ATTENTION          0x04
#define PARSER_CODE_MEDITATION         0x05
#define PARSER_CODE_8BITRAW_SIGNAL     0x06
#define PARSER_CODE_RAW_MARKER         0x07

#define PARSER_CODE_RAW_SIGNAL         0x80
#define PARSER_CODE_EEG_POWERS         0x81
#define PARSER_CODE_ASIC_EEG_POWER_INT 0x83

/**
 * The Parser is a state machine that manages the parsing state.
 */
//解析器是管理解析状态的状态机
typedef struct _ThinkGearStreamParser {

    unsigned char   type;
    unsigned char   state;

    unsigned char   lastByte;

    unsigned char   payloadLength;
    unsigned char   payloadBytesReceived;
    unsigned char   payload[256];
    unsigned char   payloadSum;
    unsigned char   chksum;

    void (*handleDataValue)( unsigned char extendedCodeLevel,
                             unsigned char code, unsigned char numBytes,
                             const unsigned char *value, void *customData );
    void  *customData;

} ThinkGearStreamParser;

/**
 * @param parser              Pointer to a ThinkGearStreamParser object.
 * @param parserType          One of the PARSER_TYPE_* constants defined above:
 *                            PARSER_TYPE_PACKETS or PARSER_TYPE_2BYTERAW.
 * @param handleDataValueFunc A user-defined callback function that will
 *                            be called whenever a data value is parsed
 *                            from a Packet.
 * @param customData          A pointer to any arbitrary data that will
 *                            also be passed to the handleDataValueFunc
 *                            whenever a data value is parsed from a
 *                            Packet.
 *
 * @return -1 if @c parser is NULL.
 * @return -2 if @c parserType is invalid.
 * @return 0 on success.
 */
int
THINKGEAR_initParser( ThinkGearStreamParser *parser, unsigned char parserType,
                      void (*handleDataValueFunc)(
                          unsigned char extendedCodeLevel,
                          unsigned char code, unsigned char numBytes,
                          const unsigned char *value, void *customData),
                      void *customData );

/**
 * This is merely an example function prototype for a handleDataValueFunc()
 * callback function to be passed to THINKGEAR_initParser().  The user is
 * expected to actually define a function with this function signature, and pass
 * its name into the THINKGEAR_initParser() to create a ThinkGearStreamParser.
 * Then, each time a Data Value is successfully parsed by THINKGEAR_parseByte(),
 * this function gets automatically called, so that the user's application can
 * do whatever it likes with the @c value.
 *
 * The @c extendedCodeLevel and @c code together tells the application
 * what type of Data Value was received. The @c numBytes indicates the
 * number of bytes in the @c value[] array.
 *
 * The @c customData is a pointer to whatever user-defined data was
 * originally passed to THINKGEAR_initParser().
 *
 */
/**
@param解析器指针指向ThinkGearStreamParser对象。
@param parserType的parserType常量之一：parsertype信息包或parsertype2byteraw。
@param handleDataValueFunc是用户定义的回调函数，每当从数据包解析数据值时就会调用它。
@param定制了一个指向任意数据的指针，当数据值从数据包中解析时，它也会被传递给handleDataValueFunc。
@return-1如果@c解析器为空。
@return-2如果@c parserType无效。
成功的@返回0。
*/
void
exampleHandleDataValueFunc( unsigned char extendedCodeLevel,
                            unsigned char code,
                            unsigned char numBytes,
                            const unsigned char *value,
                            void *customData );


/**
 * Feeds the @c byte into the @c parser.  If the @c byte completes a
 * complete, valid parser, then the @c parser's handleDataValue()
 * function is automatically called on each DataRow in the Packet.
 * The return value provides an indication of the state of the
 * @c parser after parsing the byte.
 *
 * @param parser Pointer to an initialized ThinkGearDataParser object.
 * @param byte   The next byte of the data stream.
 *
 * @return -1 if @c parser is NULL.
 * @return -2 if a complete Packet was received, but the checksum failed.
 * @return -3 if an invalid Packet with PLENGTH > 170 was detected.
 * @return -4 if an invalid Packet with PLENGTH == 170 was detected.
 * @return -5 if the @c parser is somehow in an unrecognized state.
 * @return 0 if the @c byte did not yet complete a Packet.
 * @return 1 if a Packet was received and parsed successfully.
 *
 */
/**
将@c字节输入到@c解析器中。如果@c字节完成了一个完整的、有效的解析器，那么@c解析器的handleDataValue（）函数就会自动在数据包中的每个DataRow上调用。返回值提供了一种状态的指示
解析字节后的@c解析器。
@param解析器指针指向一个初始化的ThinkGearDataParser对象。
@param字节是数据流的下一个字节。
*
@return-1如果@c解析器为空。
如果收到完整的数据包，则返回-2，但是校验和失败了。
如果检测到带有PLENGTH 170的无效数据包，则会被检测到。
如果检测到有PLENGTH==170的无效包，则返回-4。
如果@c解析器处于某种未被识别的状态，则@return-5。
如果@c字节还没有完成一个数据包，则返回0。
如果接收并成功解析数据包，则返回1。
*/
int
THINKGEAR_parseByte( ThinkGearStreamParser *parser, unsigned char byte );


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* THINKGEAR_STREAM_DECODER_H_ */
