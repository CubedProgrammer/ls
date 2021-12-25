#include<array>
#include<algorithm>
#include<filesystem>
#include<iostream>
#if __cplusplus >= 202002L
#include<ranges>
#endif
#include"size_and_colours.hpp"

using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::cout;
using std::endl;

std::array<std::string, 5>archive_exts{ ".7z" , ".zip" , ".tar" , ".tar.gz" , ".jar" };

void print_entry(const directory_entry& cont, const std::string& name)
{
    if (cont.is_regular_file())
    {
        std::string ext = cont.path().extension().string();
        if (ext == ".psh" || ext == ".bat" || ext == ".exe")
            cout << green_foreground;
        else if (std::find(archive_exts.begin(), archive_exts.end(), ext) != archive_exts.end())
            cout << red_foreground;
        else
            cout << reset_console;
    }
    else if (cont.is_symlink())
        cout << cyan_foreground;
    else if (cont.is_directory())
        cout << blue_foreground;
    cout << name;
}

int main_pp(std::vector<std::string>& args)
{
    enable_colours();
    auto consz = get_console_size_cpp();
    std::string currstr = ".";
    bool bycol = true, dots = false;
    if (args.size() != 0)
    {
        for (const auto& arg : args)
        {
            if (arg[0] == '-')
            {
#if __cplusplus >= 202002L
                using namespace std::ranges;
                for (char ch : subrange(arg.begin() + 1, arg.end()))
#else
                for (char ch : arg.substr(1))
#endif
                {
                    switch (ch)
                    {
                    case'x':
                        bycol = false;
                        break;
                    case'a':
                        dots = true;
                        break;
                    default:
                        cout << "Unrecognized option -" << ch << endl;
                    }
                }
            }
            else
                currstr = arg;
        }
    }
    path curr = currstr;
    if (currstr.back() == '\\')
        currstr = currstr.substr(0, currstr.size() - 1);
    size_t maxi = 0;
    std::vector<directory_entry>dir_ens;
    if (dots)
    {
        dir_ens.push_back(directory_entry{ currstr + "\\." });
        dir_ens.push_back(directory_entry{ currstr + "\\.." });
    }
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
    if (bycol)
    {
        for (size_t i = -1, ps = 0; ps < dir_ens.size(); ++ps, i += rcnt)
        {
            if (colcnt == 0)
                i = (i + 1) % dir_ens.size();
            const auto& cont = dir_ens[i];
            tmp = cont.path().string().substr(currstr.size() + 1);
            print_entry(cont, tmp);
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
    }
    else
    {
        for (size_t ps = 0; ps < dir_ens.size(); ++ps)
        {
            const auto& cont = dir_ens[ps];
            tmp = cont.path().string().substr(currstr.size() + 1);
            print_entry(cont, tmp);
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
    }
    cout << reset_console;
    if (colcnt != 0)
        endl(cout);
    return 0;
}

int main(int argl, char* argv[])
{
    std::vector<std::string>args(argv + 1, argv + argl);
    return main_pp(args);
}