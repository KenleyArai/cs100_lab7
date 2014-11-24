#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "my_rm.h"
#include "my_cp.h"
using namespace std;

bool file_exist(const string &file, struct stat &info);

int main(int argc, char *argv[])
{
    //If there are no arguments exit the program
    if(argc == 1)
    {
        cerr << "No arguments" << endl;
        return 1;
    }
    else if(argc > 3)
    {
        cerr << "Too many arguments "<< endl;
        return 1;
    }

    //Getting all the arguments and putting them
    //into a vector
    vector<string> args(argv + 1, argv + argc);

    //Returning because the file is itself
    if(args.front() == args.back())
        return 0;

    struct stat info;
    bool file_exists = file_exist(args.back(), info);

    if(file_exists && S_ISREG(info.st_mode))
    {
        cerr << "Can't mv into a regular file" << endl;
        return 1;
    }

    if(file_exists)
    {
        string new_path = args.back() + "/" + args.front();
        if(link(args.front().c_str(), new_path.c_str()) == -1)
            perror("Error link");
        if(my_rm(args.front()) == -1)
            perror("Couldn't remove old file");
    }
    else
    {
        my_cp(args);
        args.pop_back();
        args.push_back("-r");
        if(my_rm(args) == -1)
            perror("Counld't remove old file");
    }

    return 0;
}

bool file_exist(const string &file, struct stat &info)
{
    return ( stat(file.c_str(), &info) == 0 );
}
