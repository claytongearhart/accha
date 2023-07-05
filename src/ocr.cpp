// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.


#include "ocr.h"

int main() {

  adb newAdb;

  while (true) {
	std::cout << "vez\n";
	newAdb.takePicture();
	newAdb.seekCards([&]() {
	  newAdb.flashPicture();
	  adb::pullFile("/sdcard/Pictures/CameraX-Image/vo.jpg",
					"./media/noFlash");
	});

	adb::pullFile("/sdcard/Pictures/CameraX-Image/yah.jpg", "./media/withFlash.jpg");
  }

//	arduino ardControl;
//	std::string input;
//	while (true)
//	{
//	  std::cin >> input;
//	  if (input == "start") ardControl.startMotor();
//	  else if (input == "stop") ardControl.stopMotor();
//	  else if (input == "q") break;
//	}

  return 0;
}