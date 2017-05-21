#include "mpi.h"
#include <omp.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;

int number_of_lines(string filename);
string filename_max_lines(string folder);
string findCommentID(string comment_id);
bool existCommentID(string filename, string comment_id);
int write_line(string prefix, string name, string line);
vector<string> explode(const string& str, const char& ch);

int main()
{
    cout << "Starting program..." << endl;

    // <ts, comment_id, user_id, comment, user, comment_replied, post_commented>
    cout << "Starting first phase of the program..." << endl;

    std::ifstream infile("comments_simple.dat");
    std::string line;

    while (std::getline(infile, line))
    {
        vector<string> line_vector = explode(line, '|');

        if (line_vector.size() == 7) {
            write_line("reply_posts/rp_", line_vector[6] + ".dat", line);
        } else {
            write_line("reply_comments/rc_", "all.dat", line);
        }
    }


    cout << "Starting second phase of the program..." << endl;

    std::ifstream infile_rc_all("./reply_comments/rc_all.dat");
    int line_number = 0;
    while (std::getline(infile_rc_all, line))
    {
        line_number++;
        vector<string> line_vector = explode(line, '|');

        string filename_loc = findCommentID(line_vector[5]);

        if (filename_loc.compare("") == 0) {
            write_line("reply_comments/rc_", "all_missing.dat", line);
        } else {
            write_line("", filename_loc, line);
            write_line("reply_comments/rc_", "all_processed.dat", line);
        }

        cout << line_number << endl;
    }


    cout << "Starting third phase of the program..." << endl;

    cout << "Max Post: " << filename_max_lines("./reply_posts/") << endl;



    int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int n = 10;
	int i = 0;
	int a = 0;

	// #pragma omp parallel num_threads(4)
	// {
	// 	cout << sum(num1,num2) << endl;
	// }

	// #pragma omp parallel for private(i,a)
	// for (i=0; i<n; i++)
	// {
	// 	a = i+1;
	// 	printf("Thread %d has a value of a = %d for i = %d\n", omp_get_thread_num(),a,i);
	// } /*-- End of parallel for --*/
	int res1 = 0;
	int res2 = 0;
	int res3 = 0;
	int res4 = 0;

	for (i=0; i<2; i++) {
		// cout << sum(i,n) << endl;
		// sum(num1,num2)
		#pragma omp parallel num_threads(4)
		{
			switch (omp_get_thread_num()) {
				case 0:
					res1 = num1 + i;
				break;
				case 1:
					res2 = num2 + i;
				break;
				case 2:
					res3 = num3 + i;
				break;
				case 3:
					res4 = num4 + i;
				break;
			}
		}
		cout << "res1: " << res1 << endl;
		cout << "res2: " << res2 << endl;
		cout << "res3: " << res3 << endl;
		cout << "res4: " << res4 << endl;
	}
    return 0;
}


vector<string> explode(const string& str, const char& ch) {
    string next;
    vector<string> result;

    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ch) {
                result.push_back(next);
                next.clear();
        } else {
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}


int write_line(string prefix, string name, string line) {
    string filename = prefix + name;
    std::ofstream o_file(filename.c_str(), std::ios_base::app | std::ios_base::out);
    o_file << line + "\n";
    o_file.close();
}

bool existCommentID(string filename, string comment_id) {
    std::ifstream infile(filename.c_str());
    std::string line;
    while (std::getline(infile, line))
    {
        vector<string> line_vector = explode(line, '|');
        if (comment_id.compare(line_vector[1]) == 0)
            return true;
    }

    return false;
}

string findCommentID(string comment_id) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("./reply_posts/")) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        string folder = "./reply_posts/";
        string filename = folder + ent->d_name;
        if (existCommentID(filename, comment_id))
            return filename;
      }
      closedir (dir);
    } else {
      perror ("");
      return "";
    }
    return "";
}


string filename_max_lines(string folder) {
    string maxlines_filename = "";
    int maxlines_number = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (folder.c_str())) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        string filename = folder + ent->d_name;

        int lines_number = number_of_lines(filename);

        if (lines_number > maxlines_number) {
            maxlines_number = lines_number;
            maxlines_filename = filename;
        }
      }

      closedir (dir);
    } else {
      perror ("");
      return "";
    }
    return maxlines_filename;
}

int number_of_lines(string filename) {
    std::ifstream infile(filename.c_str());
    int line_number = 0;
    string line;
    while (std::getline(infile, line)) line_number++;

    return line_number;
}
