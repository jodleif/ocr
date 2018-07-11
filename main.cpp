#include <iostream>
#include <filesystem>
#include <algorithm>
#include <string_view>
#include <sstream>
#include <vector>
#include <regex>
#include "ocr_lib.h"

namespace {

std::vector<std::string> split_lines(const std::string & src)
{
   std::stringstream ss;
   std::vector<std::string> lines;
   for(const auto& c : src){
       if(c=='\n'){
           lines.emplace_back(ss.str());
           ss.clear();
           ss.str(std::string());
       } else {
           ss << c;
       }
   }
   return lines;
}
std::string to_lowercase(const std::string& src)
{
        auto str_cp = std::string();
        str_cp.resize(src.size());
        std::transform(src.begin(),src.end(),str_cp.begin(),[](const char c) {return std::tolower(c);});
        return str_cp;
}

constexpr std::string_view action {"action required:"};
constexpr std::string_view connectid {"connect id:"};
constexpr std::string_view filetype_suffix {".png"};
constexpr std::string_view email {"email id:"};

struct Information {
    std::string email;
    std::string ticket_id;
    std::string operation;
    std::string connect_id;

};
std::string find_string(const std::string& src, const std::string_view prefix)
{
    auto iter = std::search(src.begin(),src.end(), prefix.begin(),prefix.end());
    auto str = std::string();
    if(iter!=src.end()){
    iter += prefix.size();
    if(*iter == ' ') {
        ++iter;
    }
    str.resize(std::distance(iter,src.end()));
        std::copy(iter,src.end(),str.begin());
    }
    return str;
}
std::string find_regexmatch(const std::string& src, const std::regex& reg)
{
    auto regex_begin = std::sregex_iterator(src.begin(),src.end(), reg);
    auto regex_end = std::sregex_iterator();
    for(auto i {regex_begin};i != regex_end; ++i){
        std::smatch match = *i;
        if (match.size() > 1) {
            return match[1].str();
        }
    }
    return std::string();
}
class Extractor {
    std::vector<std::string> lines;
    const std::regex reg = std::regex("#([0-9]{6})", std::regex_constants::ECMAScript);
public:
    Extractor(std::string & s)
    {
        lines = split_lines(to_lowercase(s));
    }
    Information extract_information()
    {
        Information inf;
        std::for_each(lines.begin(),lines.end(), [&inf,this](const std::string& str){
            auto connect_id = find_string(str, connectid);
            if(connect_id.size()!=0){
                inf.connect_id = connect_id;
                return;
            }
            auto email_ = find_string(str,email);
            if(email_.size()!=0){
                inf.email = email_;
                return;
            }
            auto action_ = find_string(str,action);
            if(action_.size() != 0){
                inf.operation = action_;
                return;
            }
            if(inf.ticket_id.size() == 0) {
                auto ticket_id = find_regexmatch(str, reg);
                if(ticket_id.size()>0){
                    inf.ticket_id = ticket_id;
                }
            }
        });
        return inf;
    }
};


void print_results(std::optional<std::string> str)
{
    if (str) {
        Extractor e(*str);
        auto inf = e.extract_information();
        std::cout << '\n' << inf.email << '\t' << inf.connect_id << "\t\thttps://telenordigital.zendesk.com/agent/tickets/" << inf.ticket_id << "\t" << inf.operation << '\n';
        //auto str_cp = to_lowercase(*str);
        //std::cout << str_cp << '\n';
    } else {
        std::cout << "Could not load image\n";
    }
}
}

void parse_image(const char * c)
{
    if(!c) return;
    ocr::ImageOcr img;

    auto str = img.read_image(c);
    print_results(str);
}

int main(int argc, const char **argv)
{
    if(argc > 2) {
        std::error_code ec;
       auto path = std::filesystem::path(argv[1]);
        auto exists = std::filesystem::exists(path, ec);
        if (!exists && !ec) {
            std::cout << "File: " << path << " does not exist\n";
            return 1;
        }
        auto direc = std::filesystem::directory_iterator(path);
        for(auto dir : direc) {
            if(dir.is_regular_file()) {
                auto path = dir.path();
                auto path_s = path.string();
                auto iter = std::search(path_s.begin(),path_s.end(),filetype_suffix.begin(),filetype_suffix.end());
                if(iter != path_s.end()){
                    std::cerr << "Trying file: " << path_s << '\n';
                    parse_image(path_s.c_str());
                }
            }
        }
        return 0;
    }
    if (argc < 2) return 1;
    {
        std::error_code ec;
        auto path = std::filesystem::path(argv[1]);
        auto exists = std::filesystem::exists(path, ec);
        if (!exists && !ec) {
            std::cout << "File: " << path << " does not exist\n";
            return 1;
        }
        parse_image(argv[1]);
    }
    return 0;
}
