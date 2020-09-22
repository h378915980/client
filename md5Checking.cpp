#include "md5Checking.h"

QByteArray getFileMd5(QString filePath)  
{
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    /*QCryptographicHash类提供了一种生成加密哈希的方法。
    QCryptographicHash可用于生成二进制或文本数据的密码散列。
    目前支持MD4、MD5和SHA-1。
    */

    QCryptographicHash ch(QCryptographicHash::Md5);

    //文件总大小
    quint64 totalBytes = 0;
    //已读取的文件大小
    quint64 bytesWritten = 0;
    //等待读取的文件大小
    quint64 bytesToWrite = 0;
    //每次读取的文件大小
    quint64 loadSize = 1024 * 4;

    QByteArray buf;

    //获取文件大小
    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    //循环读取文件
    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    //关闭文件
    localFile.close();

    QByteArray md5 = ch.result();

    return md5;
}

