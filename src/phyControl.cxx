// Copyright 2023 Clayton Gearhart
// This software is licensed under the GNU Lesser General Public License v3.0. See LICENSE for details.

//
// Created by Clayton Gearhart on 7/3/23.
//

#include "phyControl.hxx"

bool is_number(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && (std::isdigit(*it)) | *it == '.') ++it;
  return !s.empty() && it == s.end();
}

std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
	throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
	result += buffer.data();
  }
  return result;
}

void adb::seekCards(const std::function<void(void)>& vo) {
  double mean, sq_sum, stdev;
  std::string currVal;
  while (getline(&logcatLine, &lcLen, fp) != -1) {
	if (logcatLine[0] != '-') {
	  std::string lcLine(logcatLine);
	  auto const delimPos = lcLine.find_last_of(' ');
	  currVal = lcLine.substr(delimPos + 1);
	  currVal.erase(std::remove_if(currVal.begin(), currVal.end(), isspace));
	  if (currVal == "vo")
	  {
		vo();
		return;
	  }
	  else if (currVal == "yah")
	  {
		return;
	  }
	  if (lumValues.size() >= 1000) {
		lumValues.pop_back();
	  }
	  if (is_number(currVal)) {
		double dCurrVal = std::stod(currVal);
		lumValues.push_front(dCurrVal);

		mean = std::accumulate(lumValues.begin(), lumValues.end(), 0.0) / lumValues.size();
		std::vector<double> diff(lumValues.size());

		std::transform(lumValues.begin(), lumValues.end(), diff.begin(), [mean](double x) { return x - mean; });
		sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		stdev = std::sqrt(sq_sum / lumValues.size());


		if (lumValues.size() > 100 & (dCurrVal < mean - stdev || dCurrVal > mean + stdev)) {
		  return;
		}
	  }
	}


  }
}
