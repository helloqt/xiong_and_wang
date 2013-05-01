#ifndef _USER_DEFINE_
#define _USER_DEFINE_



#include <QDebug>
#define MY_DEBUG

#define POINTCOUNT 400//每个通道采样点个数
#define SECTIONCOUNT 4//通道个数
#define TEMPFILENAME "temp.uds"//MyFile类中使用的临时文件，接收到的数据都存放在这个临时文件中。
#define FILEHEADERSIZE 10240//文件头大小，方便以后对文件头添加不同协议。
#define LOWSENDBYTECOUNT (POINTCOUNT * SECTIONCOUNT * 2 + 16)
#define DATANODESIZE (POINTCOUNT*SECTIONCOUNT*sizeof(short) + 4)
#define FILEFINISHSTATE "finish"
/************************************************************************/
/* 主机指令                                                                     */
//主机指令为两个字节，第一个字节为指令代码，第二字节为命令参数。
//若没有发送参数设置的指令，则自动选择默认设置

#define STOPSAMPLING 0x01
#define STARTSAMPLING 0x02
//设置孔径范围
#define SETRADIUS1 0x10001 //≤1.5m，采样延时t0=0（默认）
#define SETRADIUS2 0x10002 //≤3.0m,采样延时t0=0
#define SETRADIUS3 0x10003 //＞3.0m，采样延时t0=1000mm

/************************************************************************/

#include <QList>

class MyDataNode
{
public :
    MyDataNode()
    {
        height = 0;
        for (int i = 0; i < SECTIONCOUNT; i++)
        {
            for (int j = 0; j < POINTCOUNT; j++)
            {
                data[i][j] = 0;
            }
        }
    }
    ~MyDataNode(){}
    float height;

    short data[SECTIONCOUNT][POINTCOUNT];

    MyDataNode& operator = (const MyDataNode& node)
    {
        this->height = node.height;
        for (int i = 0; i < SECTIONCOUNT; i++)
        {
            for (int j = 0; j < POINTCOUNT; j++)
            {
                data[i][j] = node.data[i][j];
            }
        }
        return *this;
    }
};



typedef union _MY_FILE_HEADER
{
    struct
    {
        int heightCount;//测量点个数，每个测量点有四个通道，一个通道采集200个点，每个点为2个字节；
    };
    char reserved[FILEHEADERSIZE];
} MY_FILE_HEADER, *pMY_FILE_HEADER;



#endif
