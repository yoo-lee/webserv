#include "uri.hpp"
#include "request.hpp"
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include <map>

using std::string;

URI::URI()
{
}
URI::~URI()
{
}

URI::URI(const URI& uri)
{
    *this = uri;
}

URI& URI::operator=(URI const& uri)
{
    if (this == &uri){
        return (*this);
    }
    this->_filename = uri._filename;
    this->_root = uri._root;
    this->_filepath = uri._filepath;
    this->_location_path = uri._location_path;
    this->_query = uri._query;
    this->_path_info = uri._path_info;
    return (*this);
}

static std::string get_root(std::map<std::string, std::vector<std::string> >& props);

URI::URI(Request *req)
{
    string path = req->get_path();
    this->retrieve_query(path);

    const Config* cfg = req->get_config();
    std::map<std::string, std::vector<std::string> > props =
        cfg->get_locations_properties(req->get_port(), req->get_host(), this->_location_path);

    this->_root = get_root(props);
    this->remove_file_info(_root + "/" +  this->_location_path);
    this->print_uri();
}

/*
URI::URI(Request &req)
{
    string path = req.get_path();
    this->retrieve_query(path);

    const Config* cfg = req.get_config();
    std::map<std::string, std::vector<std::string> > props =
        cfg->get_locations_properties(req.get_port(), req.get_host(), this->_location_path);

    this->_root = get_root(props);
    this->remove_file_info(_root + "/" +  this->_location_path);
    this->print_uri();
}
*/

void URI::retrieve_query(string& uri)
{
    size_t pos = uri.find("?");
    string path;
    string query;
    if (pos != std::string::npos) {
        this->_query = uri.substr(pos + 1);
        path = uri.substr(0, pos);
    } else {
        this->_query = "";
        path = uri;
    }
    this->_location_path = Utility::delete_duplicated_slash(path);
}

static std::string get_root(std::map<std::string, std::vector<std::string> >& props)
{
    std::map<std::string, std::vector<std::string> >::iterator ite = props.find("root");
    const char* tmp_path;
    struct stat tmp_stat;
    if (ite != props.end()) {
        for (size_t i = 0; i < props["root"].size(); i++) {
            tmp_path = (props["root"][i]).c_str();
            stat(tmp_path, &tmp_stat);
            if (S_ISDIR(tmp_stat.st_mode)) {
                return (Utility::delete_duplicated_slash(tmp_path) + "/");
            } else {
                continue;
            }
        }
    }
    throw std::invalid_argument("Invalid URI");
}

void URI::print_uri()
{
    cout << endl << "print URI" << endl;
    cout << "_filename:[" << _filename << "]" << endl;
    cout << "_root:[" << _root << "]" << endl;
    cout << "_filepath:[" << _filepath << "]" << endl;
    cout << "_location_path:[" << _location_path << "]" << endl;
    cout << "_query:[" << _query << "]" << endl;
    cout << "_path_info:[" << _path_info << "]" << endl << endl;
}


void URI::remove_file_info(std::string tmp_file_path)
{
    std::string path = Utility::delete_duplicated_slash(tmp_file_path);
    size_t pos = path.rfind("/");
    string path_info;
    struct stat tmp_stat;
    bool exist_flag = false;
    while (pos != std::string::npos && pos != 0) {
        path_info = path.substr(pos) + "/" + path_info;
        path = path.substr(0, pos);
        int rval = stat(path.c_str(), &tmp_stat);
        if (rval == 0 && S_ISREG(tmp_stat.st_mode)) {
            exist_flag = true;
            break;
        }
        pos = path.rfind("/", pos - 1);
    }
    if (!exist_flag){
        throw std::invalid_argument("Invalid URI");
    }
    this->_path_info = Utility::delete_duplicated_slash(path_info);
    if (this->_root.size() < path.size()){
        this->_filepath = _root + path.substr(this->_root.size());
    }else{
        throw std::invalid_argument("Invalid URI");
    }

    pos = this->_filepath.rfind('/');
    if (pos != std::string::npos){
        this->_filename = this->_filepath.substr(pos+1);
    }else{
        throw std::invalid_argument("Invalid URI");
    }
}
