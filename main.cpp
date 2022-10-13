#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <time.h>
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
void playSingleSong(details d)
{
    int n = 0;
    time_t t1, t2, previous_pause_time=0;
    string filename = "open \"songs\\"+d.returnFileName()+"\" type mpegvideo alias mp3";
    mciSendString(filename.c_str(), NULL, 0, NULL);

    while(1)
    {
        t1=time(0);
        mciSendString("play mp3", NULL, 0, NULL);
        cout<<"Playing song - "<<d.returnSongName()<<endl;
        cout<<endl;
        cout << "Enter 1 to pause the song or 2 to end the song - " << endl;
        cin>>n;
        if(n==1)
        {
            mciSendString("pause mp3", NULL, 0, NULL);
            t2=time(0);
            cout<<"Song paused after playing for " << t2-t1+previous_pause_time << " seconds.\n\n";
            previous_pause_time+=t2-t1;
            cout<<"Enter 1 to resume the song or 2 to exit: ";
            cin>>n;
            if(n==1)
            {
                t1=time(0);
                mciSendString("play mp3", NULL, 0, NULL);
                cout<<"Resuming song... \n";
            }
            else if(n==2)
            {
                mciSendString("close mp3", NULL, 0, NULL);
                break;
            }
        }
        else if(n==2)
        {
            mciSendString("close mp3", NULL, 0, NULL);
            break;
        }

    }
}

int main()
{
    details d[10];
    writeSongs(d);
//    mciSendString("open \"songs\\BlindingLights.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
//    mciSendString("open \"songs\\comethru.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
//    mciSendString("play mp3 wait", NULL, 0, NULL);
    for(int x=0; x<2; x++)
    {
        cout<<"Song number: "<<x+1<<endl;
        d[x].printDetails();
        cout<<"\n";
    }
    int y;
    cout<<"Enter song number to play: ";
    cin>>y;
    playSingleSong(d[y-1]);
    return 0;
}
