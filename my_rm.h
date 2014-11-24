#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

int my_rm(int argc, char *argv[]);
int my_rm(const string &path);
int my_rm(const vector<string> &args);
vector<string> find_all_dirs(const vector<string> &all_args);
vector<string> get_contents(const string &path);
bool find_recursive_flag(const vector<string> &all_args);


int my_rm(int argc, char *argv[])
{
    //If there are no arguments exit the program 
    if(argc <= 1)
        return 1;

    //Putting the arguments into a vector
    vector<string> args(argv + 1, argv + argc);
    return my_rm(args);
}

int my_rm(const string &path)
{
    vector<string> dir{path};
    return my_rm(dir);
}

int my_rm(const vector<string> &args)
{
    //Sorting the arguments into directories and flag
    vector<string> dirs = find_all_dirs(args);
    bool rec_flag       = find_recursive_flag(args);
    struct stat info;

    for(auto it : dirs)
    {
        if(stat(it.c_str(), &info) == -1)
            perror("Error using stat");
        if(rec_flag && S_ISDIR(info.st_mode))
        {
            vector<string> dir = get_contents(it);
            dir.push_back("-r");
            my_rm(dir);
            if(rmdir(it.c_str()) == -1)
                perror("Error removing directory");
        }
        else
        {
            if(unlink(it.c_str()) == -1)
                perror("Error removing file");
        }
    }

    return 0;
}

vector<string> get_contents(const string &path)
{
    vector<string> v;
    DIR *dir;
    struct dirent *ent;
    if(NULL != (dir = opendir(path.c_str())))
    {
        while(NULL != (ent = readdir(dir)))
            if(string(".") != ent->d_name && string("..") != ent->d_name)
                v.push_back(path + "/" + ent->d_name);
        closedir(dir);
    }
    else
        perror("Error closing dir");
    return v;
}

vector<string> find_all_dirs(const vector<string> &all_args)
{
    vector<string> dirs;
    for(auto it : all_args)
        if(it.at(0) != '-')
            dirs.push_back(it);
    return dirs;
}

bool find_recursive_flag(const vector<string> &all_args)
{
    for(auto it : all_args)
        if(it.at(0) == '-' && it.at(1) == 'r')
            return true;
    return false;
}

