// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.


#include "ocr.h"

int
main()
{

	Adb NewAdb;
	arduino ArdControl;
	ArdControl.startMotor();
	while (true)
	{
		std::cout << "vez\n";
		NewAdb.seekCards([&]()
						 {
							 std::cout << "stopping motor and taking picture without flash\n";
							  ArdControl.stopMotor();
							 NewAdb.takePicture();
;

						 }, [&]()
						 {
							std::cout << "retrieving vo image and taking picture with flash\n";
							 Adb::pullFile("/sdcard/Pictures/CameraX-Image/vo.jpg",
							               "./media/noFlash");
							  NewAdb.flashPicture();
						 });
		std::cout << "pulling yah image and starting motor\n";
//		adb::pullFile("/sdcard/Pictures/CameraX-Image/yah.jpg", "./media/withFlash.jpg");
//		ardControl.startMotor();
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