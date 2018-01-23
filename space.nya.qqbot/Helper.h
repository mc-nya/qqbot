#pragma once
#include<string>
#include<time.h>
class Helper
{
public:
	static std::string getDate() {
		struct tm t;
		time_t now = time(0);
		localtime_s(&t, &now);
		std::string _stime = std::to_string(t.tm_year + 1900) + "-"
			+ std::to_string(t.tm_mon + 1) + "-" + std::to_string(t.tm_mday);
		return _stime;
	}
	Helper();
	~Helper();
};

