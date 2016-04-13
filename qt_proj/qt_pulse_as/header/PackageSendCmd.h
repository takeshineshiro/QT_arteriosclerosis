/*******************************************
作用：产生动脉硬化检测仪上位机向下位机发送的命令

*******************************************/

#ifndef PACKAGESENDDATA_H
#define PACKAGESENDDATA_H

#include "PackageCommon.h"

class PackageSendCmd:public PackageCommon
{
public:
    // 构造函数
    PackageSendCmd();

    // 获取要发送的启动命令
    QString getNoramlStart() const;

    // 获取停止的命令
    QString getNomalEnd() const;

    // 根据传入的压力值，获取快速放气的命令
    // 参数：[IN] strPressure  十六进制压力值,占2个字节
    QString getFastStart(const QString &strPressure) const;

    // 根据传入的压力值，获取慢速放气的命令
    // 参数：[IN] strPressure  十六进制压力值,占2个字节
    QString getSlowStart(const QString &strPressure) const;

    // 寿命测试开始
    QString getLifeSpanStart();

    // 寿命测试结束
    QString getLifeSpanEnd();

    // 获取阀门闭气及泄气的命令
    // 参数：[IN] bIsOn 若为true,则获取阀门闭气的命令，否则为阀门泄气的命令
    QString getSwitchOnOff(bool bIsOn = true);

    // 获取泵1的打开关闭命令
    // 参数：[IN] bIsOn 若为true,则获取泵1打开命令，否则为泵1关闭命令
    QString getPrump1OnOff(bool bIsOn = true);

    // 获取泵2的打开关闭命令
    // 参数：[IN] bIsOn 若为true,则获取泵2打开命令，否则为泵2关闭命令
    QString getPrump2OnOff(bool bIsOn = true);

    // 获取泵3的打开关闭命令
    // 参数：[IN] bIsOn 若为true,则获取泵3打开命令，否则为泵3关闭命令
    QString getPrump3OnOff(bool bIsOn = true);

    // 获取泵4的打开关闭命令
    // 参数：[IN] bIsOn 若为true,则获取泵4打开命令，否则为泵4关闭命令
    QString getPrump4OnOff(bool bIsOn = true);


    // 获取阀1的快速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getFastSwitch1(bool bIsOn = true);

    // 获取阀2的快速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getFastSwitch2(bool bIsOn = true);

    // 获取阀3的快速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getFastSwitch3(bool bIsOn = true);

    // 获取阀4的快速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getFastSwitch4(bool bIsOn = true);

    // 获取阀1的慢速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getSlowSwitch1(bool bIsOn = true);

    // 获取阀2的慢速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getSlowSwitch2(bool bIsOn = true);

    // 获取阀3的慢速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getSlowSwitch3(bool bIsOn = true);

    // 获取阀4的慢速放气命令
    // 参数：[IN] bIsOn 若为true,则闭气，否则泄气
    QString getSlowSwitch4(bool bIsOn = true);

	// 获取动态升压的命令
	QString getBoostMeasureCmd(const int &nRightArm = 220, const int &nLeftArm = 220,
		                       const int &nRightAnkle = 240, const int &nLeftAnkle = 240);

	// 获取和下位机握手的命令，验证下位机是否可用
	QString getHandleCmd();
};

#endif // PACKAGESENDDATA_H
