#ifndef GLOBALS_H
#define GLOBALS_H

#define VSYNC_ON true

#define ROOM_WIDTH 832
#define ROOM_HEIGHT 600

#define VIEW_WIDTH ROOM_WIDTH
#define VIEW_HEIGHT ROOM_HEIGHT

#define BAR_HEIGHT 88

#define UDP_PORT 5050
#define TCP_PORT 5060
#define SERVER "192.168.1.87"

//#define _DEBUG

#ifdef _DEBUG
	#define DEBUG_MODE
#endif

#endif