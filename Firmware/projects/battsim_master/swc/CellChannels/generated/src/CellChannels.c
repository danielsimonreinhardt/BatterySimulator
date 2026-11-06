/*
 * File: CellChannels.c
 *
 * Code generated for Simulink model 'CellChannels'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CellChannels.h"
#include <math.h>
#include "Platform_Types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_CellChannels_T CellChannels_ARID_DEF;/* '<Root>/ActualVolt_V' */

/* Model step function for TID1 */
void CellTask10ms(void)                /* Explicit Task: CellTask10ms */
{
  float32 rtb_TmpSignalConversionAtSFunctionInport1[14];
  float32 tmpRead_3[Rte_SysCon_CellCount];
  float32 tmpRead_6[Rte_SysCon_CellCount];
  float32 tmpRead_7[Rte_SysCon_CellCount];
  float32 tmpRead_8[Rte_SysCon_CellCount];
  float32 tmpRead_9[Rte_SysCon_CellCount];
  float32 e;
  float32 mv;
  float32 tmpRead_a;
  float32 tmpRead_b;
  float32 tmpRead_c;
  float32 tmpRead_d;
  float32 v;
  sint32 i;
  sint32 k;
  uint32 rtb_TmpSignalConversionAtSFunctionInport3[14];
  uint32 qY;
  uint16 duty[14];
  uint8 rtb_TmpSignalConversionAtSFunctionInport2[14];
  uint8 rtb_TmpSignalConversionAtSFunctionInport4[14];
  uint8 state[14];
  uint8 tmpRead_1;
  uint8 tmpRead_4;
  boolean en[14];
  boolean selLed[14];
  boolean below;
  boolean below_tmp;
  boolean clipped;
  boolean commanded;
  boolean ctrlActive;
  boolean exitg1;
  boolean holding;
  boolean tmp;
  boolean tmpRead;
  boolean tmpRead_0;
  boolean tmpRead_2;
  boolean tmpRead_5;

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell14Qlf'
   */
  (void)Rte_Read_UCell14Qlf_UCell14Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[13]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell14_mV'
   */
  (void)Rte_Read_UCell14_mV_UCell14_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[13]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell14Enable'
   */
  (void)Rte_Read_Cell14Enable_Cell14Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[13]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell14VoltSet_V'
   */
  (void)Rte_Read_Cell14VoltSet_V_Cell14VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[13]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell13Qlf'
   */
  (void)Rte_Read_UCell13Qlf_UCell13Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[12]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell13_mV'
   */
  (void)Rte_Read_UCell13_mV_UCell13_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[12]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell13Enable'
   */
  (void)Rte_Read_Cell13Enable_Cell13Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[12]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell13VoltSet_V'
   */
  (void)Rte_Read_Cell13VoltSet_V_Cell13VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[12]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell12Qlf'
   */
  (void)Rte_Read_UCell12Qlf_UCell12Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[11]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell12_mV'
   */
  (void)Rte_Read_UCell12_mV_UCell12_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[11]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell12Enable'
   */
  (void)Rte_Read_Cell12Enable_Cell12Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[11]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell12VoltSet_V'
   */
  (void)Rte_Read_Cell12VoltSet_V_Cell12VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[11]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell11Qlf'
   */
  (void)Rte_Read_UCell11Qlf_UCell11Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[10]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell11_mV'
   */
  (void)Rte_Read_UCell11_mV_UCell11_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[10]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell11Enable'
   */
  (void)Rte_Read_Cell11Enable_Cell11Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[10]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell11VoltSet_V'
   */
  (void)Rte_Read_Cell11VoltSet_V_Cell11VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[10]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell10Qlf'
   */
  (void)Rte_Read_UCell10Qlf_UCell10Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[9]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell10_mV'
   */
  (void)Rte_Read_UCell10_mV_UCell10_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[9]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell10Enable'
   */
  (void)Rte_Read_Cell10Enable_Cell10Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[9]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell10VoltSet_V'
   */
  (void)Rte_Read_Cell10VoltSet_V_Cell10VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[9]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell09Qlf'
   */
  (void)Rte_Read_UCell09Qlf_UCell09Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[8]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell09_mV'
   */
  (void)Rte_Read_UCell09_mV_UCell09_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[8]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell09Enable'
   */
  (void)Rte_Read_Cell09Enable_Cell09Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[8]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell09VoltSet_V'
   */
  (void)Rte_Read_Cell09VoltSet_V_Cell09VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[8]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell08Qlf'
   */
  (void)Rte_Read_UCell08Qlf_UCell08Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[7]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell08_mV'
   */
  (void)Rte_Read_UCell08_mV_UCell08_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[7]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell08Enable'
   */
  (void)Rte_Read_Cell08Enable_Cell08Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[7]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell08VoltSet_V'
   */
  (void)Rte_Read_Cell08VoltSet_V_Cell08VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[7]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell07Qlf'
   */
  (void)Rte_Read_UCell07Qlf_UCell07Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[6]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell07_mV'
   */
  (void)Rte_Read_UCell07_mV_UCell07_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[6]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell07Enable'
   */
  (void)Rte_Read_Cell07Enable_Cell07Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[6]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell07VoltSet_V'
   */
  (void)Rte_Read_Cell07VoltSet_V_Cell07VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[6]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell06Qlf'
   */
  (void)Rte_Read_UCell06Qlf_UCell06Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[5]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell06_mV'
   */
  (void)Rte_Read_UCell06_mV_UCell06_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[5]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell06Enable'
   */
  (void)Rte_Read_Cell06Enable_Cell06Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[5]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell06VoltSet_V'
   */
  (void)Rte_Read_Cell06VoltSet_V_Cell06VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[5]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell05Qlf'
   */
  (void)Rte_Read_UCell05Qlf_UCell05Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[4]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell05_mV'
   */
  (void)Rte_Read_UCell05_mV_UCell05_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[4]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell05Enable'
   */
  (void)Rte_Read_Cell05Enable_Cell05Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[4]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell05VoltSet_V'
   */
  (void)Rte_Read_Cell05VoltSet_V_Cell05VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[4]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell04Qlf'
   */
  (void)Rte_Read_UCell04Qlf_UCell04Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[3]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell04_mV'
   */
  (void)Rte_Read_UCell04_mV_UCell04_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[3]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell04Enable'
   */
  (void)Rte_Read_Cell04Enable_Cell04Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[3]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell04VoltSet_V'
   */
  (void)Rte_Read_Cell04VoltSet_V_Cell04VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[3]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell03Qlf'
   */
  (void)Rte_Read_UCell03Qlf_UCell03Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[2]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell03_mV'
   */
  (void)Rte_Read_UCell03_mV_UCell03_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[2]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell03Enable'
   */
  (void)Rte_Read_Cell03Enable_Cell03Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[2]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell03VoltSet_V'
   */
  (void)Rte_Read_Cell03VoltSet_V_Cell03VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[2]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell02Qlf'
   */
  (void)Rte_Read_UCell02Qlf_UCell02Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[1]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell02_mV'
   */
  (void)Rte_Read_UCell02_mV_UCell02_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[1]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell02Enable'
   */
  (void)Rte_Read_Cell02Enable_Cell02Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[1]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell02VoltSet_V'
   */
  (void)Rte_Read_Cell02VoltSet_V_Cell02VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[1]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell01Qlf'
   */
  (void)Rte_Read_UCell01Qlf_UCell01Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[0]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/UCell01_mV'
   */
  (void)Rte_Read_UCell01_mV_UCell01_mV
    (&rtb_TmpSignalConversionAtSFunctionInport3[0]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell01Enable'
   */
  (void)Rte_Read_Cell01Enable_Cell01Enable
    (&rtb_TmpSignalConversionAtSFunctionInport2[0]);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/Cell01VoltSet_V'
   */
  (void)Rte_Read_Cell01VoltSet_V_Cell01VoltSet_V
    (&rtb_TmpSignalConversionAtSFunctionInport1[0]);

  /* Inport: '<Root>/CalMeasGain' */
  (void)Rte_Read_CalMeasGain_CalMeasGain(tmpRead_9);

  /* Inport: '<Root>/CalMeasOffset_V' */
  (void)Rte_Read_CalMeasOffset_V_CalMeasOffset_V(tmpRead_8);

  /* Inport: '<Root>/CalSetGain' */
  (void)Rte_Read_CalSetGain_CalSetGain(tmpRead_7);

  /* Inport: '<Root>/CalSetOffset_V' */
  (void)Rte_Read_CalSetOffset_V_CalSetOffset_V(tmpRead_6);

  /* Inport: '<Root>/HandEnable' */
  (void)Rte_Read_HandEnable_HandEnable(selLed);

  /* Inport: '<Root>/HandSetpoint_V' */
  (void)Rte_Read_HandSetpoint_V_HandSetpoint_V(tmpRead_3);

  /* Inport: '<Root>/HandActive' */
  (void)Rte_Read_HandActive_HandActive(&tmpRead_2);

  /* Inport: '<Root>/ControllerOffReq' */
  (void)Rte_Read_ControllerOffReq_ControllerOffReq(&tmpRead_1);

  /* Inport: '<Root>/LimitActive' */
  (void)Rte_Read_LimitActive_LimitActive(&tmpRead_0);

  /* Inport: '<Root>/OutputEnable' */
  (void)Rte_Read_OutputEnable_OutputEnable(&tmpRead);

  /* RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' incorporates:
   *  SubSystem: '<Root>/CellChannels_task10ms'
   */
  /* MATLAB Function: '<S1>/CellCore' */
  for (i = 0; i < 14; i++) {
    duty[i] = 0U;
  }

  ctrlActive = (tmpRead_1 == 0);

  /* Outport: '<Root>/ControllerOn' incorporates:
   *  MATLAB Function: '<S1>/CellCore'
   */
  (void)Rte_Write_ControllerOn_ControllerOn(ctrlActive);

  /* Outport: '<Root>/ControllerOffAct' incorporates:
   *  MATLAB Function: '<S1>/CellCore'
   */
  (void)Rte_Write_ControllerOffAct_ControllerOffAct((uint8)(tmpRead_1 != 0));

  /* MATLAB Function: '<S1>/CellCore' incorporates:
   *  Constant: '<S1>/CtrlKi_per_s_Const'
   *  Constant: '<S1>/CtrlKp_Const'
   *  Constant: '<S1>/CtrlLimit_V_Const'
   *  Constant: '<S1>/OutputFloor_V_Const'
   *  Constant: '<S1>/OverVoltThd_V_Const'
   *  Constant: '<S1>/QLF_OK_Const'
   *  Constant: '<S1>/SetpointLimit_V_Const'
   *  Constant: '<S1>/StepHold_cycles_Const'
   *  Constant: '<S1>/StepThreshold_V_Const'
   *  Constant: '<S1>/VoltageSenseFilter_Const'
   *  Constant: '<S1>/VoltageSenseMeas_Ax_Const'
   *  Constant: '<S1>/VoltageSenseValue_T_Const'
   *  Constant: '<S1>/VoltageSetPwm_T_Const'
   *  Constant: '<S1>/VoltageSetValue_Ax_Const'
   *  Reshape: '<S1>/CalMeasGain_r'
   *  Reshape: '<S1>/CalMeasOffset_V_r'
   *  Reshape: '<S1>/CalSetGain_r'
   *  Reshape: '<S1>/CalSetOffset_V_r'
   *  Reshape: '<S1>/HandEnable_r'
   *  Reshape: '<S1>/HandSetpoint_V_r'
   */
  for (k = 0; k < 14; k++) {
    if (tmpRead_2) {
      commanded = selLed[k];
      v = tmpRead_3[k];
    } else {
      commanded = (rtb_TmpSignalConversionAtSFunctionInport2[k] == 2);
      v = rtb_TmpSignalConversionAtSFunctionInport1[k];
    }

    clipped = (v > Rte_Prm_CalPrm_SetpointLimit_V());
    if (clipped) {
      v = Rte_Prm_CalPrm_SetpointLimit_V();
    }

    if (v <= 0.0F) {
      v = 0.0F;
    }

    below_tmp = !clipped;
    below = (commanded && (v > 0.0F) && (v < Rte_Prm_CalPrm_OutputFloor_V()) &&
             below_tmp);
    if (fabsf(v - CellChannels_ARID_DEF.prevV[k]) >
        Rte_Prm_CalPrm_StepThreshold_V()) {
      CellChannels_ARID_DEF.holdCnt[k] = Rte_Prm_CalPrm_StepHold_cycles();
    }

    CellChannels_ARID_DEF.prevV[k] = v;
    tmpRead_1 = CellChannels_ARID_DEF.holdCnt[k];
    holding = (tmpRead_1 > 0);
    if (holding) {
      /* MW:begin MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA CPP rule violation' */
      qY = tmpRead_1 - 1U;

      /* MW:end MISRA2012:D4.1 CERT-C:INT30-C */
      if (tmpRead_1 - 1U > tmpRead_1) {
        qY = 0U;
      }

      CellChannels_ARID_DEF.holdCnt[k] = (uint8)qY;
    }

    /* Reshape: '<S1>/CalSetGain_r' incorporates:
     *  Constant: '<S1>/OutputFloor_V_Const'
     *  Constant: '<S1>/SetpointLimit_V_Const'
     *  Constant: '<S1>/StepHold_cycles_Const'
     *  Constant: '<S1>/StepThreshold_V_Const'
     *  Reshape: '<S1>/HandEnable_r'
     *  Reshape: '<S1>/HandSetpoint_V_r'
     */
    tmpRead_a = tmpRead_7[k];

    /* Reshape: '<S1>/CalSetOffset_V_r' */
    tmpRead_b = tmpRead_6[k];
    if (tmpRead_a >= 0.5F) {
      if (tmpRead_a <= 1.5F) {
        tmp = (fabsf(tmpRead_b) <= 0.5F);
      } else {
        tmp = FALSE;
      }
    } else {
      tmp = FALSE;
    }

    if (!tmp) {
      tmpRead_a = 1.0F;
      tmpRead_b = 0.0F;
    }

    /* Reshape: '<S1>/CalMeasGain_r' incorporates:
     *  Reshape: '<S1>/CalSetGain_r'
     *  Reshape: '<S1>/CalSetOffset_V_r'
     */
    tmpRead_c = tmpRead_9[k];

    /* Reshape: '<S1>/CalMeasOffset_V_r' */
    tmpRead_d = tmpRead_8[k];
    if (tmpRead_c >= 0.5F) {
      if (tmpRead_c <= 1.5F) {
        tmp = (fabsf(tmpRead_d) <= 0.5F);
      } else {
        tmp = FALSE;
      }
    } else {
      tmp = FALSE;
    }

    if (!tmp) {
      tmpRead_c = 1.0F;
      tmpRead_d = 0.0F;
    }

    if (rtb_TmpSignalConversionAtSFunctionInport4[k] == 1) {
      qY = rtb_TmpSignalConversionAtSFunctionInport3[k];
      e = (Rte_Prm_CalPrm_VoltageSenseValue_T())[5];
      if (qY <= (Rte_Prm_CalPrm_VoltageSenseMeas_Ax())[0]) {
        e = (Rte_Prm_CalPrm_VoltageSenseValue_T())[0];
      } else {
        i = 0;
        exitg1 = FALSE;
        while ((!exitg1) && (i < 5)) {
          mv = (Rte_Prm_CalPrm_VoltageSenseMeas_Ax())[i + 1];
          if (qY <= mv) {
            e = ((Rte_Prm_CalPrm_VoltageSenseValue_T())[i + 1] -
                 (Rte_Prm_CalPrm_VoltageSenseValue_T())[i]) * ((float32)qY -
              (Rte_Prm_CalPrm_VoltageSenseMeas_Ax())[i]) / (mv -
              (Rte_Prm_CalPrm_VoltageSenseMeas_Ax())[i]) +
              (Rte_Prm_CalPrm_VoltageSenseValue_T())[i];
            exitg1 = TRUE;
          } else {
            i++;
          }
        }
      }

      mv = tmpRead_c * e + tmpRead_d;
      if (tmpRead && commanded) {
        if (mv - fmaxf(v, Rte_Prm_CalPrm_OutputFloor_V()) >
            Rte_Prm_CalPrm_OverVoltThd_V()) {
          tmpRead_1 = CellChannels_ARID_DEF.ovCnt[k];
          if (tmpRead_1 < 255) {
            CellChannels_ARID_DEF.ovCnt[k] = (uint8)(tmpRead_1 + 1U);
          }
        } else {
          CellChannels_ARID_DEF.ovCnt[k] = 0U;
        }
      } else {
        CellChannels_ARID_DEF.ovCnt[k] = 0U;
      }

      if (CellChannels_ARID_DEF.seen[k]) {
        e = CellChannels_ARID_DEF.y[k];
        CellChannels_ARID_DEF.y[k] = (mv - e) *
          Rte_Prm_CalPrm_VoltageSenseFilter() + e;
      } else {
        CellChannels_ARID_DEF.y[k] = mv;
        CellChannels_ARID_DEF.seen[k] = TRUE;
      }
    }

    e = CellChannels_ARID_DEF.y[k];
    CellChannels_ARID_DEF.ActualVolt_V[k] = e;
    tmp = !commanded;
    if ((!tmpRead) || tmp) {
      CellChannels_ARID_DEF.ovCnt[k] = 0U;
    }

    mv = 0.0F;
    if (ctrlActive && tmpRead && commanded && below_tmp && (v > 0.0F) && (!below))
    {
      if (rtb_TmpSignalConversionAtSFunctionInport4[k] == 1) {
        below_tmp = CellChannels_ARID_DEF.seen[k];
      } else {
        below_tmp = FALSE;
      }
    } else {
      below_tmp = FALSE;
    }

    if (holding) {
      CellChannels_ARID_DEF.integ[k] = 0.0F;
      CellChannels_ARID_DEF.corrHold[k] = 0.0F;
    } else if (below_tmp && tmpRead_0) {
      mv = CellChannels_ARID_DEF.corrHold[k];
    } else if (below_tmp) {
      e = v - e;
      mv = fminf(fmaxf(Rte_Prm_CalPrm_CtrlKi_per_s() * 0.01F * e +
                       CellChannels_ARID_DEF.integ[k],
                       -Rte_Prm_CalPrm_CtrlLimit_V()),
                 Rte_Prm_CalPrm_CtrlLimit_V());
      CellChannels_ARID_DEF.integ[k] = mv;
      mv = fminf(fmaxf(Rte_Prm_CalPrm_CtrlKp() * e + mv,
                       -Rte_Prm_CalPrm_CtrlLimit_V()),
                 Rte_Prm_CalPrm_CtrlLimit_V());
      CellChannels_ARID_DEF.corrHold[k] = mv;
    } else {
      CellChannels_ARID_DEF.integ[k] = 0.0F;
      CellChannels_ARID_DEF.corrHold[k] = 0.0F;
    }

    if (tmpRead && (v > 0.0F)) {
      v = fminf(fmaxf(v + mv, 0.0F), Rte_Prm_CalPrm_SetpointLimit_V()) *
        tmpRead_a + tmpRead_b;
      tmpRead_a = (Rte_Prm_CalPrm_VoltageSetPwm_T())[5];
      if (v <= (Rte_Prm_CalPrm_VoltageSetValue_Ax())[0]) {
        tmpRead_a = (Rte_Prm_CalPrm_VoltageSetPwm_T())[0];
      } else {
        i = 0;
        exitg1 = FALSE;
        while ((!exitg1) && (i < 5)) {
          mv = (Rte_Prm_CalPrm_VoltageSetValue_Ax())[i + 1];
          if (v <= mv) {
            tmpRead_a = ((Rte_Prm_CalPrm_VoltageSetPwm_T())[i + 1] -
                         (Rte_Prm_CalPrm_VoltageSetPwm_T())[i]) * (v -
              (Rte_Prm_CalPrm_VoltageSetValue_Ax())[i]) / (mv -
              (Rte_Prm_CalPrm_VoltageSetValue_Ax())[i]) +
              (Rte_Prm_CalPrm_VoltageSetPwm_T())[i];
            exitg1 = TRUE;
          } else {
            i++;
          }
        }
      }

      duty[k] = (uint16)floorf(fminf(fmaxf(tmpRead_a * 10000.0F, 0.0F), 10000.0F)
        + 0.5F);
    }

    en[k] = (tmpRead && commanded);
    if (tmp) {
      state[k] = 1U;
    } else if (clipped) {
      state[k] = 3U;
    } else if (below) {
      state[k] = 4U;
    } else {
      state[k] = 2U;
    }

    selLed[k] = FALSE;
  }

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' */

  /* Inport: '<Root>/SelectedChannel' */
  (void)Rte_Read_SelectedChannel_SelectedChannel(&tmpRead_4);

  /* Inport: '<Root>/SelectAllMode' */
  (void)Rte_Read_SelectAllMode_SelectAllMode(&tmpRead_5);

  /* RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' incorporates:
   *  SubSystem: '<Root>/CellChannels_task10ms'
   */
  /* MATLAB Function: '<S1>/CellCore' */
  if (tmpRead_2) {
    for (k = 0; k < 14; k++) {
      selLed[k] = (tmpRead_5 || (k + 1 == tmpRead_4));
    }
  }

  /* Outport: '<Root>/ChannelsReleased' incorporates:
   *  MATLAB Function: '<S1>/CellCore'
   */
  (void)Rte_Write_ChannelsReleased_ChannelsReleased(FALSE);

  /* MATLAB Function: '<S1>/CellCore' */
  k = 0;
  exitg1 = FALSE;
  while ((!exitg1) && (k < 14)) {
    if (en[k]) {
      (void)Rte_Write_ChannelsReleased_ChannelsReleased(TRUE);
      exitg1 = TRUE;
    } else {
      k++;
    }
  }

  /* Outport: '<Root>/OverVoltage' incorporates:
   *  MATLAB Function: '<S1>/CellCore'
   */
  (void)Rte_Write_OverVoltage_OverVoltage(FALSE);

  /* MATLAB Function: '<S1>/CellCore' */
  k = 0;
  exitg1 = FALSE;
  while ((!exitg1) && (k < 14)) {
    if (CellChannels_ARID_DEF.ovCnt[k] >= Rte_Prm_CalPrm_OverVoltTime_cycles())
    {
      (void)Rte_Write_OverVoltage_OverVoltage(TRUE);
      exitg1 = TRUE;
    } else {
      k++;
    }
  }

  /* Outport: '<Root>/Cell02State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell02State'
   */
  (void)Rte_Write_Cell02State_Cell02State(state[1]);

  /* Outport: '<Root>/Cell02VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell02VoltAct_V'
   */
  (void)Rte_Write_Cell02VoltAct_V_Cell02VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[1]);

  /* Outport: '<Root>/Cell03State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell03State'
   */
  (void)Rte_Write_Cell03State_Cell03State(state[2]);

  /* Outport: '<Root>/Cell03VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell03VoltAct_V'
   */
  (void)Rte_Write_Cell03VoltAct_V_Cell03VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[2]);

  /* Outport: '<Root>/Cell04State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell04State'
   */
  (void)Rte_Write_Cell04State_Cell04State(state[3]);

  /* Outport: '<Root>/Cell04VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell04VoltAct_V'
   */
  (void)Rte_Write_Cell04VoltAct_V_Cell04VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[3]);

  /* Outport: '<Root>/Cell05State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell05State'
   */
  (void)Rte_Write_Cell05State_Cell05State(state[4]);

  /* Outport: '<Root>/Cell05VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell05VoltAct_V'
   */
  (void)Rte_Write_Cell05VoltAct_V_Cell05VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[4]);

  /* Outport: '<Root>/Cell06State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell06State'
   */
  (void)Rte_Write_Cell06State_Cell06State(state[5]);

  /* Outport: '<Root>/Cell06VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell06VoltAct_V'
   */
  (void)Rte_Write_Cell06VoltAct_V_Cell06VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[5]);

  /* Outport: '<Root>/Cell07State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell07State'
   */
  (void)Rte_Write_Cell07State_Cell07State(state[6]);

  /* Outport: '<Root>/Cell07VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell07VoltAct_V'
   */
  (void)Rte_Write_Cell07VoltAct_V_Cell07VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[6]);

  /* Outport: '<Root>/Cell08State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell08State'
   */
  (void)Rte_Write_Cell08State_Cell08State(state[7]);

  /* Outport: '<Root>/Cell08VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell08VoltAct_V'
   */
  (void)Rte_Write_Cell08VoltAct_V_Cell08VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[7]);

  /* Outport: '<Root>/Cell09State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell09State'
   */
  (void)Rte_Write_Cell09State_Cell09State(state[8]);

  /* Outport: '<Root>/Cell09VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell09VoltAct_V'
   */
  (void)Rte_Write_Cell09VoltAct_V_Cell09VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[8]);

  /* Outport: '<Root>/Cell10State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell10State'
   */
  (void)Rte_Write_Cell10State_Cell10State(state[9]);

  /* Outport: '<Root>/Cell10VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell10VoltAct_V'
   */
  (void)Rte_Write_Cell10VoltAct_V_Cell10VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[9]);

  /* Outport: '<Root>/Cell11State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell11State'
   */
  (void)Rte_Write_Cell11State_Cell11State(state[10]);

  /* Outport: '<Root>/Cell11VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell11VoltAct_V'
   */
  (void)Rte_Write_Cell11VoltAct_V_Cell11VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[10]);

  /* Outport: '<Root>/Cell12State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell12State'
   */
  (void)Rte_Write_Cell12State_Cell12State(state[11]);

  /* Outport: '<Root>/Cell12VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell12VoltAct_V'
   */
  (void)Rte_Write_Cell12VoltAct_V_Cell12VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[11]);

  /* Outport: '<Root>/Cell13State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell13State'
   */
  (void)Rte_Write_Cell13State_Cell13State(state[12]);

  /* Outport: '<Root>/Cell13VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell13VoltAct_V'
   */
  (void)Rte_Write_Cell13VoltAct_V_Cell13VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[12]);

  /* Outport: '<Root>/Cell14State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell14State'
   */
  (void)Rte_Write_Cell14State_Cell14State(state[13]);

  /* Outport: '<Root>/Cell14VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell14VoltAct_V'
   */
  (void)Rte_Write_Cell14VoltAct_V_Cell14VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[13]);

  /* Outport: '<Root>/Duty02' incorporates:
   *  SignalConversion generated from: '<S1>/Duty02'
   */
  (void)Rte_Write_Duty02_Duty02(duty[1]);

  /* Outport: '<Root>/Duty03' incorporates:
   *  SignalConversion generated from: '<S1>/Duty03'
   */
  (void)Rte_Write_Duty03_Duty03(duty[2]);

  /* Outport: '<Root>/Duty04' incorporates:
   *  SignalConversion generated from: '<S1>/Duty04'
   */
  (void)Rte_Write_Duty04_Duty04(duty[3]);

  /* Outport: '<Root>/Duty05' incorporates:
   *  SignalConversion generated from: '<S1>/Duty05'
   */
  (void)Rte_Write_Duty05_Duty05(duty[4]);

  /* Outport: '<Root>/Duty06' incorporates:
   *  SignalConversion generated from: '<S1>/Duty06'
   */
  (void)Rte_Write_Duty06_Duty06(duty[5]);

  /* Outport: '<Root>/Duty07' incorporates:
   *  SignalConversion generated from: '<S1>/Duty07'
   */
  (void)Rte_Write_Duty07_Duty07(duty[6]);

  /* Outport: '<Root>/Duty08' incorporates:
   *  SignalConversion generated from: '<S1>/Duty08'
   */
  (void)Rte_Write_Duty08_Duty08(duty[7]);

  /* Outport: '<Root>/Duty09' incorporates:
   *  SignalConversion generated from: '<S1>/Duty09'
   */
  (void)Rte_Write_Duty09_Duty09(duty[8]);

  /* Outport: '<Root>/Duty10' incorporates:
   *  SignalConversion generated from: '<S1>/Duty10'
   */
  (void)Rte_Write_Duty10_Duty10(duty[9]);

  /* Outport: '<Root>/Duty11' incorporates:
   *  SignalConversion generated from: '<S1>/Duty11'
   */
  (void)Rte_Write_Duty11_Duty11(duty[10]);

  /* Outport: '<Root>/Duty12' incorporates:
   *  SignalConversion generated from: '<S1>/Duty12'
   */
  (void)Rte_Write_Duty12_Duty12(duty[11]);

  /* Outport: '<Root>/Duty13' incorporates:
   *  SignalConversion generated from: '<S1>/Duty13'
   */
  (void)Rte_Write_Duty13_Duty13(duty[12]);

  /* Outport: '<Root>/Duty14' incorporates:
   *  SignalConversion generated from: '<S1>/Duty14'
   */
  (void)Rte_Write_Duty14_Duty14(duty[13]);

  /* Outport: '<Root>/En02' incorporates:
   *  SignalConversion generated from: '<S1>/En02'
   */
  (void)Rte_Write_En02_En02(en[1]);

  /* Outport: '<Root>/En03' incorporates:
   *  SignalConversion generated from: '<S1>/En03'
   */
  (void)Rte_Write_En03_En03(en[2]);

  /* Outport: '<Root>/En04' incorporates:
   *  SignalConversion generated from: '<S1>/En04'
   */
  (void)Rte_Write_En04_En04(en[3]);

  /* Outport: '<Root>/En05' incorporates:
   *  SignalConversion generated from: '<S1>/En05'
   */
  (void)Rte_Write_En05_En05(en[4]);

  /* Outport: '<Root>/En06' incorporates:
   *  SignalConversion generated from: '<S1>/En06'
   */
  (void)Rte_Write_En06_En06(en[5]);

  /* Outport: '<Root>/En07' incorporates:
   *  SignalConversion generated from: '<S1>/En07'
   */
  (void)Rte_Write_En07_En07(en[6]);

  /* Outport: '<Root>/En08' incorporates:
   *  SignalConversion generated from: '<S1>/En08'
   */
  (void)Rte_Write_En08_En08(en[7]);

  /* Outport: '<Root>/En09' incorporates:
   *  SignalConversion generated from: '<S1>/En09'
   */
  (void)Rte_Write_En09_En09(en[8]);

  /* Outport: '<Root>/En10' incorporates:
   *  SignalConversion generated from: '<S1>/En10'
   */
  (void)Rte_Write_En10_En10(en[9]);

  /* Outport: '<Root>/En11' incorporates:
   *  SignalConversion generated from: '<S1>/En11'
   */
  (void)Rte_Write_En11_En11(en[10]);

  /* Outport: '<Root>/En12' incorporates:
   *  SignalConversion generated from: '<S1>/En12'
   */
  (void)Rte_Write_En12_En12(en[11]);

  /* Outport: '<Root>/En13' incorporates:
   *  SignalConversion generated from: '<S1>/En13'
   */
  (void)Rte_Write_En13_En13(en[12]);

  /* Outport: '<Root>/En14' incorporates:
   *  SignalConversion generated from: '<S1>/En14'
   */
  (void)Rte_Write_En14_En14(en[13]);

  /* Outport: '<Root>/Sel02' incorporates:
   *  SignalConversion generated from: '<S1>/Sel02'
   */
  (void)Rte_Write_Sel02_Sel02(selLed[1]);

  /* Outport: '<Root>/Sel03' incorporates:
   *  SignalConversion generated from: '<S1>/Sel03'
   */
  (void)Rte_Write_Sel03_Sel03(selLed[2]);

  /* Outport: '<Root>/Sel04' incorporates:
   *  SignalConversion generated from: '<S1>/Sel04'
   */
  (void)Rte_Write_Sel04_Sel04(selLed[3]);

  /* Outport: '<Root>/Sel05' incorporates:
   *  SignalConversion generated from: '<S1>/Sel05'
   */
  (void)Rte_Write_Sel05_Sel05(selLed[4]);

  /* Outport: '<Root>/Sel06' incorporates:
   *  SignalConversion generated from: '<S1>/Sel06'
   */
  (void)Rte_Write_Sel06_Sel06(selLed[5]);

  /* Outport: '<Root>/Sel07' incorporates:
   *  SignalConversion generated from: '<S1>/Sel07'
   */
  (void)Rte_Write_Sel07_Sel07(selLed[6]);

  /* Outport: '<Root>/Sel08' incorporates:
   *  SignalConversion generated from: '<S1>/Sel08'
   */
  (void)Rte_Write_Sel08_Sel08(selLed[7]);

  /* Outport: '<Root>/Sel09' incorporates:
   *  SignalConversion generated from: '<S1>/Sel09'
   */
  (void)Rte_Write_Sel09_Sel09(selLed[8]);

  /* Outport: '<Root>/Sel10' incorporates:
   *  SignalConversion generated from: '<S1>/Sel10'
   */
  (void)Rte_Write_Sel10_Sel10(selLed[9]);

  /* Outport: '<Root>/Sel11' incorporates:
   *  SignalConversion generated from: '<S1>/Sel11'
   */
  (void)Rte_Write_Sel11_Sel11(selLed[10]);

  /* Outport: '<Root>/Sel12' incorporates:
   *  SignalConversion generated from: '<S1>/Sel12'
   */
  (void)Rte_Write_Sel12_Sel12(selLed[11]);

  /* Outport: '<Root>/Sel13' incorporates:
   *  SignalConversion generated from: '<S1>/Sel13'
   */
  (void)Rte_Write_Sel13_Sel13(selLed[12]);

  /* Outport: '<Root>/Sel14' incorporates:
   *  SignalConversion generated from: '<S1>/Sel14'
   */
  (void)Rte_Write_Sel14_Sel14(selLed[13]);

  /* Outport: '<Root>/Cell15State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell15State'
   */
  (void)Rte_Write_Cell15State_Cell15State(CellChannels_ConstB.Cell15State_Const);

  /* Outport: '<Root>/Cell16State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell16State'
   */
  (void)Rte_Write_Cell16State_Cell16State(CellChannels_ConstB.Cell16State_Const);

  /* Outport: '<Root>/Cell15VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell15VoltAct_V'
   */
  (void)Rte_Write_Cell15VoltAct_V_Cell15VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell16VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell16VoltAct_V'
   */
  (void)Rte_Write_Cell16VoltAct_V_Cell16VoltAct_V(0.0F);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' */

  /* Outport: '<Root>/En01' */
  (void)Rte_Write_En01_En01(en[0]);

  /* Outport: '<Root>/Sel01' */
  (void)Rte_Write_Sel01_Sel01(selLed[0]);

  /* Outport: '<Root>/Cell01VoltAct_V' */
  (void)Rte_Write_Cell01VoltAct_V_Cell01VoltAct_V
    (CellChannels_ARID_DEF.ActualVolt_V[0]);

  /* Outport: '<Root>/Cell01State' */
  (void)Rte_Write_Cell01State_Cell01State(state[0]);

  /* Outport: '<Root>/Duty01' */
  (void)Rte_Write_Duty01_Duty01(duty[0]);
  (void)Rte_Write_ActualVolt_V_ActualVolt_V(CellChannels_ARID_DEF.ActualVolt_V);
}

/* Model initialize function */
void CellChannels_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' incorporates:
   *  SubSystem: '<Root>/CellChannels_task10ms'
   */
  /* SystemInitialize for Outport: '<Root>/Cell15State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell15State'
   */
  (void)Rte_Write_Cell15State_Cell15State(CellChannels_ConstB.Cell15State_Const);

  /* SystemInitialize for Outport: '<Root>/Cell16State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell16State'
   */
  (void)Rte_Write_Cell16State_Cell16State(CellChannels_ConstB.Cell16State_Const);

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
