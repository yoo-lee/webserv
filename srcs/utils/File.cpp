#include "File.hpp"

#include <sys/stat.h>

#include <cassert>
#include <sstream>

#include "string.hpp"

namespace utils {

File::File(const std::string& absolute_path)
    : absolute_path_(absolute_path), file_type_(kNotExist) {
  if (stat(absolute_path_.c_str(), &stat_) < 0) {
    file_type_ = kNotExist;
  } else {
    SetFileType();
  }
}

File::File(const File& rhs) {
  *this = rhs;
}

File::~File() {}

File& File::operator=(File const& rhs) {
  if (this != &rhs) {
    absolute_path_ = rhs.absolute_path_;
    stat_ = rhs.stat_;
    file_type_ = rhs.file_type_;
  }
  return *this;
}

// autoindexでsortを使用したいので作成
// ディレクトリが上にきてほしい。
bool operator<(const File& lhs, const File& rhs) {
  if (lhs.GetFileType() != rhs.GetFileType()) {
    return lhs.GetFileType() < rhs.GetFileType();
  }
  return lhs.GetAbsolutePath() < rhs.GetAbsolutePath();
}

bool operator>(const File& lhs, const File& rhs) {
  return rhs < lhs;
}

bool operator<=(const File& lhs, const File& rhs) {
  return !(lhs > rhs);
}

bool operator>=(const File& lhs, const File& rhs) {
  return !(lhs < rhs);
}

void File::SetFileType() {
  if (S_ISDIR(stat_.st_mode)) {
    file_type_ = kDir;
  } else {
    file_type_ = kFile;
  }
}

std::string File::GetAbsolutePath() const {
  return absolute_path_;
}

std::string File::GetFileName() const {
  if (absolute_path_ == "/") {
    return "/";
  }

  std::vector<std::string> splited = utils::SplitString(absolute_path_, "/");

  std::string file_name;
  for (std::vector<std::string>::const_reverse_iterator it = splited.rbegin();
       it != splited.rend(); it++) {
    if (*it == "") {
      continue;
    }
    file_name = *it;
    break;
  }
  if (IsDir()) {
    return file_name + "/";
  } else {
    return file_name;
  }
}

File::FileType File::GetFileType() const {
  return file_type_;
}

bool File::IsDir() const {
  return file_type_ == kDir;
}

std::string File::GetFileSizeStr() const {
  std::stringstream ss;

  assert(file_type_ != kNotExist);

  if (file_type_ == kDir) {
    return "-";
  }
  ss << stat_.st_size;
  return ss.str();
}

std::string File::GetDateStr(const std::string fmt) const {
  char buf[256];
  struct tm* tm;

  assert(file_type_ != kNotExist);

  // tm = gmtime(&stat_.st_atime);
  tm = gmtime(&stat_.st_mtime);
  strftime(buf, 256, fmt.c_str(), tm);
  return std::string(buf);
}

}  // namespace utils
