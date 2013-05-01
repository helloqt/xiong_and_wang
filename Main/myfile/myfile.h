#ifndef MYFILE_H
#define MYFILE_H

#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>

#include "UserDef.h"
#include <QDebug>
#include <QList>


class MyFile
{
public:
    MyFile(void);
    ~MyFile(void);

    /************************************************************************/
    /* 变量                                                                     */
    FILE *file;//保存当前打开文件指针
    QString fileName;//保存当前打开的文件名
    bool isSaveAs;//当为false时,表示当前文件没有另存为.
    /************************************************************************/


    /************************************************************************/
    /* 方法                                                                     */
    bool WriteFileHead(const MY_FILE_HEADER &fileHeader);
    static bool SaveDataToFile(QString fileName, QList<QList<qint16> > &dataNodeListRaw, QList<qreal> &dataNodeHeight);
    bool LoadDataFromFile(QString &fileName);
    bool WriteData(const MyDataNode& node);
    bool GetNextDataNode(MyDataNode& node);
    bool GetDataNode(int height, MyDataNode& node);
    bool Open(const QString& fileName);
    bool Close();
    bool SeekToNodeIndexBegin(int heightIndex);
    int GetCurrentNodeIndex();
    int GetNodeCount();
    bool GetFileHead(MY_FILE_HEADER &fileHeader);
    bool ClearFileContent();
    bool IsSaveAs();
    bool IsOpen();
    bool WriteFinishState();
    static bool IsWriteFinishState(QString fileName);
private:



    /************************************************************************/
};


#endif // MYFILE_H
