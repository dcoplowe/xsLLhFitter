#ifndef __READPARAM__CPP__
#define __READPARAM__CPP__

#include <ReadParam.h>

#include <sstream>
#include <cctype>
#include <stdlib.h>
#include <fstream> //ifstream header
#include <cassert>
#include <algorithm>
#include <iterator>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

const bool ReadParam::m_verbose = false;

bool ReadParam::IsNumber(const std::string &in)
{
    bool yes = false;
    if(isdigit( in[0] )) yes = true;
    else if(in.length() > 1 && in[0] == '.'){
        if(isdigit( in[1] ) ) yes = true;
    }
    else if(in.length() > 1 && in[0] == '-'){
        if(isdigit( in[1] ) ) yes = true;
        else if(in.length() > 2 && isdigit( in[2] ) ) yes = true;
    }
    return yes;
}

std::vector<std::string> ReadParam::ReadFile(const std::string &infile, const std::string &left_arrow, const std::string &right_arrow,
    const std::string &ignore)
{
    // cout << " ReadParam::ReadFile(std::string infile, std::string left_arrow, std::string right_arrow, std::string ignore) " << endl;
    std::vector<std::string> list;
    list.clear();

    std::ifstream fin( infile.c_str() );
    assert(fin.is_open());
    string line;

    string tmp_line = "";
    bool complete = false;

    while (getline(fin, line))
    {
        if(!ignore.empty()){
            if(line.find( ignore ) != string::npos) continue;
        }

        if(line.find(left_arrow) != std::string::npos){
            tmp_line = line;
            if(line.find(right_arrow) != std::string::npos) complete = true;
        }
        else if(line.find(right_arrow) != std::string::npos){
            tmp_line += line;
            complete = true;
        }
        else{
            tmp_line += line;
        }

        if(complete){
            list.push_back( tmp_line );
            tmp_line = "";
            complete = false;
            // cout << "push_back success" << endl;
        }
    }
    fin.close();

    return list;
}

void ReadParam::RemoveArrows(std::string &word, const std::string &left_arrow, const std::string &right_arrow)
{
    // Add | to both arrows as we may/will have instances where the comparison symbols (</>) are used. 
    size_t larrow = word.find(left_arrow);
    size_t rarrow = word.find(right_arrow);
    if(larrow != string::npos && rarrow != string::npos){
        size_t l_length = left_arrow.size();
        size_t r_length = right_arrow.size();
        size_t length = rarrow - larrow - r_length;
        word = word.substr(larrow + l_length, length);
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Could not find left or right arrow(s)." << endl;
        exit(0);
    }
}

std::string ReadParam::GetParameterS(const std::string &name, const std::string &infile, const std::string &left_arrow,
    const std::string &right_arrow, const bool remove_spaces, const std::string &ignore)
{
    // cout << " ReadParam::GetParameterS : " << infile << endl;
    std::vector<std::string> list = ReadParam::ReadFile(infile, left_arrow, right_arrow, ignore);
    // Fix this to make sure it finds exact words.
    int store = -999;
    bool called = false;
    size_t name_size = name.size();
    for(size_t i = 0; i < list.size(); i++){
        // Need to do a little bit of work here:
        // string are a bit of a pain... To find exact string
        // we want to first get the substring 'name' from <| name = var |>
        string tmp = list[i];

        // In order to speed things up, let's first to a quick check that the text pattern exists,
        // then do full check.
        size_t found = -1;
        if( (found = tmp.find(name)) != tmp.npos){

            // 1) remove <| and |>
            if(m_verbose) cout << "1) tmp = " << tmp << " : size = " << tmp.size() << endl;
            RemoveArrows(tmp, left_arrow, right_arrow);
            if(m_verbose) cout << "2) tmp = " << tmp << " : size = " << tmp.size() << endl;

                 // 2) remove = var term
            size_t place = tmp.find("=");
            if(place != string::npos){
                tmp = tmp.erase(place, tmp.size());
            }
            else{
                cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter not in correct form (missing =), we have: " << tmp << endl;
                exit(0);
            }
            if(m_verbose) cout << "3) tmp = " << tmp << " : size = " << tmp.size() << endl;

                 // 3) remove white space
            tmp.erase(std::remove(tmp.begin(), tmp.end(),' '), tmp.end());
            if(m_verbose) cout << "4) tmp = " << tmp << " : size = " << tmp.size() << " name.size() = " << name_size << endl;

                 // Should now be able to compare the sizes of the strings:
                 // found = line.find("xyz")) != line.npos)
                 // Old method:
                 // if(tmp.find(name) != string::npos && !called){ 
                 // New method should only find exact strings.
            if(name_size == tmp.size()){ 
                found = -1;
                if((found = tmp.find(name) != tmp.npos) && !called){ 
                   store = i;
                   called = true;
                }
                else if(tmp.find(name) == string::npos) continue;
                else if(called){
                   cout << __FILE__ << ":" << __LINE__ << " : ERROR : Multiple instances of param name " << name;
                   cout << " in parameters file (" << infile << ")" << endl;
                }
            }
        }
    }

    string param = "";
    if(store != -999){
        param = list[store];
        if(!param.empty()){
            RemoveArrows(param, left_arrow, right_arrow);
            size_t place = param.find("=");
            // if(place != string::npos){
            param = param.erase(0, place + 1);
            // }
            // else{
            //     cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter not in correct form (missing =), we have: " << param << endl;
            //     exit(0);
            // }
            // Remove white space:
            if(remove_spaces) param.erase(std::remove(param.begin(), param.end(),' '), param.end());
        }
    }

    list.clear();
    // This is to get rid of white space quickly:
    return param;
}

std::string ReadParam::GetParameterN(const std::string &name, const std::string &infile, const std::string &left_arrow, const std::string &right_arrow,
    const std::string &ignore)
{
    string param = GetParameterS(name, infile, left_arrow, right_arrow, true, ignore);
    if(!param.empty()){
        param.erase(std::remove(param.begin(),param.end(),' '),param.end());
        if(!IsNumber(param)){
            cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter is not a number, we have: " << param << endl;
            exit(0);
        }
    }
    else param = "-999";
    return param;
}

int ReadParam::GetParameterI(const std::string &name, const std::string &infile, const std::string &left_arrow, const std::string &right_arrow,
    const std::string &ignore)
{
    return atoi(GetParameterN(name, infile, left_arrow, right_arrow, ignore).c_str());
}

double ReadParam::GetParameterD(const std::string &name, const std::string &infile, const std::string &left_arrow, const std::string &right_arrow,
    const std::string &ignore)
{
    return atof(GetParameterN(name, infile, left_arrow, right_arrow, ignore).c_str());
}

bool ReadParam::GetParameterB(const std::string &name, const std::string &infile, const std::string &left_arrow, const std::string &right_arrow,
    const std::string &ignore)
{
    return static_cast<bool>( GetParameterI(name, infile, left_arrow, right_arrow, ignore) );
}

#endif
