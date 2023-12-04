#include <iostream>
#include <filesystem>
#include <fstream>
#include <Core/Types.h>
#include <unistd.h>

namespace fs = std::filesystem;

#define BUFF_SIZE 10240

class FSTest {
public:

    void testPath()
    {
        auto file_path = fs::path("src");
        file_path = file_path / "Filesystem" / "FSTest.cpp";

        std::cout << file_path.string() << std::endl;
        std::cout << file_path.extension() << std::endl;
        std::cout << file_path.filename() << std::endl;
        std::cout << file_path.parent_path() << std::endl;
    }

    void create()
    {
        auto path = fs::path("test");
        fs::create_directories(path / "dir1" / "dir2");

        std::ofstream out1(path / "test1.txt");
        String s = "hello world";
        out1.write(s.data(), s.length());
        out1.close();

        std::ofstream out2(path / "test2.txt");
        int a = 1024;
        out2.write(reinterpret_cast<char *>(&a), sizeof(int));
        out2.close();

        for (auto it : fs::directory_iterator(path))
        {
            if (it.is_directory())
            {
                std::cout << "dir: " << it.path().filename() << std::endl;
            }
            else if (it.is_regular_file())
            {
                std::cout << "regular file: " << it.path().filename() << std::endl;
            }
            else
            {
                std::cout << "other: " << it.path().filename() << std::endl;
            }
        }
        std::cout << fs::absolute(path) << std::endl;
    }


    void testRead()
    {
        int size;
        char buffer[BUFF_SIZE];

        while ((size = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0)
        {
            write(STDOUT_FILENO, &size, 4);
            if (write(STDOUT_FILENO, buffer, size) != size)
                std::cout << "write error" << std::endl;
        }

        if (size < 0)
            std::cout << "read error" << std::endl;
    }


};

//
//int main()
//{
//    FSTest test;
////    test.create();
//
//    test.testRead();
//    exit(0);
//}


