/************************************************************
动脉硬化检测仪数据通信协议的解析类：
数据包分为帧头、数据、校验、帧尾。数值为16进制，共15个字节

示例：24	0E	3B	CMD&DATA	xx	xx	xx	xx	xx	xx	xx	xx	sum	STR	55
注解：
24	0E	3B 为固定帧头。

CMD&DATA
为数据或命令标志。暂时0x01为数据，0x02、0x03为命令。

xx	xx	Xx	xx	xx	xx	xx	xx
为预留数据值。

Sum
为第4个字节到第12个字节的和，取低8位。

STR	55
为固定帧尾。其中STR表示一包字节数长度，默认为15个字节(即0x0F).

（1）数据发送格式：
24	0E	3B	01	TH1	TL1	TH2	TL2	TH3	TL3	TH4	TL4	sum	0F	55
注解：
24	0E	3B
为固定帧头。

01
为数据指令标志。

TH1	TL1	TH2	TL2	TH3	TL3	TH4	TL4
为四路压力值。

顺序依次代表：路1高8位，路1低8位；路2高8位，路2低8位；路3高8位，路3低8位；路4高8位，路4低8位；
Sum
为第4个字节到第12个字节的和，取低8位。

0F	55
为帧尾。其中0F表示一包字节数长度，默认为15(即0x0F).

（2）命令互通格式：
24	0E	3B	02	xx	xx	xx	xx	xx	xx	xx	xx	sum	0F	55
注解：
24	0E	3B
为固定帧头。

02/03
为0x02命令指令标志。

xx	xx	xx	xx	xx	xx	xx	xx
为预留不同命令值。

此命令格式可以减少字节数，命令数据简单无需多位，为了适应数据位的统一性。暂定于此，后续可修改。
Sum
为第4个字节到第12个字节的和，取低8位。

0F	55
为帧尾。其中0F表示一包字节数长度，默认为15(即0x0F).
2、应用举例：
(1)上位机向下位机发送启动命令环节：
上位机发送启动命令，如下：
24	0E	3B	02	01	00	00	00	00	00	00	00	03	0F	55
Sum -> 0x03=0x02+0x01;
下位机回传应答命令，如下：
24	0E	3B	03	01	00	00	00	00	00	00	00	04	0F	55
Sum -> 0x04=0x03+0x01;
(2)上位机向下位机发送停止命令环节：
上位机发送停止命令，如下：
24	0E	3B	02	02	00	00	00	00	00	00	00	04	0F	55
Sum -> 0x04=0x02+0x02;
下位机回传应答命令，如下：
24	0E	3B	03	02	00	00	00	00	00	00	00	05	0F	55
Sum -> 0x05=0x03+0x02;
(3)下位机向上位机发送数据环节：
上位机发送数据，如下：
24	0E	3B	01	07	3F	06	58	03	95	00	81	BE	0F	55
1855=0x07*256+0x3F;	//1855表示压力为185.5mmHg。
1624=0x06*256+0x58;	//1624表示压力为162.4mmHg。
917=0x03*256+0x95; 	//917表示压力为91.7mmHg。
129=0x00*256+0x81;	//129表示压力为12.9mmHg。
Sum -> 0xBE=0x01+0x07+0x3F+0x06+0x58+0x03+0x95+0x00+0x81


以下方案也可选用，具体联调时，可以两个方案任选。暂优选第一个。
通信原则升级版:上述的通信原则也可升级为可变字节的，数据和命令格式可调整。
数据包分为帧头、长度、数据/命令标志、数据，帧尾。数值为16进制。
例如：
24	0E	3B	STR	CMD&DATA	xx	xx	…	…	…	…	xx	xx	sum	55
注解：
24	0E	3B
为固定帧头。

STR
为字符串长度。

CMD&DATA
为数据或命令指令标志。

xx	xx	…	…	…	…	xx	xx
为数据或命令值，长度不定。

Sum
为第4个字节到此的所有字节的和，取低8位。

55
为帧尾。其中0F表示一包字节数长度，默认为15(即0x0F)。


补充：送检模式:
1．上升到相应压力，快速放气
上位机发送命令，如下：
24	0E	3B	03	01	TH	TL	00	00	00	00	00	Sum	0F	55
Sum =0x03+0x01+….;
下位机回传应答命令，如下：
24	0E	3B	04	01	00	00	00	00	00	00	00	Sum	0F	55
Sum =0x04+0x01+…;

2．上升到相应压力，慢速放气：
上位机发送命令，如下：
24	0E	3B	03	02	TH	TL	00	00	00	00	00	Sum	0F	55
Sum =0x03+0x02+….;
下位机回传应答命令，如下：
24	0E	3B	04	02	00	00	00	00	00	00	00	Sum	0F	55
Sum =0x04+0x02+…;
***********************************************************/
#ifndef PARSERECVDATA_H
#define PARSERECVDATA_H

#include "packagecommon.h"

class QString;
class PackageCommon;

class ParseRecvCmd:public PackageCommon
{
public:
    // 构造函数：
    // 参数：  strMessage 收到的消息内容
    ParseRecvCmd(const QString &strMessage = "");

    // 消息的类型
    enum RecvType
    {
       eRECV_DATA =0x01, // 收到数据
       eSend_TEST_CMD = 0x02,// 收到测试结束
       eRECV_TEST_END_CMD = 0x03,// 收到正常测量结束
       eRECV_LIFE_TEST_CMD = 0x04, // 收到寿命测试命令
       eRECV_CTRL_CMD = 0x05, // 阀及泵相关的命令
	   eHANDLE_CMD = 0x06 ,// 握手规则 
	   eINVALID_CMD = 0x10 // 无效命令
    };

	// 03命令下的状态表示
	enum eStatus
	{
		eStart_Status = 0x01, //启动环节
		eStop_Status = 0x02,//停止环节
		eSecond_BoostStatus = 0x03// 二次升压阶段
	};

	// 02命令下的状态表示
	enum eTestStatus
	{
		eTest_Start = 0x01, //启动环节
		eTest_End = 0x02,//停止环节
	};

    // 判断消息头
    bool isHeaderOk() const;

    // 判断包长是否正确
    bool isLengthOk() const;

    // 判断消息末尾是否正确
    bool isTailOk() const;

    // 判断SUM位是否正确
    bool isSumOk() const;

    // 消息的类型
    RecvType recvDataType() const;

    // 获取收到的压力数据
    // 参数：[OUT] nArrPressure 收到的压力数据，依次为右臂、左臂、右踝、左踝
    void getPressure(int *pArrPressure) const;

    // 判断是否为正常测试开始的命令
    bool isNormalStartCmd();

    // 判断是否为正常测试停止命令
    bool isNormalEndCmd();

    // 判断是否为寿命测试开始的命令
    bool isLifeSpanStartCmd();

    // 判断是否为寿命停止命令
    bool isLifeSpanEndCmd();

    // 判断是否是阀门闭气命令
    bool isSWitchOn();

    // 判断是否为阀门泄气命令
    bool isSwitchOff();


    // 判断是否为泵1打开命令
    // 参数：[IN] bIsOn 若为true,
    //                 表示判断是否是打开命令,否则判断是否为关闭命令
    bool isPrump1OnOff(bool bIsOn = true);


    // 判断是否为泵2打开命令
    // 参数：[IN] bIsOn 若为true,
    //                 表示判断是否是打开命令,否则判断是否为关闭命令
    bool isPrump2OnOff(bool bIsOn = true);

    // 判断是否为泵3打开命令
    // 参数：[IN] bIsOn 若为true,
    //                 表示判断是否是打开命令,否则判断是否为关闭命令
    bool isPrump3OnOff(bool bIsOn = true);

    // 判断是否为泵4打开命令
    // 参数：[IN] bIsOn 若为true,
    //                 表示判断是否是打开命令,否则判断是否为关闭命令
    bool isPrump4OnOff(bool bIsOn = true);

    // 获取阀门及泵的开关命令
    short getSwitchPrumpStatus();

	// 是否为结束命令,若返回true表示结束命令，否则，表示开始命令
	bool isEndCmd();

	// 获取命令标志后的状态：01表示启动测试，02表示停止命令，03表示二次升压阶段
	short getStatus();
private:

    // 数据包的内容：以16进制显示
    QString m_strMessage;
};

#endif // PARSERECVDATA_H
