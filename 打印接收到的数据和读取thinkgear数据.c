//example1：
/*简单的打印程序，接收到的数据的值到标准输出*/
#include <stdio.h>
#include "ThinkGearStreamParser.h"
/**
* 1) 函数作用于收到的value[ ]的每个字节的thinkgear DataRow.
*/
void handleDataValueFunc(unsigned char extendedCodeLevel,
	unsigned char code,
	unsigned char valueLength,
	const unsigned char *value,
	void *customData)
{
	if (extendedCodeLevel == 0)
	{
		switch (code)
		{
			/* [CODE]: ATTENTION eSense */
		case(0x04):
			printf("Attention Level: %d\n", value[0] & 0xFF);
			break;
			/* [CODE]: MEDITATION eSense */
		case(0x05):
			printf("Meditation Level: %d\n", value[0] & 0xFF);
			break;
			/* Other [CODE]s */
		default:
			printf("EXCODE level: %d CODE: 0x%02X vLength: %d\n",
				extendedCodeLevel, code, valueLength);
			printf("Data value(s):");
			for (int i = 0; i<valueLength; i++) printf(" %02X", value[i] & 0xFF);
			printf("\n");
		}
	}
}



//example2：

/*从COM端口读取thinkgear数据值。*/
int
main(int argc, char **argv) {
	/* 初始化thinkgear流分析器*/
	ThinkGearStreamParser parser;
	THINKGEAR_initParser(&parser, PARSER_TYPE_PACKETS,
		handleDataValueFunc, NULL);
	/* 这里读取的初始化数据流从串行数据流中读取，或是任何适合您的应用程序数据流.
	See documentation for "Serial I/O" for your platform for details.*/
	FILE *stream = fopen("COM4", "r");
	/* 填充每个字节从数据流到解析器.
	每一次的数据值被接收，以handledatavaluefunc()命名*/
	unsigned char streamByte;
	while (1) {
		fread(&streamByte, 1, stream);
		THINKGEAR_parseByte(&parser, streamByte);
	}
}
