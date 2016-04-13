#ifndef _Q_CONST_VALUE_H
#define _Q_CONST_VALUE_H

// ABI 正常范围的最小值（0.9-1.4）
const float fNORMALABI_RANGE_MIN = 0.9f;
const float fNORMALABI_RANGE_MAX = 1.4f;

// PWV 正常范围的最大值（800-1400）
const int nNORMALPWV_RANGE_MIN = 800;
const int nNormalPWV_RANGE_MAX = 1400;

// 默认第一次测量的最大值
const int nDEFAULT_MAX_VALUE = 1990;

// 默认第一次下降到的最小值
const int nDEFAULT_MIN_VALUE = 71;

const int nMIN_ARM_PRESSURE = 20;
const int nMIN_ANKLE_VALUE = 22;

// 滤波相关的参数
const float nCURR_WITH_PRE_DIFF = 0.001f;
// 波谷与波峰的最小差值 
const float fMIN_WAVE_AMPLITUDE = 2.0f;

// 波形采样率
const int nPRESSURE_SAMPLE_RATE = 800;
// 由于脉搏波的频率为0.5Hz-5Hz，因此每个脉搏波的时间范围为0.2s-2s,
// 所以最小脉搏波的压力值的个数为160 = (0.2s/(1/800)) 至 1600 = (2s/(1/800))
const int nMIN_WAVE_PRESSURECOUNT = 160;
const int nMAX_WAVE_PRESSURECOUNT = 1600;

// 利用波谷法找到每个脉搏波的上升曲线，每次上升时，判断连续30个点是1
const int nSLOPE_CONTINUOUS_COUNT = 30; 

#endif