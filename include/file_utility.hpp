#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include <byte_vector.hpp>
#include <string>
using std::string;

class FileUtility
{
  public:
    static bool is_file_exist(const string& path);
    static bool is_directory_exist(const string& path);
    static string get_cwd();
    static string read_file_text(const string& path);
    static ByteVector read_file_binary(const string& path);
    static bool delete_file(const string& path);
    static vector<string> get_entries_in_directory(const string& path);
};

#endif /* FILE_UTILITY_H */
