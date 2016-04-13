/*******************************************
作用：动脉硬化检测仪数据包协议的常量定义文件

*******************************************/

#ifndef PACKAGECOMMON_H
#define PACKAGECOMMON_H

#include <QString>

class PackageCommon
{
public:

    // 构造函数
    PackageCommon();

    // 获取数据包的长度
    short getPackageLength();

    // 获取数据包的包头
    QString getPackageHeader();

    // 获取数据包的结尾
    QString getPackageTail();

protected:

    // 获取校验码
    short getSum(const QString &) const;

protected:

    // 固定头文件
    QString m_strHeader;

    // 数据包的长度
    short m_nMessageLength;

    // 数据包的结尾
    QString m_strTail;

    // 校验和的开始位
    short m_nSumStart;

    // 校验和的结束位
    short m_nEndStart;

    // 数据位的起始位
    short m_nDataStart;

    // 数据位的终止位
    short m_nDataEnd;
};

#endif // PACKAGECOMMON_H
