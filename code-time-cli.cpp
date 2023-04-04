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

void print_code_type_time(const vector<pair<string, time_t>> &type_time) {
	format_print(10, ' ', "Type:");
	format_print(10, ' ', "Time:");
	cout << endl;
	for (auto &code_time : type_time) {
		format_print(10, ' ', code_time.first);
		format_print(10, ' ', code_time.second);
		cout << endl;
	}
}

void print_code_start_time(time_t start_time) {
	print_time(start_time);
}

class code_time_t {
	public:
		code_time_t(const string &filetype, time_t start_time, time_t end_time)
			:filetype(filetype), start_time(start_time), last_time(end_time - start_time) {}
		string filetype;
		time_t start_time;
		time_t last_time;
};

class CodeTime {
	public:
		CodeTime(const string &data_path)
			:data_path(data_path) {
				read_data();
				cal_data();
			}

		time_t get_code_time_total() const {
			return total_code_time;
		}

		vector<pair<string, time_t>> get_code_time_by_lang_type() const {
			vector<pair<string, time_t>> type_time;
			for (auto it = language_code_time.begin();it != language_code_time.end();it++)
				type_time.push_back({it->first, it->second});
			sort(type_time.begin(), type_time.end());
			return type_time;
		}

		time_t get_start_code_time() const {
			return code_start_time;
		}

	private:
		void read_data() {
			char filetype_cstr[20];
			string read_content, filetype;
			time_t start_time, end_time;
			in_file.open(data_path, ios::in);

			getline(in_file, read_content);
			sscanf(read_content.c_str(), "start: %lu %s", &start_time, filetype_cstr);
			filetype = string(filetype_cstr);
			code_start_time = start_time;

			while (getline(in_file, read_content)) {
				size_t start_loc = read_content.find("start:");
				if (start_loc == string::npos) {
					sscanf(read_content.c_str(), "end: %lu", &end_time);
					if (start_time != end_time)
						code_time.push_back(code_time_t(filetype, start_time, end_time));
					start_time = end_time;
				} else {
					time_t before_start_time = start_time;
					string before_filetype = filetype;
					sscanf(read_content.c_str(), "start: %lu %s", &start_time, filetype_cstr);
					filetype = string(filetype_cstr);
					if (before_filetype == filetype) {
						start_time = before_start_time;
						continue;
					} else {
						if (before_filetype.size() == 0)
							continue;
						if (start_time != before_start_time)
							code_time.push_back(code_time_t(before_filetype, before_start_time, start_time));
					}
				}
			}

			in_file.close();
		}

		void cal_data() {
			for (int i = 0;i < code_time.size();i++)
				total_code_time += code_time[i].last_time;
			for (int i = 0;i < code_time.size();i++)
				language_code_time[code_time[i].filetype] += code_time[i].last_time;
		}

		ifstream in_file;
		string data_path;
		time_t code_start_time;
		time_t total_code_time;
		vector<code_time_t> code_time;
		unordered_map<string, time_t> language_code_time;
};

int main(int argc, char *argv[])
{
	string home_path = getenv("HOME");
	string data_path = home_path + "/.vim/plugged/code-time/data";
	CodeTime ct(data_path);
	cout << "Start Time: ";
	print_code_start_time(ct.get_start_code_time());
	cout << endl;
	print_code_type_time(ct.get_code_time_by_lang_type());

	return 0;
}
