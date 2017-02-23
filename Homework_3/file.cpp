#include <iostream>
#include <string>
using namespace std;

//--------------- Declarations ---------------

class File
{
public:
    File(string n);
    virtual ~File();
    string name() const;
    virtual void open() const = 0;
    virtual void redisplay() const;
private:
    string m_name;
    
};

class TextMsg : public File
{
public:
    TextMsg(string n);
    virtual ~TextMsg();
    virtual void open() const;
};

class Video : public File
{
public:
    Video(string n, int time);
    virtual ~Video();
    virtual void open() const;
    virtual void redisplay() const;
private:
    int m_time;
};

class Picture : public File
{
public:
    Picture(string n);
    virtual ~Picture();
    virtual void open() const;
};

//-------------- Implementations --------------

//-------------- File ---------------
File::File(string n)
:m_name(n)
{}

string File::name() const
{
    return m_name;
}

File::~File()
{}

void File::redisplay() const
{
    cout<<"refresh the screen";
}

//-------------- TextMsg ---------------
TextMsg::TextMsg(string n)
:File(n)
{}

TextMsg::~TextMsg()
{
    cout<<"Destroying "<<name()<<", a text message"<<endl;
}

void TextMsg::open() const
{
    cout<<"open text message";
}

//-------------- Video ---------------
Video::Video(string n, int time)
:File(n), m_time(time)
{}

Video::~Video()
{
    cout<<"Destroying "<<name()<<", a video"<<endl;
}

void Video::open() const
{
    cout<<"play "<<m_time<<" second video";
}

void Video::redisplay() const
{
    cout<<"replay video";
}

//-------------- Picture ---------------
Picture::Picture(string n)
:File(n)
{}

Picture::~Picture()
{
    cout<<"Destroying the picture "<<name()<<endl;
}

void Picture::open() const
{
    cout<<"show picture";
}


//------------------- End -------------------

void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}

int main()
{
    File* files[4];
    files[0] = new TextMsg("fromFred.txt");
    // Videos have a name and running time
    files[1] = new Video("goblin.mpg", 3780);
    files[2] = new Picture("kitten.jpg");
    files[3] = new Picture("baby.jpg");
    
    for (int k = 0; k < 4; k++)
        openAndRedisplay(files[k]);
    
    // Clean up the files before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete files[k];
}