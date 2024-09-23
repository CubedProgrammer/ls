#include<array>
#include<algorithm>
#include<filesystem>
#include<iostream>
#include<ranges>
#include"size_and_colours.hpp"

using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::filesystem::recursive_directory_iterator;
using std::ranges::copy;
using std::ranges::max_element;
using std::views::transform;
using std::cout;
using std::endl;

std::array<std::string, 3>execute_exts{ ".ps1", ".bat", ".exe" };
std::array<std::string, 5>archive_exts{ ".7z" , ".zip" , ".tar" , ".tar.gz" , ".jar" };
std::array<std::string, 5>picture_exts{ ".bmp", ".gif", ".jpg", ".png", ".svg" };

void print_entry(const path& cont)
{
    using std::ranges::contains;
    bool reset = true;
    if (is_regular_file(cont))
    {
        std::string ext = cont.extension().string();
        if (contains(execute_exts, ext))
            cout << green_foreground;
        else if (ext == ".lnk")
            cout << cyan_foreground;
        else if (contains(archive_exts, ext))
            cout << red_foreground;
        else if (contains(picture_exts, ext))
            cout << magenta_foreground;
        else
            reset = false;
    }
    else if (is_symlink(cont))
        cout << cyan_foreground;
    else if (is_directory(cont))
        cout << blue_foreground;
    cout << cont.string();
    if (reset)
        cout << reset_console;
}

void display_entries(std::pair<int, int>consz, bool bycol, const std::vector<std::string>& ens)
{
    using namespace std::views;
    size_t maxi = (*max_element(ens, std::ranges::less{}, &std::string::size)).size();
    size_t cols = consz.first / (maxi + 2);
    size_t last_row_cnt = ens.size() % cols;
    size_t rcnt = ens.size() / cols;
    std::string tmp;
    if (bycol)
    {
        for (auto r : iota(static_cast<size_t>(0), rcnt))
        {
            for (std::string cont : stride(drop(ens, r), rcnt))
            {
                print_entry(path(cont));
                cout << std::string(maxi + 2 - cont.size(), ' ');
            }
            endl(cout);
        }
    }
    else
    {
        for (auto r : chunk(ens, cols))
        {
            for (std::string cont : r)
            {
                print_entry(path(cont));
                cout << std::string(maxi + 2 - cont.size(), ' ');
            }
            endl(cout);
        }
    }
    cout << reset_console;
}

int main_pp(std::vector<std::string>& args)
{
    enable_colours();
    auto consz = get_console_size_cpp();
    std::string currstr = ".";
    bool bycol = true, dots = false;
    bool rev = false;
    bool recurse = false;
    if (args.size() != 0)
    {
        for (const auto& arg : args)
        {
            if (arg[0] == '-')
            {
                using std::views::drop;
                for (char ch : arg | drop(1))
                {
                    switch (ch)
                    {
                    case'x':
                        bycol = false;
                        break;
                    case'a':
                        dots = true;
                        break;
                    case'r':
                        rev = true;
                        break;
                    case'R':
                        recurse = true;
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
    std::vector<std::string>dir_ens;
    if (dots)
    {
        dir_ens.push_back({ currstr + "\\." });
        dir_ens.push_back({ currstr + "\\.." });
    }
    auto direntstr = [](const directory_entry& en) { return en.path().string(); };
    if (recurse)
        dir_ens.append_range(transform(recursive_directory_iterator(curr), direntstr));
    else
        dir_ens.append_range(transform(directory_iterator(curr), direntstr));
    if (rev)
        std::ranges::reverse(dir_ens);
    if (!std::ranges::empty(dir_ens))
        display_entries(consz, bycol, dir_ens);
    return 0;
}

int main(int argl, char* argv[])
{
    std::vector<std::string>args(argv + 1, argv + argl);
    return main_pp(args);
}
