#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
using namespace std;

class details
{
    vector<string> name;
    vector<string> artist;
    vector<string> fileName;
public:
    bool equality(details d)
    {
        if(name == d.name && artist == d.artist && fileName == d.fileName)
            return true;
        else
            return false;
    }
    void addDetails(string n, string a, string f)
    {
        name.push_back(n);
        artist.push_back(a);
        fileName.push_back(f);
    }
    void printDetails()
    {
        cout<<"Name of song: "<<name.front()<<endl;
        cout<<"Name of artist: "<<artist.front()<<endl;
        cout<<"Name of file: "<<fileName.front()<<endl;
    }
    string returnFileName()
    {
        return fileName.front();
    }
    string returnSongName()
    {
        return name.front();
    }
    string returnArtistName()
    {
        return artist.front();
    }
};
bool writeSongs(details *d)
{
    fstream database("nameartist.txt");
    fstream fileNames("mp3list.txt");
    string line, name, artist, file;
    char ch;
    int i, flag, j = 0;
    if(database.is_open() && fileNames.is_open())
    {
      while(!database.eof() && !fileNames.eof())
      {
          getline(database, line);
          getline(fileNames, file);
          name = "";
          artist = "";
          i = flag = 0;
          while(i<line.length())
          {
              ch = line[i];
              if(ch == ',')
              {
                flag = 1;
                goto skipComma;
              }

              if(flag)
                artist += ch;
              else
                name += ch;
    skipComma:
              i++;
          }
          (d+j)->addDetails(name, artist, file);
          j++;
      }
      return true;
    }
    else
        return false;
}
int main()
{
    details d[10];
    writeSongs(d);
    writeSongs(d);
    d[0].printDetails();
    d[1].printDetails();
    //mciSendString("open \"songs\\BlindingLights.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    mciSendString("open \"songs\\comethru.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    mciSendString("play mp3 wait", NULL, 0, NULL);
    cout << "Hello world!" << endl;
    return 0;
}
