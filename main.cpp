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
struct song
{
    details d;
    song* next;
    song* prev;
};
bool writeSongs(details *d)
{
    fstream database("nameartist.txt");
    fstream fileNames("mp3list.txt");
    string line, name, artist, file;
    char ch;
    unsigned int i, flag, j = 0;
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
void playSong(details d)
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
            cout<<"Enter 1 to resume the song or 2 to stop: ";
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
class songQueue
{
    song *head, *tail;
public:
    songQueue()
    {
        head = NULL;
        tail = NULL;
    }
    void addSong(details d)
    {
        song *s = new song;
        s->d = d;
        if(head==NULL)
        {
            head = s;
            tail = s;
            s->prev = NULL;
        }
        else
        {
            song *temp = head;
            while(temp->next!=NULL)
                temp = temp->next;
            temp->next = s;
            s->prev = temp;
            tail = tail->next;
        }
        s->next = NULL;

    }

    bool printSongByIndex(int x)
    {
        song* temp = head;
        for(int i=0; i<x; i++)
            temp = temp->next;

        cout<<"Song number "<<x+1<<" : "<<endl;
        (temp->d).printDetails();
        cout<<endl;

        return true;
    }
    void playQueueFromEnd(int i)
    {
        song* temp = tail;
        while(temp!=head->prev)
        {
            printSongByIndex(i);
            i--;
            playSong(temp->d);
            temp = temp->prev;
        }
    }
    void playQueueFromFront()
    {
        song* temp = head;
        int j = 0;
        while(temp!=tail->next)
        {
            printSongByIndex(j);
            j++;
            playSong(temp->d);
            temp = temp->next;
        }
    }
    void clearQueue()
    {
        song* temp;
        while(head!=NULL)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
    song* retHead()
    {
        return head;
    }
    ~songQueue()
    {
        song* temp;
        while(head!=NULL)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main()
{
    details d[10];
    songQueue q;
    int ch,w=0,opt;
    writeSongs(d);
    while(true)
    {
        cout<<"1: Play Single Song"<<endl;
        cout<<"2: Add songs to Queue"<<endl;
        cout<<"3: Play queue"<<endl;
        cout<<"4: Exit"<<endl;
        cout<<"Enter choice: ";
        cin>>ch;
        switch(ch)
        {
        case 1:
        {
            for(int x=0; x<5; x++)
            {
                cout<<"Song number: "<<x+1<<endl;
                d[x].printDetails();
                cout<<"\n";
            }
            int y;
            cout<<"Enter song number to play: ";
            cin>>y;
            playSong(d[y-1]);
            system("cls");
            break;
        }
        case 2:
        {
            while(opt)
            {
                for(int x=0; x<5; x++)
                {
                    cout<<"Song number: "<<x+1<<endl;
                    d[x].printDetails();
                    cout<<"\n";
                }
                int y;
                cout<<"Enter song number to add to queue: ";
                cin>>y;
                q.addSong(d[y-1]);
                w++;
                cout<<"Do you want to add another song [1(YES)/0(NO)]: ";
                cin>>opt;
            }
            system("cls");
            break;
        }
        case 3:
        {
            if(q.retHead() == NULL)
            {
                cout<<"No song in the queue \n";
                break;
            }
replay:
            cout<<"Play from beginning or end [1(BEGINNING)/0(END)]: ";
            cin>>opt;
            if(opt==1)
                q.playQueueFromFront();
            else if(opt==0)
                q.playQueueFromEnd(w-1);
            else
            {
                cout<<"Not a valid selection \n";
                goto replay;
            }

            q.clearQueue();
            system("cls");
            break;
        }
        case 4:
        {
            cout<<"THANK YOU "<<(char)1<<endl;
            exit(0);
        }
        default:
        {
            cout<<"INVALID!!!!"<<endl;
            break;
        }
        }
    }
    return 0;
}
