#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "ATI_Wireless.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <windows.h>
#include <winbase.h>
#include <sys/types.h>
#include <sys/stat.h>
//#pragma comment(lib,"ws2_32.lib") 

#ifdef WIN32
#pragma warning (disable: 4204) 
#include <conio.h>   // For _kbhit()
#include <cstdio>    // For getchar()
#include <windows.h> // For Sleep()
#else //linux
#define __cdecl __attribute__((__cdecl__))
#endif // WIN32

using namespace std;

namespace Sensor
{
	int readInt(char *buffer, int &index, bool little)
	{
		static char ucval[sizeof(int)];
		static int x;
		static int returnMe;

		for (x = 0; x < sizeof(int); ++x)
		{
			ucval[x] = (little ? buffer[index + (sizeof(int) - (x + 1))] : buffer[index + x]);
		}
		index += sizeof(int);
		memcpy(&returnMe, ucval, sizeof(int));
		return returnMe;
	}

	unsigned short crcByte(unsigned short crc, unsigned char ch) // lookup table version
	{
		static const unsigned short ccitt_crc16_table[256] =
		{
			0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
			0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
			0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
			0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
			0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
			0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
			0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
			0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
			0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
			0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
			0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
			0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
			0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
			0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
			0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
			0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
			0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
			0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
			0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
			0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
			0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
			0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
			0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
			0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
			0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
			0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
			0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
			0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
			0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
			0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
			0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
			0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
		};
		return ccitt_crc16_table[((crc >> 8) ^ ch) & 0xff] ^ (crc << 8);
	}

	#define CRC_INIT 0x1234 // this is the seed value used for along with the buffer’s first byte
	unsigned short crcBuf(char * buff, unsigned long len)
	{
		unsigned long i;
		unsigned short crc = CRC_INIT;
		const char * buf = buff;
		for (i = 0; i < len; i++)
		{
			crc = crcByte(crc, buf[i]);
		}
		return crc;
	}

	LIBRARY_API ATI_Wireless::ATI_Wireless(char * ipAddress,long LocalPort)
	{
		//To connect to the sensor, a local UDP port must be bounded, and then an initialization command to stream must be sent to the sensor.
		//The initialization command to stream must be sent from the local UDP port to a fixed destination port (49152) of the sensor's IP address.
		//The sensor will then send force/torque data to the local UDP port, which will then be sampled by this library.

		char* intf = NULL; //Tell ULAPI to bind to non-remote address for listening
		long DestinationPort = 49152; //Fixed destination port the ATI sensor is listening on
		const int BufLen = 10; //Length of intialization command
		char SendBuf[BufLen]; //Buffer for initialization command
		unsigned short crc_bytes; //Initialization of a Cyclic Redundancy Check (CRC) packet that consists of 2 bytes at the end of the initialization command.
		struct sockaddr_in dest_addr;
		//ULAPI doesn't sendto from a UDP port, so we do that part ourselves
		std::memset(&dest_addr, 0, sizeof(struct sockaddr_in));
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(DestinationPort);
		dest_addr.sin_addr.s_addr = inet_addr(ipAddress);

		//Set the byte values of the initialization commands (can be looked up in the manual)
		SendBuf[0] = '\x00'; SendBuf[1] = '\x0a'; SendBuf[2] = '\x00'; SendBuf[3] = '\x01'; SendBuf[4] = '\x00'; SendBuf[5] = '\x00'; SendBuf[6] = '\x00'; SendBuf[7] = '\x00';
		crc_bytes = crcBuf(SendBuf, BufLen - 2); //compute CRC bytes
		SendBuf[8] = crc_bytes >> 8; SendBuf[9] = crc_bytes & 0xff; //set the last 2 bytes of the initialization command to the value of the CRC bytes

		ATI_ID = ulapi_socket_get_broadcastee_id_on_interface(LocalPort, intf); //Establish listening connection
		sendto(ATI_ID, SendBuf, BufLen, 0, (SOCKADDR *)& dest_addr, sizeof(dest_addr)); //send initialization command
		
	}

	LIBRARY_API ATI_Wireless::~ATI_Wireless()
	{
		closesocket(ATI_ID);
	}


	LIBRARY_API void ATI_Wireless::GetATIForces(robotPose &forces)
	{
		int iResult;
		const int buff_length = 42; //Length of data buffer sent by the sensor to the local UDP port
		char buff[buff_length];
		bool little = false; //Endian-ness check
		int i = 0x01234567;
		char* c = (char*)&i;
		little = (c[0] == 'g');

		iResult = recv(ATI_ID, buff, buff_length, 0); //receive data from the local UDP port

		int ind = 18; //There's stuff like timestamps that we're not interested in, so we start at byte index 18 to parse the data
		double Fx = ((double)readInt(buff, ind, little))*1e-6;
		double Fy = ((double)readInt(buff, ind, little))*1e-6;
		double Fz = ((double)readInt(buff, ind, little))*1e-6;
		double Tx = ((double)readInt(buff, ind, little))*1e-6;
		double Ty = ((double)readInt(buff, ind, little))*1e-6;
		double Tz = ((double)readInt(buff, ind, little))*1e-6;

		forces.x = Fx - Offset.x;		 forces.y = Fy - Offset.y;			forces.z = Fz - Offset.z;
		forces.xrot = Tx - Offset.xrot;	 forces.yrot = Ty - Offset.yrot;	forces.zrot = Tz - Offset.zrot;

	}

	LIBRARY_API void ATI_Wireless::ZeroATI()
	{
		robotPose OffsetTemp, temp, sum;
		Offset = OffsetTemp; //Default the offset back to 0 in case this isn't the first time zero-ing the sensor
		const int N = 200; //Number of points for averaging
		for (int i = 0; i < N; i++) //Take the sum of N iterations of samples
		{
			GetATIForces(temp);
			sum.x = sum.x + temp.x;				sum.y = sum.y + temp.y;				sum.z = sum.z + temp.z;
			sum.xrot = sum.xrot + temp.xrot;	sum.yrot = sum.yrot + temp.yrot;	sum.zrot = sum.zrot + temp.zrot;
		}

		//Assign the average as the offset values
		Offset.x = sum.x / N;			Offset.y = sum.y / N;			Offset.z = sum.z / N;
		Offset.xrot = sum.xrot / N;		Offset.yrot = sum.yrot / N;		Offset.zrot = sum.zrot / N;

	}


}