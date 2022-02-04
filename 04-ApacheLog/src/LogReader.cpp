#include <string>
using namespace std;

#include "LogReader.h"

log LogReader::GetNextLog()
{
	string tmp;
	std::getline(*this, tmp);
	return log(tmp);
}

string& StripBaseURL(string& url, string base) 
{
	auto position = url.find(base);

	// Url de base présente
	if (position != string::npos)
	{
		url = url.substr(position + base.length());
	}
	return url;
}

string& TrimTrailingSlash(string& url)
{
	auto length = url.length();
	
	if (url[length - 1] == '/')
	{
		url = url.substr(0, length - 1);
	}
	
	return url;
}

ostream& operator<< (ostream& os, const datetime& date)
{
	os << date.hour << ":" << date.minute << ":" << date.second;
	return os;
}
