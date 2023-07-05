// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/3/23.
//

#ifndef DROIDCONTROL__PHYCONTROL_HXX_
#define DROIDCONTROL__PHYCONTROL_HXX_

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <iostream>
#include <numeric>
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
#include <vector>

class arduino {
 private:
  struct termios tio;
  struct termios stdio;
  int tty_fd;

 public:
  arduino() : tio(), stdio(), tty_fd() {

	memset(&tio, 0, sizeof(tio));
	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_cflag = CS8 | CREAD | CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

	tty_fd = open("/dev/cu.usbmodem1601", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio, B4800);            // 4800 baud
	cfsetispeed(&tio, B4800);            // 4800 baud

	tcsetattr(tty_fd, TCSANOW, &tio);
  }

  ~arduino() {
	close(tty_fd);
  }

  void startMotor() {
	char command[] = {'M', 'E'};
	for (char &i : command) {
	  write(tty_fd, &i, 1);
	}
  }

  void stopMotor() {
	char command[] = {'M', 'D'};
	for (char &i : command) {
	  write(tty_fd, &i, 1);
	}
  }
};

class adb {
  bool isOverNoFlash;
  std::deque<double> lumValues;
  FILE *fp;
  char *logcatLine = nullptr;
  size_t lcLen;

 public:
  adb() : isOverNoFlash(false), lumValues(), lcLen(0) {
	fp = popen("adb logcat -s CameraXApp", "r");
  }

  void seekCards(const std::function<void(void)>& vo);

  static void pullFile(const std::string &srcName, const std::string &dstName = "image.jpg") {
	std::system(std::string("adb pull " + srcName + " " + dstName).c_str());
	std::system(std::string("adb shell rm " + srcName ).c_str());
  }

  void takePicture() const { clickButton(true); }
  void flashPicture() const {
	clickButton(false);
	clickButton(true);
  }

  void clickButton(bool overPicture) const {
	if (overPicture == isOverNoFlash) {
	  system("adb shell input keyevent 66");
	} else {
	  system("adb shell input keyevent 61");
	  system("adb shell input keyevent 66");

	}
  }

 private:

};

#endif //DROIDCONTROL__PHYCONTROL_HXX_
