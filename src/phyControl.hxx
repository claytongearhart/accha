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
#include <chrono>
#include <thread>
#include <stdexcept>
#include <string>
#include <array>
#include <iostream>
#include <numeric>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <cerrno>      // Error number definitions
#include <termios.h>
#include <fstream>
#include <vector>

using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

class arduino
{
 private:
	struct termios tio;
	struct termios stdio;
	int tty_fd;

 public:
	arduino() : tio(), stdio(), tty_fd()
	{

		memset(&tio, 0, sizeof(tio));
		tio.c_iflag = 0;
		tio.c_oflag = 0;
		tio.c_cflag = CS8 | CREAD | CLOCAL;           // 8n1, see termios.h for more information
		tio.c_lflag = 0;
		tio.c_cc[VMIN] = 1;
		tio.c_cc[VTIME] = 5;

		tty_fd = open("/dev/cu.usbmodem1601", O_RDWR | O_NONBLOCK);
		cfsetospeed(&tio, B4800);
		cfsetispeed(&tio, B4800);  // I haven't setup input yet but I might add sensors later so why not

		tcsetattr(tty_fd, TCSANOW, &tio);
	}

	~arduino()
	{
		close(tty_fd);
	}

	void
	startMotor() const;

	void
	stopMotor() const;
};

class Adb
{
	bool cardLeftView;
	std::deque<double> lumValues;
	FILE* fp;
	char* logcatLine = nullptr;
	size_t lcLen;

 public:
	Adb() : cardLeftView(true), lumValues(), lcLen(0)
	{
		fp = popen("adb logcat -s CameraXApp", "r");
	}

	void
	seekCards(const std::function<void(void)>& OnCard, const std::function<void(void)>& OnVoSave,
			  const std::function<void(void)> &OnYahSave);

	static void
	pullFile(const std::string& SrcName, const std::string& DstName = "image.jpg");

	static void
	takePicture();
	static void
	flashPicture();


 private:

};

#endif //DROIDCONTROL__PHYCONTROL_HXX_
