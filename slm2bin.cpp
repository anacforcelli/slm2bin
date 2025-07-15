/* slm2bin */
#include <iostream>
#include <fstream>
#include <cstring>
#include <exception>
#include <errno.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cerr << "Usage: \nslm2cpp <infile>.slm <outfile>.bin" << endl;
        return -EINVAL;
    }

    ifstream slmFile(argv[1]);

    if(slmFile.fail())
    {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return -errno;
    }

    ofstream binFile(argv[2]);

    if(binFile.fail())
    {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return -errno;
    }

    char in[9]; // Buffer large enough for 8 characters plus null terminator
    while(true)
    {
        slmFile.get(in, 2);
        if(slmFile.gcount() == 0) break; // Nothing read
        if(slmFile.eof()) break;

        slmFile.getline(in, 9, ' '); // Read up to 8 characters for address
        if(slmFile.gcount() == 0) break; // Nothing read
        unsigned long addr = strtoul(in, NULL, 16);

        cout << std::hex << addr << " ";

        slmFile.getline(in, 3, '\n'); // Read up to 3 characters for data
        if(slmFile.gcount() == 0) break; // Nothing read
        char data = (char)strtoul(in, NULL, 16); // Note: strtoint should be strtoul

        cout << std::hex << (int)data << endl; // 

        binFile.seekp(addr);
        binFile.put(data);
    }

    slmFile.close();
    binFile.close();

    cout << "Conversion Finished" << endl;

    return 0;
}