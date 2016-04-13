#include "./header/stdafx.h"
#include "./header/PackageCommon.h"

PackageCommon::PackageCommon():
    m_strHeader("240e3b"),m_nMessageLength(30),m_strTail("0f55"),
    m_nSumStart(6),m_nEndStart(24),m_nDataStart(8),m_nDataEnd(24)
{
}

short PackageCommon::getPackageLength()
{
    return m_nMessageLength;
}

// 获取数据包的包头
QString PackageCommon::getPackageHeader()
{
    return m_strHeader;
}

// 获取数据包的结尾
QString PackageCommon::getPackageTail()
{
    return m_strTail;
}

// 获取校验码
short PackageCommon::getSum(const QString &strMessage) const
{
    // 计算接受到的数据中，从第四个字节到第12个字节的和
    int nRet = 0;
    bool bIsOK =false;
    for (int i = m_nSumStart; i < m_nEndStart; i += 2)
    {
        nRet += strMessage.mid(i, 2).toInt(&bIsOK, 16);
    }

    return nRet &= 0x00ff;
}
