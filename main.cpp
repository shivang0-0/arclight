#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <queue>
#include <graphics.h>
using namespace std;

class TreeNode;
class tree;
class details
{
public:
    string name;
    string artist;
    string fileName;
    bool equality(details d)
    {
        if(name == d.name && artist == d.artist && fileName == d.fileName)
            return true;
        else
            return false;
    }
    void addDetails(string n, string a, string f)
    {
        name = n;
        artist = a;
        fileName = f;
    }
    void addDetails(TreeNode*,tree);
    void printDetails()
    {
        cout<<"Name of song: "<<name<<endl;
        cout<<"Name of artist: "<<artist<<endl;
        cout<<"Name of file: "<<fileName<<endl;
    }
    string returnFileName()
    {
        return fileName;
    }
    string returnSongName()
    {
        return name;
    }
    string returnArtistName()
    {
        return artist;
    }
};

class TreeNode
{
public:
    string name;
    TreeNode *left,*right;
    TreeNode()
    {
        name ="";
        left = NULL;
        right = NULL;
    }
    TreeNode(string s)
    {
        name = s;
        left = NULL;
        right = NULL;
    }
};
class tree
{
public:
    TreeNode *root;
    tree()
    {
        root = NULL;
    }
    void insertIntoBST(string name)
    {
        if(root == NULL)
        {
            TreeNode *t = new TreeNode(name);
            root = t;
            return;
        }
        TreeNode *curr = root;
        while (true)
        {
            if (name < (curr->name))
            {
                if(curr->left)
                    curr= curr->left;
                else
                {
                    curr->left= new TreeNode(name);
                    break;
                }
            }
            else
            {
                if(curr->right)
                    curr= curr->right;
                else
                {
                    curr->right= new TreeNode(name);
                    break;
                }
            }
        }
    }
    void display()
    {
        if(root==NULL)
        {
            cout<<"Empty!!!"<<endl;
            return;
        }
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty())
        {
            TreeNode* node = q.front();
            q.pop();
            cout<<node->name<<" ";
            if(node->left != NULL)
                q.push(node->left);
            if(node->right != NULL)
                q.push(node->right);
        }
        cout<<endl;
    }
    int getindex(details *d,string name)
    {
        cout<<"getindx reached"<<endl;
        for(int i=0; i<10; i++)
        {
            cout<<"In loop"<<endl;
            if(d[i].name == name)
            {
                cout<<"idx in getindx: "<<i<<endl;
                return i;
            }
        }
        return -1;
    }
    int searchnplay(string name,details *d)
    {
        cout<<"srchnplay reached "<<endl;
        TreeNode *temp = root;
        while(temp != NULL)
        {
            if(temp->name == name)
            {
                cout<<"Called getindex"<<endl;
                int idx = getindex(d,name);
                cout<<"idx in srchnplay: "<<idx<<endl;
                return idx;
            }
            else if(name < temp->name)
            {
                temp = temp->left;
            }
            else
            {
                temp = temp->right;
            }
        }
        cout<<"Not found in our library!!"<<endl;
        return -1;
    }
} bst;
void details::addDetails(TreeNode *root,tree t)
{
    t.insertIntoBST(name);
}
struct song
{
    details d;
    song* next;
    song* prev;
};
bool writeSongs(details *d, tree *bst)
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
            bst->insertIntoBST(name);
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
    string command="Set mp3 time format milliseconds";
    mciSendString(command.c_str(), NULL, 0, NULL);
    command="Status mp3 length";
    long int duration;
    char retstring[256];
    mciSendString(command.c_str(), retstring, 256, NULL);
    string r(retstring);
    duration =stol(r);
    cout<<"\nSong Duration: "<<duration<<endl;
    while(1)
    {
        t1=time(0);
        mciSendString("play mp3", NULL, 0, NULL);
        cout<<"Playing song - "<<d.returnSongName()<<endl;
        cout<<endl;
        char songname[d.returnSongName().length()+1];
        strcpy(songname,d.returnSongName().c_str());
        int gd = DETECT, gm;
        initgraph(&gd, &gm, "");
        for(int i=0; i<1280; i++)
        {
            setcolor(YELLOW);
            settextstyle(8,0,4);
            int x=strlen(songname)*20;
            if(i<640-x)
                moveto(i,200);
            else
                moveto(abs(640-x-(i-640+x)),200);
            outtext(songname);
            delay(duration/800);
            setcolor(WHITE);
            line(i,290,i,310);
        }
        closegraph();
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
class playlist
{
    song *head;
public:
    playlist()
    {
        head = NULL;
    }
    void addSong(details d)
    {
        song *s = new song;
        s->d = d;
        if(head==NULL)
        {
            head = s;
            s->prev = NULL;
        }
        else
        {
            song *temp = head;
            while(temp->next!=NULL)
                temp = temp->next;
            temp->next = s;
            s->prev = temp;
        }
        s->next = NULL;
    }
    bool delSong(details d)
    {
        int flag = 0;
        song *temp = head;
        if(d.equality(temp->d))
            goto skipWhile;
        while(temp->next!=NULL)
        {
            if((d.equality((temp->next)->d)))
            {
                flag = 1;
                break;
            }
            else
                temp = temp->next;
        }
skipWhile:
        if(flag)
        {
            song* tbd = temp->next;
            temp->next = tbd->next;
            delete tbd;
            return true;
        }
        else
            return false;
    }
    bool printSongs()
    {
        int i = 1;
        song* temp = head;
        while(temp!=NULL)
        {
            cout<<"Song number "<<i<<" : "<<endl;
            (temp->d).printDetails();
            cout<<endl;
            temp = temp->next;
            i++;
        }
        return true;
    }
    bool searchSong(details d)
    {
        song* temp = head;
        while(temp!=NULL)
        {
            if(temp->d.returnFileName() == d.returnFileName())
                return true;
            else
                temp = temp->next;

        }
        return false;
    }
    void playPlaylist()
    {
        song* temp = head;
        while(temp!=NULL)
        {
            playSong(temp->d);
            temp = temp->next;
        }
    }
    ~playlist()
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
    int ch,w=0,opt=1;
    int i=0,x,y,z;
    playlist p[10];
    writeSongs(d,&bst);
    bst.display();
    cout<<endl;
    while(true)
    {
        cout<<"1: Play Single Song"<<endl;
        cout<<"2: Add songs to Queue"<<endl;
        cout<<"3: Play queue"<<endl;
        cout<<"4: Create Playlist"<<endl;
        cout<<"5: Play existing Playlist"<<endl;
        cout<<"6: Search 'n play"<<endl;
        cout<<"7: Exit"<<endl;
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
            if(i<=9)
            {
                opt = 1;
                cout<<"Play list name: Play list-"<<i+1<<"\n";
                while(opt != 0)
                {
                    for(x=0; x<5; x++)
                    {
                        cout<<"Song number: "<<x+1<<endl;
                        d[x].printDetails();
                        cout<<"\n";
                    }
                    cout<<"Enter song number to add to play list: ";
                    cin>>y;
                    p[i].addSong(d[y-1]);
                    cout<<"Do you want to add another song(1/0): ";
                    cin>>opt;
                }
                i++;
            }
            else
                cout<<"Maximum play list created!";
            break;
        }
        case 5:
        {
            if(i==0)
            {
                cout<<"No play list to play yet!\n";
                break;
            }
            cout<<"Enter the play list number to play: ";
            cin>>z;
            if(z>10 || z>i)
            {
                cout<<"Not a valid play list";
                break;
            }
            cout<<"\n\n";
            p[z-1].printSongs();
            p[z-1].playPlaylist();
            break;
        }
        case 6:
        {
            string name;
            cout<<"Enter song name to search: ";
            getline(cin>>ws,name);
            int idx=bst.searchnplay(name,d);
            playSong(d[idx]);
            break;
        }
        case 7:
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
