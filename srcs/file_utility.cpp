#include "file_utility.hpp"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

#ifdef UNIT_TEST
#include "doctest.h"
#endif

using std::cout;
using std::endl;
using std::ifstream;

bool FileUtility::is_file_exist(const string& path)
{
    struct stat fileInfo;
    return (stat(path.c_str(), &fileInfo) == 0);
}

bool FileUtility::is_directory_exist(const string& path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0)
        return false;
    return (fileInfo.st_mode & S_IFDIR);
}

vector<string> FileUtility::get_entries_in_directory(const string& path)
{
    vector<string> entries;
    DIR* dir;
    struct dirent* dp;
    if ((dir = opendir(path.c_str())) == NULL) {
        perror("opendir");
        return entries;
    }
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.')
            continue;
        entries.push_back(dp->d_name);
    }

    closedir(dir);
    return entries;
}

string FileUtility::get_cwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return string(cwd);
    } else {
        perror("getcwd() error");
        return "";
    }
}

string FileUtility::read_file_text(const string& path)
{
    ifstream ifs(path.c_str());
    if (ifs.is_open() == false)
        throw std::runtime_error("read_file_text: file open error");

    string buf = "";
    string result = "";
    while (getline(ifs, buf)) {
        cout << buf << endl;
        result += buf + "\n";
    }
    std::cout << FileUtility::get_cwd() << std::endl;
    return (result);
}

ByteVector FileUtility::read_file_binary(const string& path)
{
    return ByteVector(FileUtility::read_file_text(path));
}

bool FileUtility::delete_file(const string& path)
{
    return (remove(path.c_str()) == 0);
}

#ifdef UNIT_TEST

TEST_CASE("is_file_exist")
{
    CHECK(FileUtility::is_file_exist("Makefile") == true);
    CHECK(FileUtility::is_file_exist("Makefile2") == false);
    CHECK(FileUtility::is_file_exist(FileUtility::get_cwd() + "/Makefile") == true);
    CHECK(FileUtility::is_file_exist(FileUtility::get_cwd() + "/Makefile2") == false);
}

TEST_CASE("read_file_text")
{
    CHECK(FileUtility::read_file_text("./statics/readfile1") == "");
    CHECK(FileUtility::read_file_text("./statics/readfile2") == "test\n");
    CHECK_THROWS_AS(FileUtility::read_file_text("not_found_path"), std::runtime_error);
}

TEST_CASE("read_file_bytes")
{
    CHECK(FileUtility::read_file_binary("./statics/readfile1") == ByteVector(""));
    CHECK(FileUtility::read_file_binary("./statics/readfile2") == ByteVector("test\n"));
}

#endif