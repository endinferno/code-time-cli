#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

	template <typename T>
void format_print(int width, char fill, T val)
{
	cout << setw(width) << setfill(fill) << val;
}

void format_print_time(time_t t)
{
	bool start_print = false;
	int tm_sec = t % 60;
	int tm_min = (t / 60) % 60;
	int tm_hour = (t / 3600) % 24;
	int tm_mday = t / 86400;

	if (tm_mday != 0) {
		format_print(2, '0', tm_mday);
		cout << " ";
		format_print(2, '0', tm_hour);
		cout << ":";
		format_print(2, '0', tm_min);
		cout << ":";
		format_print(2, '0', tm_sec);
	} else if (tm_hour != 0) {
		format_print(2, '0', tm_hour);
		cout << ":";
		format_print(2, '0', tm_min);
		cout << ":";
		format_print(2, '0', tm_sec);
	} else if (tm_min != 0) {
		format_print(2, '0', tm_min);
		cout << ":";
		format_print(2, '0', tm_sec);
	} else if (tm_sec != 0) {
		format_print(2, '0', tm_sec);
	}
}

void print_time(time_t sec_time)
{
	struct tm *curr_time = localtime(&sec_time);
	format_print(4, '0', 1900 + curr_time->tm_year);
	cout << "-";
	format_print(2, '0', 1 + curr_time->tm_mon);
	cout << "-";
	format_print(2, '0', curr_time->tm_mday);
	cout << " ";
	format_print(2, '0', curr_time->tm_hour);
	cout << ":";
	format_print(2, '0', curr_time->tm_min);
	cout << ":";
	format_print(2, '0', curr_time->tm_sec);
}

int main(int argc, char *argv[])
{
	string str;
	ifstream in_file;
	char filetype_cstr[10];
	string filetype;
	time_t start_time, end_time;
	time_t startup_time;
	string home_path = getenv("HOME");
	string data_path = home_path + "/.vim/plugged/code-time/data";
	in_file.open(data_path, ios::in);
	getline(in_file, str);
	sscanf(str.c_str(), "start: %lu %s", &start_time, filetype_cstr);
	filetype = string(filetype_cstr);
	startup_time = start_time;

	unordered_map<string, time_t> file_edit;
	while (getline(in_file, str))
	{
		size_t start_loc = str.find("start:");
		if (start_loc == string::npos) {
			sscanf(str.c_str(), "end: %lu", &end_time);
			file_edit[filetype] += (end_time - start_time);
			start_time = end_time;
		} else {
			time_t before_start_time = start_time;
			string before_filetype = filetype;
			sscanf(str.c_str(), "start: %lu %s", &start_time, filetype_cstr);
			filetype = string(filetype_cstr);
			if (before_filetype == filetype) {
				start_time = before_start_time;
				continue;
			} else {
				if (before_filetype.size() == 0)
					continue;
				file_edit[before_filetype] += (start_time - before_start_time);
			}
		}
	}
	in_file.close();
	cout << "Start Time: ";
	print_time(startup_time);
	cout << endl;
	format_print(10, ' ', "Type:");
	format_print(10, ' ', "Time:");
	cout << endl;
	for (auto it = file_edit.begin();it != file_edit.end();it++) {
		format_print(10, ' ', it->first);
		format_print(10, ' ', it->second);
		//format_print_time(it->second);
		cout << endl;
	}
	return 0;
}
