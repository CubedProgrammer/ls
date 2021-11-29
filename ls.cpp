#include<filesystem>
#include<iostream>
#include"size_and_colours.hpp"

//using std::filesystem::absolute;
using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::cout;
using std::endl;

int main(int argl, char* argv[])
{
    enable_colours();
    auto consz = get_console_size_cpp();
    std::string currstr = ".";
    if (argv[1] != nullptr)
        currstr = argv[1];
    path curr = currstr;
    if (currstr.back() == '\\')
        currstr = currstr.substr(0, currstr.size() - 1);
    size_t maxi = 0;
    std::vector<directory_entry>dir_ens;
    for (const auto& cont : directory_iterator(curr))
    {
        maxi = std::max(maxi, cont.path().string().size());
        dir_ens.push_back(cont);
    }
    maxi -= currstr.size() + 1;
    size_t cols = consz.first / (maxi + 2), rm;
    size_t colcnt = 0, last_row_cnt = dir_ens.size() % cols;
    size_t rcnt = dir_ens.size() / cols;
    std::string tmp;
    for(size_t i = -1, ps = 0; ps < dir_ens.size(); ++ps, i+= rcnt)
    {
        if (colcnt == 0)
            i = (i + 1) % dir_ens.size();
        const auto& cont = dir_ens[i];
        tmp = cont.path().string().substr(currstr.size() + 1);
        if (cont.is_regular_file())
        {
            if (cont.path().extension().string() == ".exe")
                cout << green_foreground;
            else
                cout << reset_console;
        }
        else if (cont.is_directory())
            cout << blue_foreground;
        cout << tmp;
        if (colcnt < last_row_cnt)
            ++i;
        if (colcnt == cols - 1)
        {
            cout << '\n';
            colcnt = -1;
        }
        else
        {
            rm = maxi + 2 - tmp.size();
            for (size_t i = 0; i < rm; i++)
                cout << ' ';
        }
        ++colcnt;
    }
    cout << reset_console;
    if (colcnt != 0)
        endl(cout);
    return 0;
}