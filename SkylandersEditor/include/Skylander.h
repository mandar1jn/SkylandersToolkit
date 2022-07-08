#pragma once

#include <string>

class Skylander {

	const static int SKYLANDER_SIZE = 1024;

	unsigned char data[SKYLANDER_SIZE];

	Skylander();

	unsigned char* GetData();

};

//How to read all data:
//for (int i = 0; i < 64; i++)
//{
//	RWCommand command = RWCommand();
//
//	command.writeBuffer[1] = 'Q';
//	command.writeBuffer[2] = 0x10;
//	command.writeBuffer[3] = i;
//
//	do
//	{
//		portal->Write(&command);
//	} while (portal->CheckResponse(&command, 'Q'));
//
//	printf("Block: %i/%X (%X %X) ", i, i, command.readBuffer[1], command.readBuffer[2]);
//
//	for (int j = 0; j < 16; j++)
//	{
//		printf("%X ", command.readBuffer[j + 3]);
//	}
//
//	printf("\n");
//}