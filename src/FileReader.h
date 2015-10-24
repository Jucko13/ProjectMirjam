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
    void changeSettings(int i, string s);
    void getTime(unsigned int i, int *, string *);
    void clearSettings();
private:
    vector<int> settingsInt;
    vector<string> settingsString;
};

#endif // FILEREADER_H
