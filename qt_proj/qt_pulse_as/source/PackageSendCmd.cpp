#include "./header/stdafx.h"
#include "./header/PackageSendCmd.h"

PackageSendCmd::PackageSendCmd()
{

}

// 根据传入的压力值，获取快速放气的命令
// 参数：[IN] strPressure  压力值
QString PackageSendCmd::getFastStart(const QString &strPressure) const
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(3, 16).rightJustified(2,'0');

    // 数据
    strStart += QString::number(1, 16).rightJustified(2,'0');
    strStart += strPressure;
    strStart += "000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 根据传入的压力值，获取慢速放气的命令
// 参数：[IN] strPressure  压力值
QString PackageSendCmd::getSlowStart(const QString &strPressure) const
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(3, 16).rightJustified(2,'0');

    // 数据
    strStart += QString::number(2, 16).rightJustified(2,'0');
    strStart += strPressure;
    strStart += "000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 寿命测试开始
QString PackageSendCmd::getLifeSpanStart()
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(3, 16).rightJustified(2,'0');

    // 数据
    strStart += QString::number(1, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 寿命测试结束
QString PackageSendCmd::getLifeSpanEnd()
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(3, 16).rightJustified(2,'0');

    // 数据
    strStart += QString::number(2, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取泵1的打开关闭命令
// 参数：[IN] bIsOn 若为true,则获取泵1打开命令，否则为泵1关闭命令
QString PackageSendCmd::getPrump1OnOff(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 2;
    // 泄气
    if(bIsOn)
    {
        nCmd = 1;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取泵2的打开关闭命令
// 参数：[IN] bIsOn 若为true,则获取泵2打开命令，否则为泵2关闭命令
QString PackageSendCmd::getPrump2OnOff(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 4;
    // 泄气
    if(bIsOn)
    {
        nCmd = 3;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取泵3的打开关闭命令
// 参数：[IN] bIsOn 若为true,则获取泵3打开命令，否则为泵3关闭命令
QString PackageSendCmd::getPrump3OnOff(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 6;
    // 泄气
    if(bIsOn)
    {
        nCmd = 5;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取泵4的打开关闭命令
// 参数：[IN] bIsOn 若为true,则获取泵4打开命令，否则为泵4关闭命令
QString PackageSendCmd::getPrump4OnOff(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 8;
    // 泄气
    if(bIsOn)
    {
        nCmd = 7;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀门闭气及泄气的命令
// 参数：[IN] bIsOn 若为true,则获取阀门闭气的命令，否则为阀门泄气的命令
QString PackageSendCmd::getSwitchOnOff(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 10;
    // 泄气
    if(!bIsOn)
    {
        nCmd = 9;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取要发送的启动命令
QString PackageSendCmd::getNoramlStart() const
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(2, 16).rightJustified(2,'0');

    // 启动的数据
    strStart += QString::number(1, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    // 校验位SUM
    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取停止的命令
QString PackageSendCmd::getNomalEnd() const
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(2, 16).rightJustified(2,'0');

    // 停止的数据
    strStart += QString::number(2, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    // 校验位SUM
    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀1的快速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getFastSwitch1(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x0c;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x0b;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀2的快速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getFastSwitch2(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x0e;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x0d;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀3的快速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getFastSwitch3(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x10;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x0f;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀4的快速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getFastSwitch4(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x12;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x11;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀1的慢速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getSlowSwitch1(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x14;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x13;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀2的慢速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getSlowSwitch2(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x16;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x15;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀3的慢速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getSlowSwitch3(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x18;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x17;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取阀4的慢速放气命令
// 参数：[IN] bIsOn 若为true,则闭气，否则泄气
QString PackageSendCmd::getSlowSwitch4(bool bIsOn)
{
    // 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(4, 16).rightJustified(2,'0');

    int nCmd = 0x1a;
    // 泄气
    if(bIsOn)
    {
        nCmd = 0x19;
    }

    strStart += QString::number(nCmd, 16).rightJustified(2,'0');
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

// 获取动态升压的命令
QString PackageSendCmd::getBoostMeasureCmd(const int &nRightArm, const int &nLeftArm,
		                       const int &nRightAnkle, const int &nLeftAnkle)
{
	// 消息头
    QString strStart(m_strHeader);

    // 开始命令
    strStart += QString::number(2, 16).rightJustified(2,'0');

    strStart += QString::number(3, 16).rightJustified(2,'0');


	int nArm = qMax(nRightArm, nLeftArm);
	int nAnkle = qMax(nRightAnkle, nLeftAnkle);
	strStart += QString::number(nArm, 16).rightJustified(2,'0');
	strStart += QString::number(nAnkle, 16).rightJustified(2,'0');
    strStart += "0000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}

QString PackageSendCmd::getHandleCmd()
{
	// 消息头
    QString strStart(m_strHeader);

    // 握手标志
    strStart += QString::number(5, 16).rightJustified(2,'0');

    strStart += QString::number(1, 16).rightJustified(2,'0');
	
    strStart += "00000000000000";

    short nSum = getSum(strStart);
    // 校验位SUM
    strStart += QString::number(nSum, 16).rightJustified(2,'0');

    // 消息末尾
    strStart += m_strTail;

    return strStart;
}
