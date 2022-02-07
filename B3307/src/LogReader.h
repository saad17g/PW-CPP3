#if ! defined LOGREADER_H
#define LOGREADER_H

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

const string BASE_URL = "intranet-if.insa-lyon.fr";

string& StripBaseURL(string& url, string base);
string& TrimTrailingSlash(string& url);

typedef struct datetime {
	size_t day;
	string month;
	size_t year;
	
	size_t hour;
	size_t minute;
	size_t second;

	size_t tmz;

	inline datetime()
	{
		day = 01;
		month = "Jan";
		year = 1997;

		hour = 00;
		minute = 00;
		second = 00;

		tmz = +00;
	};

	inline datetime(string src)
	{
		stringstream ss(src);
		string tmp;

		getline(ss, tmp, '/');
		day = atoi(tmp.c_str());

		getline(ss, month, '/');

		getline(ss, tmp, ':');
		year = atoi(tmp.c_str());

		getline(ss, tmp, ':');
		hour = atoi(tmp.c_str());

		getline(ss, tmp, ':');
		minute = atoi(tmp.c_str());
		
		getline(ss, tmp, ' ');
		second = atoi(tmp.c_str());

		getline(ss, tmp, '\0');
		tmz = atoi(tmp.c_str());
	};
} datetime;

ostream& operator<< (ostream& os, const datetime& date);

typedef struct log {
	string ip;
	string username;
	string authname;
	datetime date;
	string verb;
	string origin;
	string http_version;
	string ret_code;
	string download_size;	
	string target;
	string user_agent;
	
	inline log(string src)
	{
		stringstream ss(src);
		string tmp;
		
		getline(ss, ip, ' ');
		getline(ss, username, ' ');
		getline(ss, authname, ' ');

		getline(ss, tmp, '[');
		getline(ss, tmp, ']');
		date = datetime(tmp);

		getline(ss, tmp, '\"');
		getline(ss, verb, ' ');
		getline(ss, target, ' ');
		TrimTrailingSlash(target);
	
		getline(ss, http_version, '\"');

		getline(ss, tmp, ' ');
		getline(ss, ret_code, ' ');
	
		getline(ss, download_size, ' ');

		getline(ss, tmp, '\"');
		getline(ss, origin, '\"');
		TrimTrailingSlash(origin);

		getline(ss, tmp, ' ');
		getline(ss, user_agent, '\n');
	};

} log;

class LogReader : public ifstream
{

public:
	inline LogReader(string path):
		ifstream(path) {};

	log GetNextLog();	
};

#endif
