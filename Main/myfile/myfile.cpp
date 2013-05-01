#include "./myfile/myfile.h"

MyFile::MyFile(void)
{
    this->file = NULL;
    isSaveAs = true;
}

MyFile::~MyFile(void)
{
    Close();
}

/*****************************************************
  *类型：公有成员函数
  *功能：将文件头写入文件中,文件头记录本文件相关信息.
  ***************************************************/
bool MyFile::WriteFileHead(const MY_FILE_HEADER &fileHeader)
{
    if (!this->file)
        return false;
    fseek(this->file, 0, SEEK_SET);
    fwrite(&fileHeader, FILEHEADERSIZE, 1, this->file);
    fflush(this->file);
    fseek(this->file, 0, SEEK_END);

    isSaveAs = false;
    return true;
}

/*****************************************************
  *类型：公有成员函数
  *功能：将当前文件内容另存为另一个文件.
  ***************************************************/
bool MyFile::SaveDataToFile(QString fileName, QList<QList<qint16> > &dataNodeListRaw, QList<qreal> &dataNodeHeight)
{
    MyDataNode node;

    if (!fileName.isNull())
    {
        if (QFile::exists(fileName))
        {
            QFile::remove(fileName);
        }

    }
    else
    {
        return false;
    }

    FILE *saveFile = fopen(fileName.toAscii(), "w+");
    fclose(saveFile);
    saveFile = fopen(fileName.toAscii(), "r+b");
    fseek(saveFile, FILEHEADERSIZE, SEEK_SET);
    for (int i = 0; i < dataNodeListRaw.count(); i++)
    {
        int index = 0;
        for (int section = 0; section < SECTIONCOUNT; section++)
        {
            for (int point = 0; point < POINTCOUNT; point++)
            {
                node.data[section][point] = dataNodeListRaw.at(i).at(index++);
            }
        }
        node.height = dataNodeHeight.at(i);

        fwrite(&node, sizeof(node), 1, saveFile);
    }


    fflush(saveFile);
    fflush(saveFile);
    fclose(saveFile);
    return true;
}


/*****************************************************
  *类型：公有成员函数
  *功能：从文件中加载数据,当加载数据成功时,就可以调用GetNextDataNode获取
  *下一个数据节点.
  ***************************************************/
bool MyFile::LoadDataFromFile(QString &fileName)
{
    this->fileName = fileName;
    if (!fileName.isNull())
    {
        if (this->file)
        {
            fflush(this->file);
            fclose(this->file);
        }
        this->file = fopen(this->fileName.toAscii(), "r+b");
        if (!this->file)
            return false;
        fseek(this->file, FILEHEADERSIZE, SEEK_SET);
        return true;
    }
    else
    {
        return false;
    }
}

/*****************************************************
  *类型：公有成员函数
  *功能：将指定的数据节点写到文件中.
  ***************************************************/
bool MyFile::WriteData(const MyDataNode& node)
{
    if (!this->file)
    {
        return false;
    }
    fwrite(&node, sizeof(node), 1, this->file);
    fflush(this->file);

    isSaveAs = false;
    return true;
}


/*****************************************************
  *类型：公有成员函数
  *功能：使用GetNextDataNode方法可以以历遍的方式获取数据节点.
  ***************************************************/
bool MyFile::GetNextDataNode(MyDataNode& node)
{
//    qDebug() << ftell(this->file);
//    qDebug() << feof(this->file);
    if ( (ftell(this->file) < FILEHEADERSIZE) || fread(&node, sizeof(node), 1, this->file) == 0)
    {
        return false;
    }
    return true;
}

/*****************************************************
  *类型：公有成员函数
  *功能：打开指定的文件.注意,此方法会将打开的文件内容清零.
  ***************************************************/
bool MyFile::Open(const QString& fileName)
{
    this->file = fopen(fileName.toAscii(), "w+");
    fclose(this->file);
    this->file = fopen(fileName.toAscii(), "r+b");
    MY_FILE_HEADER fileHeader;
    WriteFileHead(fileHeader);
    isSaveAs = true;
    this->fileName = fileName;
    return true;
}

/*****************************************************
  *类型：公有成员函数
  *功能：关闭类中维护的文件指针.
  ***************************************************/
bool MyFile::Close()
{
    if (this->file)
    {
        fflush(this->file);
        fflush(this->file);
        fclose(this->file);
        this->file = NULL;
        return true;
    }
    return false;
}

/*****************************************************
  *类型：公有成员函数
  *功能：定位到指定数据节点的开始位置.
  ***************************************************/
bool MyFile::SeekToNodeIndexBegin(int NodeIndex)
{
    if (NodeIndex >= GetNodeCount())
        return false;
    if (NodeIndex < 0)
        return false;
    if (!this->file)
        return false;
    fseek(file, FILEHEADERSIZE + DATANODESIZE * NodeIndex, SEEK_SET );
    return true;
}


/*****************************************************
  *类型：公有成员函数
  *功能：获取当前数据在文件中的索引位置.
  ***************************************************/
int MyFile::GetCurrentNodeIndex()
{
    if (!this->file)
        return 0;
    long offset = ftell(this->file);
    if (offset > FILEHEADERSIZE)
    {
        offset -= FILEHEADERSIZE;
    }
    return offset / DATANODESIZE;
}

/*****************************************************
  *类型：公有成员函数
  *功能：获取文件中数据节点的总数.
  ***************************************************/
int MyFile::GetNodeCount()
{

    long offsetOld = ftell(this->file);
    fseek(this->file, 0, SEEK_END);
    long offsetNow = ftell(this->file);
    fseek(this->file, offsetOld, SEEK_SET);
    if (offsetNow > FILEHEADERSIZE)
    {
        offsetNow -= FILEHEADERSIZE;
        return offsetNow / DATANODESIZE;
    }
    return 0;

}

/*****************************************************
  *类型：公有成员函数
  *功能：获取文件头.
  ***************************************************/
bool MyFile::GetFileHead(MY_FILE_HEADER &fileHeader)
{
    if (!this->file)
        return false;
    fseek(this->file, 0, SEEK_SET);
    fread(&fileHeader, FILEHEADERSIZE, 1, this->file);
    return true;
}

/*****************************************************
  *类型：公有成员函数
  *功能：清除类中维护的文件指针指向的文件的内容.
  ***************************************************/
bool MyFile::ClearFileContent()
{
    if (this->file)
        Close();
    this->file = fopen(this->fileName.toAscii(), "w+");
    fclose(this->file);
    this->file = fopen(this->fileName.toAscii(), "r+b");
    fseek(this->file, 0, SEEK_SET);
}


/*****************************************************
  *类型：公有成员函数
  *功能：返回当前文件指针指向的文件是否已经另存为.
  ***************************************************/
bool MyFile::IsSaveAs()
{
    return isSaveAs;
}

bool MyFile::IsOpen()
{
    return this->file ? true : false;
}

bool MyFile::WriteFinishState()
{
    if (this->IsOpen())
    {
        fclose(this->file);
        this->file = fopen(this->fileName.toAscii(), "r+");
        fseek(this->file, 0, SEEK_END);
        fwrite(FILEFINISHSTATE, sizeof(FILEFINISHSTATE), 1, this->file);
        fclose(this->file);
        this->file = fopen(this->fileName.toAscii(), "r+b");
        fseek(this->file, 0, SEEK_END);
        return true;
    }
    return false;
}

bool MyFile::IsWriteFinishState(QString fileName)
{
    if (QFile::exists(fileName))
    {
        FILE *myFile = fopen(fileName.toAscii(), "r+");
        fseek(myFile, -sizeof(FILEFINISHSTATE), SEEK_END);
        char *c = new char[sizeof(FILEFINISHSTATE)];
        fread(c, sizeof(FILEFINISHSTATE), 1, myFile);
        fclose(myFile);
        if (0 == QString::compare(c, FILEFINISHSTATE))
                return true;
    }

    return false;
}
