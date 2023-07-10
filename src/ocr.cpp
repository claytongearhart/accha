// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.


#include "ocr.h"

void
repl() {
  arduino ArdControl;
  Adb NewAdb;
  std::string Input;
  while (true) {
	std::cin >> Input;
	if (Input=="start") {
	  ArdControl.startMotor();

	} else if (Input=="stop")
	  ArdControl.stopMotor();
	else if(Input=="pic")
	{
	  Adb::takePicture();
	  NewAdb.seekCards([]() {}, [&]() {
		Adb::pullFile("/sdcard/Pictures/CameraX-Image/vo.jpg",
		              "./media/noFlash.jpg");
		Adb::flashPicture();
	  }, []() {
		std::cout << "pulling yah image \n";
		Adb::pullFile("/sdcard/Pictures/CameraX-Image/yah.jpg", "./media/withFlash.jpg");

	  });
	}
	else if (Input == "proc")
	{
	  processImg();
	}
	else if (Input=="q")
	  break;
  }
}

int
main() {
//
//  Adb NewAdb;
//  arduino ArdControl;
//  ArdControl.startMotor();
//  while (true) {
////    std::cout << "vez\n";
////    NewAdb.seekCards([&]() {
////	  std::cout << "stopping motor and taking picture without flash\n";
////	  ArdControl.stopMotor();
////	  Adb::takePicture();
////
////    }, [&]() {
////	  std::cout << "retrieving vo image and taking picture with flash\n";
////	  Adb::pullFile("/sdcard/Pictures/CameraX-Image/vo.jpg",
////	                "./media/noFlash");
////	  Adb::flashPicture();
////    }, []() {
////	  std::cout << "pulling yah image \n";
////
////	  Adb::pullFile("/sdcard/Pictures/CameraX-Image/yah.jpg", "./media/withFlash.jpg");
////    });
////    std::cout << "starting motor\n";
////    ArdControl.startMotor();
////    std::this_thread::sleep_for(256ms);
//  }

  repl();

  return 0;

}