#include "WebServer.hpp"

std::string ft_itos(int nbr)
{
    std::stringstream out;

    out << nbr;
    return out.str();
}

size_t ft_stost(std::string nbr)
{
    std::stringstream ss(nbr);
    size_t value;

    ss >> value;
    return value;
}

void printConfig(const std::vector<ServerConfig> &servers)
{
    for (size_t i = 0; i < servers.size(); ++i)
    {
        const ServerConfig &server = servers[i];
        std::cout << "Server " << i + 1 << ":\n";
        // std::cout << "  Listen: " << server.listen << "\n";
        std::cout << "  Server Name: " << server.server_name << "\n";
        std::cout << "  Host: " << server.host << "\n";
        std::cout << "  Root: " << server.root << "\n";
        // std::cout << "  Index: " << server.index << "\n";

        // Iterate through error pages
        for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
        {
            std::cout << "  Error Page " << it->first << ": " << it->second << "\n";
        }

        // Iterate through locations
        for (size_t j = 0; j < server.locations.size(); ++j)
        {
            const Location &loc = server.locations[j];
            std::cout << "  Location " << j + 1 << ":\n";
            std::cout << "    Path: " << loc.path << "\n";
            std::cout << "    Autoindex: " << (loc.autoindex ? "on" : "off") << "\n";
            // std::cout << "    Index: " << loc.index << "\n";
            // std::cout << "    Return: " << loc.return_path << "\n";

            // Iterate through allow_methods map
            for (std::map<std::string, bool>::const_iterator method_it = loc.allow_methods.begin(); method_it != loc.allow_methods.end(); ++method_it)
            {
                std::cout << "    Allow Method: " << method_it->first << " is " << (method_it->second ? "enabled" : "disabled") << "\n";
            }

            // Iterate through CGI paths
            for (size_t k = 0; k < loc.cgi_path.size(); ++k)
            {
                std::cout << "    CGI Path: " << loc.cgi_path[k] << "\n";
            }

            // Iterate through CGI extensions
            for (size_t k = 0; k < loc.cgi_ext.size(); ++k)
            {
                std::cout << "    CGI Ext: " << loc.cgi_ext[k] << "\n";
            }
        }
    }
}

// Function to generate the current date and time in HTTP format
std::string getCurrentTime()
{
    std::time_t now = std::time(0);
    std::tm *gmtm = std::gmtime(&now);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtm);
    return std::string(buf);
}

std::map<int, std::string> initStatus()
{
    std::map<int, std::string> status;

    status[200] = "OK";
    status[201] = "Created";
    status[301] = "Moved Permanently";
    status[307] = "Temporary Redirect";
    status[400] = "Bad Request";
    status[403] = "ForBidden";
    status[404] = "Not Found";
    status[405] = "Method Not Allowed";
    status[413] = "Content Too Large";
    status[422] = "Unprocessable Content";
    status[500] = "Internal Server Error";
    status[502] = "Bad Gateway";
    return status;
}

bool isDirectory(const std::string &path)
{
    struct stat info;
    // stat() returns 0 on success
    if (stat(path.c_str(), &info) != 0)
        return false; // Cannot access path
    else if (info.st_mode & S_IFDIR)
        return true; // Path is a directory
    else
        return false; // Path exists, but it's not a directory
}

bool isExists(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    else
        return true;
}

bool isReadable(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IRUSR)
        return true;
    else
        return false;
}

std::string filterSlashes(std::string path)
{
    std::string result;
    bool lastSlash = false;

    for (size_t i = 0; i < path.length(); ++i)
    {
        if (path[i] == '/')
        {
            if (!lastSlash)
            {
                result += path[i];
            }
            lastSlash = true;
        }
        else
        {
            result += path[i];
            lastSlash = false;
        }
    }
    return result;
}

bool isNumber(std::string &str)
{
    if (str.empty())
    {
        return false;
    }
    std::size_t start = 0;
    if (str[0] == '+' || str[0] == '-')
    {
        return false;
    }
    for (std::size_t i = start; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

std::string List_file(std::string path) {
    // const char* path = "."; // directory ที่ต้องการ list (ในที่นี้คือ directory ปัจจุบัน)

    // เปิด directory
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n<html lang=\"en\">\n<body>\n<div class=\"header\">\n \
    <h1>File Explorer</h1>\n";
    DIR* dir = opendir(path.c_str());

    // ตรวจสอบว่า directory ถูกเปิดสำเร็จหรือไม่
    if (dir == NULL) {
        std::cerr << "Error: Could not open directory." << std::endl;
        return NULL;
    }

    struct dirent* entry;

    // อ่านเนื้อหาใน directory ทีละไฟล์หรือโฟลเดอร์
    while ((entry = readdir(dir)) != NULL) {
        // พิมพ์ชื่อไฟล์หรือโฟลเดอร์ที่พบ
        ss << "<li style=\"position: relative; display: flex; justify-content: space-between;\">";
        ss << "\t\t\n<a href=\"?dir=" << entry->d_name << "\">" << entry->d_name << "</a>\n";
        ss << "</li>";
        // std::cout << entry->d_name << std::endl;
    }

    ss << "</body>";
    ss << "</html>";
    // ปิด directory หลังจากใช้งานเสร็จ
    closedir(dir);

    return 0;
}
