#include <stdio.h>
#include "ThinkGearStreamParser.h"
/**
* 1) Function which acts on the value[] bytes of each ThinkGear DataRow as it is received.
*/
//作用于每个ThinkGear数据的值字节的函数
void
handleDataValueFunc( unsigned char extendedCodeLevel,
unsigned char code,
unsigned char valueLength,
const unsigned char *value,
void *customData ) {
if( extendedCodeLevel == 0 ) {
switch( code ) {
/* [CODE]: ATTENTION eSense */
	//注意力
case( 0x04 ):
printf( "Attention Level: %d\n", value[0] & 0xFF );
break;
/* [CODE]: MEDITATION eSense */
    //冥想力
case( 0x05 ):
printf( "Meditation Level: %d\n", value[0] & 0xFF );
break;
/* Other [CODE]s */
default:
printf( "EXCODE level: %d CODE: 0x%02X vLength: %d\n",
extendedCodeLevel, code, valueLength );
printf( "Data value(s):" );
for( i=0; i<valueLength; i++ ) printf( " %02X", value[i] & 0xFF );
printf( "\n" );
}
}
}
/**
* Program which reads ThinkGear Data Values from a COM port.
*/
//从COM端口读取ThinkGear数据值的程序
int
main( int argc, char **argv ) {
/* 2) Initialize ThinkGear stream parser */
//初始化ThinkGear流解析器
ThinkGearStreamParser parser;
THINKGEAR_initParser( &parser, PARSER_TYPE_PACKETS,
handleDataValueFunc, NULL );
/* TODO: Initialize 'stream' here to read from a serial data
* stream, or whatever stream source is appropriate for your
* application. See documentation for "Serial I/O" for your
* platform for details.
*/
/*待办事项：初始化“流”，从串行数据流中读取，或者任何适合您的应用程序的流源。
有关详细信息，请参阅文档“串行输入/输出”文档。*/
FILE *stream = fopen( "COM4", "r" );
/* 3) Stuff each byte from the stream into the parser. Every time
* a Data Value is received, handleDataValueFunc() is called.
*/
/*将每个字节从流中放入解析器。每一次收到一个数据值，名为handleDataValueFunc（）。*/
unsigned char streamByte;
while( 1 ) {
fread( &streamByte, 1, stream );
THINKGEAR_parseByte( &parser, streamByte );
}
}
