#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
#include <vector>

using namespace std;

class FileReader
{
public:
    FileReader();
    virtual ~FileReader();
    void readFile();
    void writeFile();
    void changeSettings(unsigned int i, string s);
    string getTime(unsigned int i);
private:
    vector<string> settings;
};

#endif // FILEREADER_H
