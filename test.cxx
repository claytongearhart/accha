// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/5/23.
//
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>
#include <fstream>
int main()
{
  "ADB STREAM READ";

  FILE* fp;
  char* line = NULL;

  size_t len = 0;
  std::fstream fs;
  fp = popen("adb logcat -s CameraXApp", "r");


  while(getline(&line, &len, fp) != -1) {
	fs << line;
	free(line);
	line = NULL;
  }

"ARD COMM TEST";
//
//  struct termios tio;
//  struct termios stdio;
//  int tty_fd;

//  unsigned char c='D';
//
//  memset(&stdio,0,sizeof(stdio));
//  stdio.c_iflag=0;
//  stdio.c_oflag=0;
//  stdio.c_cflag=0;
//  stdio.c_lflag=0;
//  stdio.c_cc[VMIN]=1;
//  stdio.c_cc[VTIME]=0;
//  tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
//  tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
//  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
//
//  memset(&tio,0,sizeof(tio));
//  tio.c_iflag=0;
//  tio.c_oflag=0;
//  tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
//  tio.c_lflag=0;
//  tio.c_cc[VMIN]=1;
//  tio.c_cc[VTIME]=5;
//
//  tty_fd=open("/dev/cu.usbmodem1601", O_RDWR | O_NONBLOCK);
//  cfsetospeed(&tio,B4800);            // 115200 baud
//  cfsetispeed(&tio,B4800);            // 115200 baud
//
//  tcsetattr(tty_fd,TCSANOW,&tio);
//
//  //char str[] = {'V','E','R','\r'};
//  //the above str[] doesn't work although it's exactly the same as the following
////  auto str = "MT";
////  write(tty_fd,str,sizeof (str));
////  if (read(tty_fd,&c,1)>0)
////	write(STDOUT_FILENO,&c,1);
//
//  while (true)
//  {
////	if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1); // if new data is available on the serial port, print it out
////	if (read(STDIN_FILENO,&c,1)>0)
//	std::string inStr;
//	std::cin >> inStr;
//	if(inStr == "q")
//	{
//	  break;
//	}
//	auto cStr = inStr.c_str();
//	for (int i = 0; i < strlen(cStr); i++)
//	{
//	  write(tty_fd,&cStr[i],1);
//	}
////	write(tty_fd, inStr.c_str(), strlen(inStr.c_str()));
////	  if(c!='q')
////		write(tty_fd,&c,1);        // if new data is available on the console, send it to the serial port
//  }
//
//  close(tty_fd);
}
