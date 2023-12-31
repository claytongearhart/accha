// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/3/23.
//

#include "phyControl.hxx"

bool
isNumber(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it!=s.end() && (std::isdigit(*it)) | *it=='.')
	++it;
  return !s.empty() && it==s.end();
}

std::string
exec(const char *cmd) {
  std::array<char, 128> Buffer;
  std::string Result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
	throw std::runtime_error("popen() failed!");
  }
  while (fgets(Buffer.data(), Buffer.size(), pipe.get())!=nullptr) {
	Result += Buffer.data();
  }
  return Result;
}

void
Adb::seekCards(const std::function<void(void)> &OnCard, const std::function<void(void)> &OnVoSave, const
std::function<void(void)> &OnYahSave) {
  double Mean, SqSum, Stdev;
  std::string CurrVal;
  bool WaitingForVo, WaitingForYah = false;
  bool VoFin, YahFin = false;
  while (getline(&logcatLine, &lcLen, fp)!=-1) {
	if (logcatLine[0]!='-') {
	  std::string LcLine(logcatLine);
	  auto const DelimPos = LcLine.find_last_of(' ');
	  CurrVal = LcLine.substr(DelimPos + 1);
	  CurrVal.erase(std::remove_if(CurrVal.begin(), CurrVal.end(), isspace));
//	  if (WaitingForVo || WaitingForYah) {
	  if (true) // set to true for man
		std::cout << "curr val: " << CurrVal << '\n';
	  if (CurrVal=="vo" && true) {
//		std::this_thread::sleep_for(128ms);
		OnVoSave();
		VoFin = true;
	  } else if (CurrVal=="yah" && true) {
//		std::this_thread::sleep_for(128ms);
		OnYahSave();
		YahFin = true;
	  } else if (VoFin && YahFin)
		return;
	} else {
	  if (lumValues.size() >= 1000) {
		lumValues.pop_back();
	  }
	  if (isNumber(CurrVal)) {
		double DCurrVal = std::stod(CurrVal);
		lumValues.push_front(DCurrVal);

		Mean = std::accumulate(lumValues.begin(), lumValues.end(), 0.0)/lumValues.size();
		std::vector<double> Diff(lumValues.size());

		std::transform(lumValues.begin(),
		               lumValues.end(),
		               Diff.begin(),
		               [Mean](double x) { return x - Mean; });
		SqSum = std::inner_product(Diff.begin(), Diff.end(), Diff.begin(), 0.0);
		Stdev = std::sqrt(SqSum/lumValues.size());

		if (lumValues.size() > 100 & (DCurrVal > Mean + 1.5*Stdev)) {
		  std::cout << "card detected\n";
		  WaitingForVo = WaitingForYah = true;
		  OnCard();
		  std::cout << "awaitng vo save\n";

		} else {

		}
	  }
	}
  }

}


void
Adb::flashPicture() {
  system("adb shell input keyevent 30");
}
void
Adb::takePicture() {
  system("adb shell input keyevent 29");

}
void
Adb::pullFile(const std::string &SrcName, const std::string &DstName) {
  std::system(std::string("adb pull " + SrcName + " " + DstName).c_str());
  std::system(std::string("adb shell rm " + SrcName).c_str());
}
void
arduino::startMotor() const {
  char Command[] = {'M', 'E'};
  for (char &I : Command) {
	write(tty_fd, &I, 1);
  }
}
void
arduino::stopMotor() const {
  char Command[] = {'M', 'D'};
  for (char &I : Command) {
	write(tty_fd, &I, 1);
  }
}
