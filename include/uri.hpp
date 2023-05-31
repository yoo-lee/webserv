#ifndef URI_HPP
#define URI_HPP
#include <string>
#include <vector>

class Request;

class URI
{
  public:
    URI();
    URI(Request* req);
    URI(const URI& uri);
    URI& operator=(const URI& uri);
    ~URI();
    void print_uri() const;
    const std::string& get_uri() const;
    const std::string& get_filename() const;
    const std::string& get_root() const;
    const std::string& get_filepath() const;
    const std::string& get_location_path() const;
    const std::string& get_query() const;
    const std::string& get_path_info() const;

  private:
    std::string _uri;
    std::string _filename;
    std::string _root;
    std::string _filepath;
    std::string _location_path;
    std::string _query;
    std::string _path_info;

    void retrieve_query(std::string& uri);
    void remove_file_info(std::string tmp_file_path);
};
#endif
