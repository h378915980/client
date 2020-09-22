#ifndef MD5CHECKING_H
#define MD5CHECKING_H

#include"qbytearray.h"
#include<qfile.h>
#include<qdebug.h>
#include<QCryptographicHash>
#include <QApplication>

QByteArray getFileMd5(QString filePath);

#endif // MD5CHECKING_H
