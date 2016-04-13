#include "./header/stdafx.h"
#include <QString>
#include <QDebug>
#include "./header/PackageRecvCmd.h"

ParseRecvCmd::ParseRecvCmd(const QString &strMessage):
    m_strMessage(strMessage)
{

}

bool ParseRecvCmd::isHeaderOk() const
{
   return  !m_strMessage.isEmpty() && m_strHeader == m_strMessage.left(m_strHeader.length());
}

// 判断包长是否正确
bool ParseRecvCmd::isLengthOk() const
{
    return m_nMessageLength == m_strMessage.length();
}

// 判断消息末尾是否正确
bool ParseRecvCmd::isTailOk() const
{
    return m_strTail == m_strMessage.right(m_strTail.length());
}

// 判断SUM位是否正确
bool ParseRecvCmd::isSumOk() const
{
    // 获取校验码SUM
    int nRet = getSum(m_strMessage);

    // 数据包的验证码
    bool bIsOK = false;
    int nSum = m_strMessage.mid(m_nEndStart, 2).toInt(&bIsOK, 16);

    return nSum == nRet;
}

// 消息的类型
ParseRecvCmd::RecvType ParseRecvCmd::recvDataType() const
{
    // 获取数据包的类型
    short nType = m_strMessage.mid(m_strHeader.length(), 2).toShort();
    RecvType eType = eRECV_DATA;
    switch(nType)
    {
    // 测试停止
    case eSend_TEST_CMD:
        eType = eSend_TEST_CMD;
        break;

    // 正常测量
    case eRECV_TEST_END_CMD:
        eType = eRECV_TEST_END_CMD;
        break;

    // 寿命测量命令
    case eRECV_LIFE_TEST_CMD:
        eType = eRECV_LIFE_TEST_CMD;
        break;

    // 充放气命令
    case eRECV_CTRL_CMD:
        eType = eRECV_CTRL_CMD;
        break;

	case eHANDLE_CMD:
		eType = eHANDLE_CMD;
		break;

    default:
        eType = eRECV_DATA;
    }

    return eType;
}

void ParseRecvCmd::getPressure(int *pArrPressure) const
{
    // 空指针判断
    if (NULL == pArrPressure)
    {
        return;
    }

    bool bIsOk = false;
    for(int i = m_nDataStart; i < m_nDataEnd; i += 4)
    {
        //qDebug()<<m_strMessage.mid(i, 4);
		int nPressure = m_strMessage.mid(i, 4).toInt(&bIsOk, 16);
        
		if(bIsOk)
		{
			*pArrPressure++ = nPressure;
		}
		else
		{
			*pArrPressure++ = 0;
		}
		bIsOk = false;
    }
}

// 判断是否为正常测试开始的命令
bool ParseRecvCmd::isNormalStartCmd()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "01";
}

bool ParseRecvCmd::isEndCmd()
{
	return m_strMessage.mid(m_strHeader.length() + 2, 2) == "02";
}

// 判断是否为正常测试停止命令
bool ParseRecvCmd::isNormalEndCmd()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "02";
}

// 判断是否为寿命测试开始的命令
bool ParseRecvCmd::isLifeSpanStartCmd()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "01";
}

// 判断是否为寿命测试停止命令
bool ParseRecvCmd::isLifeSpanEndCmd()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "02";
}

// 判断是否是阀门闭气命令
bool ParseRecvCmd::isSWitchOn()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "0a";
}

// 判断是否为阀门泄气命令
bool ParseRecvCmd::isSwitchOff()
{
    return m_strMessage.mid(m_strHeader.length() + 2, 2) == "09";
}

// 判断是否为泵1打开命令
// 参数：[IN] bIsOn 若为true,
//                 表示判断是否是打开命令,否则判断是否为关闭命令
bool ParseRecvCmd::isPrump1OnOff(bool bIsOn)
{
    bool bRet = true;
    if(bIsOn)
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "01";
    }
    else
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "02";
    }
    return bRet;
}


// 判断是否为泵2打开命令
// 参数：[IN] bIsOn 若为true,
//                 表示判断是否是打开命令,否则判断是否为关闭命令
bool ParseRecvCmd::isPrump2OnOff(bool bIsOn)
{
    bool bRet = true;
    if(bIsOn)
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "03";
    }
    else
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "04";
    }
    return bRet;
}


// 判断是否为泵3打开命令
// 参数：[IN] bIsOn 若为true,
//                 表示判断是否是打开命令,否则判断是否为关闭命令
bool ParseRecvCmd::isPrump3OnOff(bool bIsOn)
{
    bool bRet = true;
    if(bIsOn)
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "05";
    }
    else
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "06";
    }
    return bRet;
}


// 判断是否为泵4打开命令
// 参数：[IN] bIsOn 若为true,
//                 表示判断是否是打开命令,否则判断是否为关闭命令
bool ParseRecvCmd::isPrump4OnOff(bool bIsOn)
{
    bool bRet = true;
    if(bIsOn)
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "07";
    }
    else
    {
        bRet = m_strMessage.mid(m_strHeader.length() + 2, 2) == "08";
    }
    return bRet;
}

// 获取阀门及泵的开关命令
short ParseRecvCmd::getSwitchPrumpStatus()
{
    bool bIsOk = false;
    return m_strMessage.mid(m_strHeader.length() + 2, 2).toInt(&bIsOk, 16);
}

short ParseRecvCmd::getStatus()
{
	return m_strMessage.mid(m_strHeader.length() + 2, 2).toInt();
}
