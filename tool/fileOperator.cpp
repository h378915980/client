#include "fileOperator.h"

FileOperator::FileOperator()
{

}
void FileOperator::createDir(QString fileName, QString filePath) {
    QDir dir(filePath);
    dir.mkpath(fileName);
}

void FileOperator::createPath(QString path)
{
    QDir dir;
    dir.mkpath(path);
}
QString FileOperator::createDirWithCurrentTime(QString fileName, QString filePath) {  //创建一个带有当前时间的文件夹

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("-yyyy.MM.dd-hh：mm：ss");
    QDir dir(filePath);

    dir.mkpath("./" + fileName + current_date);

    return fileName + current_date;
}


QFileInfoList FileOperator::getFileNames(QString filePath)
{
    QDir dir(filePath);
    QFileInfoList fileList = dir.entryInfoList();
    return fileList;
    
}

bool FileOperator::isDirExist(QString path)
{
    QDir dir;
    if(dir.exists(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool FileOperator::isFileExist(QString filePath)
{
    QFileInfo file(filePath);
    if(file.exists())
    {
        return true;
    }
    else
    {
        return false;
    }
}
