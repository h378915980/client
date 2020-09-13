#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <QFile>
#include <QDir>
#include <QTime>



class FileOperator
{
public:
    FileOperator();
    static void createDir(QString fileName, QString filePath);
    static void createPath(QString path);
    static QString createDirWithCurrentTime(QString fileName, QString filePath);   // 返回创建文件夹的名称
    static QFileInfoList getFileNames(QString filePath);  //返回当前路径下的所有文件名
    static bool isDirExist(QString path);
    static bool isFileExist(QString filePath);
};

#endif // FILEOPERATOR_H
