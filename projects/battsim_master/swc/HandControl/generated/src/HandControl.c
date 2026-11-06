/*
 * File: HandControl.c
 *
 * Code generated for Simulink model 'HandControl'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "HandControl.h"
#include <math.h>
#include "Platform_Types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_HandControl_T HandControl_ARID_DEF;/* '<Root>/HandSetpoint_V' */

/* Model step function for TID1 */
void HandTask10ms(void)                /* Explicit Task: HandTask10ms */
{
  sint64 tmp;
  float32 tmpRead_4[Rte_SysCon_CellCount];
  float32 x;
  float32 y;
  sint32 i;
  sint32 newSel;
  sint32 stepsS;
  sint32 stepsV;
  uint16 tmpRead_0;
  uint16 tmpRead_2;
  uint8 debounce;
  uint8 maxval;
  boolean evK[14];
  boolean rtb_TmpSignalConversionAtSFunctionInport6[14];
  boolean evV;
  boolean guard1;
  boolean tmpRead;
  boolean tmpRead_1;
  boolean tmpRead_3;

  /* Inport: '<Root>/ActualVolt_V' */
  (void)Rte_Read_ActualVolt_V_ActualVolt_V(tmpRead_4);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/CardKey01'
   *  Inport: '<Root>/CardKey02'
   *  Inport: '<Root>/CardKey03'
   *  Inport: '<Root>/CardKey04'
   *  Inport: '<Root>/CardKey05'
   *  Inport: '<Root>/CardKey06'
   *  Inport: '<Root>/CardKey07'
   *  Inport: '<Root>/CardKey08'
   *  Inport: '<Root>/CardKey09'
   *  Inport: '<Root>/CardKey10'
   *  Inport: '<Root>/CardKey11'
   *  Inport: '<Root>/CardKey12'
   *  Inport: '<Root>/CardKey13'
   *  Inport: '<Root>/CardKey14'
   */
  (void)Rte_Read_CardKey14_CardKey14(&rtb_TmpSignalConversionAtSFunctionInport6
    [13]);
  (void)Rte_Read_CardKey13_CardKey13(&rtb_TmpSignalConversionAtSFunctionInport6
    [12]);
  (void)Rte_Read_CardKey12_CardKey12(&rtb_TmpSignalConversionAtSFunctionInport6
    [11]);
  (void)Rte_Read_CardKey11_CardKey11(&rtb_TmpSignalConversionAtSFunctionInport6
    [10]);
  (void)Rte_Read_CardKey10_CardKey10(&rtb_TmpSignalConversionAtSFunctionInport6
    [9]);
  (void)Rte_Read_CardKey09_CardKey09(&rtb_TmpSignalConversionAtSFunctionInport6
    [8]);
  (void)Rte_Read_CardKey08_CardKey08(&rtb_TmpSignalConversionAtSFunctionInport6
    [7]);
  (void)Rte_Read_CardKey07_CardKey07(&rtb_TmpSignalConversionAtSFunctionInport6
    [6]);
  (void)Rte_Read_CardKey06_CardKey06(&rtb_TmpSignalConversionAtSFunctionInport6
    [5]);
  (void)Rte_Read_CardKey05_CardKey05(&rtb_TmpSignalConversionAtSFunctionInport6
    [4]);
  (void)Rte_Read_CardKey04_CardKey04(&rtb_TmpSignalConversionAtSFunctionInport6
    [3]);
  (void)Rte_Read_CardKey03_CardKey03(&rtb_TmpSignalConversionAtSFunctionInport6
    [2]);
  (void)Rte_Read_CardKey02_CardKey02(&rtb_TmpSignalConversionAtSFunctionInport6
    [1]);
  (void)Rte_Read_CardKey01_CardKey01(&rtb_TmpSignalConversionAtSFunctionInport6
    [0]);

  /* Inport: '<Root>/EncSelCount' */
  (void)Rte_Read_EncSelCount_EncSelCount(&tmpRead_2);

  /* Inport: '<Root>/EncVoltCount' */
  (void)Rte_Read_EncVoltCount_EncVoltCount(&tmpRead_0);

  /* Inport: '<Root>/HandActive' */
  (void)Rte_Read_HandActive_HandActive(&tmpRead);

  /* RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' incorporates:
   *  SubSystem: '<Root>/HandControl_task10ms'
   */
  /* MATLAB Function: '<S1>/HandCore' incorporates:
   *  Constant: '<S1>/CountsPerStep_Const'
   *  Constant: '<S1>/KeyDebounce_cycles_Const'
   */
  if (!HandControl_ARID_DEF.started_not_empty) {
    HandControl_ARID_DEF.started_not_empty = TRUE;
    HandControl_ARID_DEF.lastV = tmpRead_0;
    HandControl_ARID_DEF.lastS = tmpRead_2;
  }

  if (Rte_Prm_CalPrm_KeyDebounce_cycles() >= 1) {
    debounce = Rte_Prm_CalPrm_KeyDebounce_cycles();
  } else {
    debounce = 1U;
  }

  stepsV = tmpRead_0 - HandControl_ARID_DEF.lastV;
  if (stepsV > 32767) {
    stepsV -= 65536;
  } else if (stepsV < -32768) {
    stepsV += 65536;
  }

  if ((HandControl_ARID_DEF.accV < 0) && (stepsV < ((sint32)(-2147483647-1)) -
       HandControl_ARID_DEF.accV)) {
    HandControl_ARID_DEF.accV = ((sint32)(-2147483647-1));
  } else if ((HandControl_ARID_DEF.accV > 0) && (stepsV > ((sint32)(2147483647))
              - HandControl_ARID_DEF.accV)) {
    HandControl_ARID_DEF.accV = ((sint32)(2147483647));
  } else {
    HandControl_ARID_DEF.accV += stepsV;
  }

  stepsV = tmpRead_2 - HandControl_ARID_DEF.lastS;
  if (stepsV > 32767) {
    stepsV -= 65536;
  } else if (stepsV < -32768) {
    stepsV += 65536;
  }

  if ((HandControl_ARID_DEF.accS < 0) && (stepsV < ((sint32)(-2147483647-1)) -
       HandControl_ARID_DEF.accS)) {
    HandControl_ARID_DEF.accS = ((sint32)(-2147483647-1));
  } else if ((HandControl_ARID_DEF.accS > 0) && (stepsV > ((sint32)(2147483647))
              - HandControl_ARID_DEF.accS)) {
    HandControl_ARID_DEF.accS = ((sint32)(2147483647));
  } else {
    HandControl_ARID_DEF.accS += stepsV;
  }

  HandControl_ARID_DEF.lastV = tmpRead_0;
  HandControl_ARID_DEF.lastS = tmpRead_2;
  if (Rte_Prm_CalPrm_CountsPerStep() >= 1) {
    maxval = Rte_Prm_CalPrm_CountsPerStep();
  } else {
    maxval = 1U;
  }

  stepsV = 0;
  stepsS = 0;
  if (HandControl_ARID_DEF.accV < 0) {
    if (HandControl_ARID_DEF.accV <= ((sint32)(-2147483647-1))) {
      newSel = ((sint32)(2147483647));
    } else {
      newSel = -HandControl_ARID_DEF.accV;
    }
  } else {
    newSel = HandControl_ARID_DEF.accV;
  }

  if (newSel >= maxval) {
    stepsV = HandControl_ARID_DEF.accV / maxval;
    tmp = (sint64)stepsV * maxval;
    if (tmp > 2147483647LL) {
      tmp = 2147483647LL;
    } else if (tmp < -2147483648LL) {
      tmp = -2147483648LL;
    }

    if ((HandControl_ARID_DEF.accV >= 0) && ((sint32)tmp <
         HandControl_ARID_DEF.accV - ((sint32)(2147483647)))) {
      HandControl_ARID_DEF.accV = ((sint32)(2147483647));
    } else if ((HandControl_ARID_DEF.accV < 0) && ((sint32)tmp >
                HandControl_ARID_DEF.accV - ((sint32)(-2147483647-1)))) {
      HandControl_ARID_DEF.accV = ((sint32)(-2147483647-1));
    } else {
      HandControl_ARID_DEF.accV -= (sint32)tmp;
    }
  }

  if (HandControl_ARID_DEF.accS < 0) {
    if (HandControl_ARID_DEF.accS <= ((sint32)(-2147483647-1))) {
      newSel = ((sint32)(2147483647));
    } else {
      newSel = -HandControl_ARID_DEF.accS;
    }
  } else {
    newSel = HandControl_ARID_DEF.accS;
  }

  if (newSel >= maxval) {
    stepsS = HandControl_ARID_DEF.accS / maxval;
    tmp = (sint64)stepsS * maxval;
    if (tmp > 2147483647LL) {
      tmp = 2147483647LL;
    } else if (tmp < -2147483648LL) {
      tmp = -2147483648LL;
    }

    if ((HandControl_ARID_DEF.accS >= 0) && ((sint32)tmp <
         HandControl_ARID_DEF.accS - ((sint32)(2147483647)))) {
      HandControl_ARID_DEF.accS = ((sint32)(2147483647));
    } else if ((HandControl_ARID_DEF.accS < 0) && ((sint32)tmp >
                HandControl_ARID_DEF.accS - ((sint32)(-2147483647-1)))) {
      HandControl_ARID_DEF.accS = ((sint32)(-2147483647-1));
    } else {
      HandControl_ARID_DEF.accS -= (sint32)tmp;
    }
  }

  maxval = HandControl_ARID_DEF.cntV;
  evV = FALSE;

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' */

  /* Inport: '<Root>/EncVoltButton' */
  (void)Rte_Read_EncVoltButton_EncVoltButton(&tmpRead_1);

  /* RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' incorporates:
   *  SubSystem: '<Root>/HandControl_task10ms'
   */
  /* MATLAB Function: '<S1>/HandCore' */
  if (tmpRead_1) {
    if (HandControl_ARID_DEF.cntV < 255) {
      maxval = (uint8)(HandControl_ARID_DEF.cntV + 1U);
      evV = ((sint32)(HandControl_ARID_DEF.cntV + 1U) == debounce);
    }
  } else {
    maxval = 0U;
  }

  HandControl_ARID_DEF.cntV = maxval;
  maxval = HandControl_ARID_DEF.cntS;
  tmpRead_1 = FALSE;

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' */

  /* Inport: '<Root>/EncSelButton' */
  (void)Rte_Read_EncSelButton_EncSelButton(&tmpRead_3);

  /* RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' incorporates:
   *  SubSystem: '<Root>/HandControl_task10ms'
   */
  /* MATLAB Function: '<S1>/HandCore' incorporates:
   *  Constant: '<S1>/CoarseStep_mV_Const'
   *  Constant: '<S1>/FineStep_mV_Const'
   *  Constant: '<S1>/HandMinSetpoint_V_Const'
   *  Constant: '<S1>/SetpointLimit_V_Const'
   */
  if (tmpRead_3) {
    if (HandControl_ARID_DEF.cntS < 255) {
      maxval = (uint8)(HandControl_ARID_DEF.cntS + 1U);
      tmpRead_1 = ((sint32)(HandControl_ARID_DEF.cntS + 1U) == debounce);
    }
  } else {
    maxval = 0U;
  }

  HandControl_ARID_DEF.cntS = maxval;
  for (newSel = 0; newSel < 14; newSel++) {
    maxval = HandControl_ARID_DEF.cntK[newSel];
    evK[newSel] = FALSE;
    if (rtb_TmpSignalConversionAtSFunctionInport6[newSel]) {
      if (maxval < 255) {
        HandControl_ARID_DEF.cntK[newSel] = (uint8)(maxval + 1U);
        if ((sint32)(maxval + 1U) == debounce) {
          evK[newSel] = TRUE;
        }
      }
    } else {
      HandControl_ARID_DEF.cntK[newSel] = 0U;
    }
  }

  if (evV) {
    HandControl_ARID_DEF.coarse = !HandControl_ARID_DEF.coarse;
  }

  if (tmpRead_1) {
    HandControl_ARID_DEF.selAll = !HandControl_ARID_DEF.selAll;
  }

  if (stepsS > ((sint32)(2147483647)) - HandControl_ARID_DEF.sel) {
    newSel = ((sint32)(2147483647));
  } else {
    newSel = HandControl_ARID_DEF.sel + stepsS;
  }

  if (newSel < 1) {
    newSel = 1;
  } else if (newSel > 14) {
    newSel = 14;
  }

  HandControl_ARID_DEF.sel = (uint8)newSel;
  x = roundf(Rte_Prm_CalPrm_SetpointLimit_V() * 1000.0F);
  if (x < 2.14748365E+9F) {
    if (x >= -2.14748365E+9F) {
      newSel = (sint32)x;
    } else {
      newSel = ((sint32)(-2147483647-1));
    }
  } else {
    newSel = ((sint32)(2147483647));
  }

  if (tmpRead) {
    if (!HandControl_ARID_DEF.wasActive) {
      for (i = 0; i < 14; i++) {
        HandControl_ARID_DEF.spMv[i] = 0;
        HandControl_ARID_DEF.en0[i] = TRUE;
      }
    }

    if (stepsV != 0) {
      stepsS = HandControl_ARID_DEF.spMv[HandControl_ARID_DEF.sel - 1];
      if (HandControl_ARID_DEF.coarse) {
        x = Rte_Prm_CalPrm_CoarseStep_mV();
      } else {
        x = Rte_Prm_CalPrm_FineStep_mV();
      }

      x = roundf((float32)stepsV * x);
      if (x < 2.14748365E+9F) {
        if (x >= -2.14748365E+9F) {
          i = (sint32)x;
        } else {
          i = ((sint32)(-2147483647-1));
        }
      } else {
        i = ((sint32)(2147483647));
      }

      if ((stepsS < 0) && (i < ((sint32)(-2147483647-1)) - stepsS)) {
        stepsS = ((sint32)(-2147483647-1));
      } else if ((stepsS > 0) && (i > ((sint32)(2147483647)) - stepsS)) {
        stepsS = ((sint32)(2147483647));
      } else {
        stepsS += i;
      }

      x = roundf(Rte_Prm_CalPrm_HandMinSetpoint_V() * 1000.0F);
      if (x < 2.14748365E+9F) {
        if (x >= -2.14748365E+9F) {
          i = (sint32)x;
        } else {
          i = ((sint32)(-2147483647-1));
        }
      } else {
        i = ((sint32)(2147483647));
      }

      if ((stepsS > 0) && (stepsS < i)) {
        if (stepsV > 0) {
          stepsS = i;
        } else {
          stepsS = 0;
        }
      }

      if (stepsS < 0) {
        stepsS = 0;
      } else if (stepsS > newSel) {
        stepsS = newSel;
      }

      if (HandControl_ARID_DEF.selAll) {
        for (i = 0; i < 14; i++) {
          HandControl_ARID_DEF.spMv[i] = stepsS;
        }
      } else {
        HandControl_ARID_DEF.spMv[HandControl_ARID_DEF.sel - 1] = stepsS;
      }
    }

    for (stepsV = 0; stepsV < 14; stepsV++) {
      if (evK[stepsV]) {
        HandControl_ARID_DEF.en0[stepsV] = !HandControl_ARID_DEF.en0[stepsV];
      }
    }
  } else {
    for (i = 0; i < 14; i++) {
      HandControl_ARID_DEF.spMv[i] = 0;
      HandControl_ARID_DEF.en0[i] = FALSE;
    }
  }

  HandControl_ARID_DEF.wasActive = tmpRead;

  /* Outport: '<Root>/SelectedChannel' incorporates:
   *  MATLAB Function: '<S1>/HandCore'
   */
  (void)Rte_Write_SelectedChannel_SelectedChannel(HandControl_ARID_DEF.sel);

  /* Outport: '<Root>/SelectAllMode' incorporates:
   *  MATLAB Function: '<S1>/HandCore'
   */
  (void)Rte_Write_SelectAllMode_SelectAllMode(HandControl_ARID_DEF.selAll);

  /* MATLAB Function: '<S1>/HandCore' incorporates:
   *  Constant: '<S1>/DisplayFilter_cycles_Const'
   *  Constant: '<S1>/DisplayHyst_mV_Const'
   *  Reshape: '<S1>/ActualVolt_V_r'
   */
  x = tmpRead_4[HandControl_ARID_DEF.sel - 1] * 1000.0F;
  y = 0.0F;
  if (x > 9999.0F) {
    y = 9999.0F;
  } else if (x >= 0.0F) {
    y = x;
  }

  guard1 = FALSE;
  if ((!HandControl_ARID_DEF.dispInit) || (HandControl_ARID_DEF.sel !=
       HandControl_ARID_DEF.lastSel)) {
    guard1 = TRUE;
  } else {
    x = y - HandControl_ARID_DEF.filt;
    if (fabsf(x) >= Rte_Prm_CalPrm_DisplayJump_mV()) {
      guard1 = TRUE;
    } else {
      if (Rte_Prm_CalPrm_DisplayFilter_cycles() >= 1) {
        tmpRead_2 = Rte_Prm_CalPrm_DisplayFilter_cycles();
      } else {
        tmpRead_2 = 1U;
      }

      HandControl_ARID_DEF.filt += x / (float32)tmpRead_2;
      if (fabsf(HandControl_ARID_DEF.filt - HandControl_ARID_DEF.shown) >=
          Rte_Prm_CalPrm_DisplayHyst_mV()) {
        HandControl_ARID_DEF.shown = HandControl_ARID_DEF.filt;
        HandControl_ARID_DEF.shown = roundf(HandControl_ARID_DEF.shown);
      }
    }
  }

  if (guard1) {
    HandControl_ARID_DEF.filt = y;
    HandControl_ARID_DEF.shown = y;
    HandControl_ARID_DEF.shown = roundf(HandControl_ARID_DEF.shown);
  }

  HandControl_ARID_DEF.lastSel = HandControl_ARID_DEF.sel;
  HandControl_ARID_DEF.dispInit = TRUE;
  x = roundf(HandControl_ARID_DEF.shown);
  if (x < 65536.0F) {
    if (x >= 0.0F) {
      tmpRead_2 = (uint16)x;
    } else {
      tmpRead_2 = 0U;
    }
  } else {
    tmpRead_2 = ((uint16)(65535U));
  }

  /* Outport: '<Root>/DisplayValue_mV' incorporates:
   *  MATLAB Function: '<S1>/HandCore'
   */
  (void)Rte_Write_DisplayValue_mV_DisplayValue_mV(tmpRead_2);
  for (newSel = 0; newSel < Rte_SysCon_CellCount; newSel++) {
    /* Outport: '<Root>/HandEnable' incorporates:
     *  MATLAB Function: '<S1>/HandCore'
     *  Reshape: '<S1>/HandEnable_r'
     */
    HandControl_ARID_DEF.HandEnable[newSel] = HandControl_ARID_DEF.en0[newSel];

    /* Outport: '<Root>/HandSetpoint_V' incorporates:
     *  MATLAB Function: '<S1>/HandCore'
     *  Reshape: '<S1>/HandSetpoint_V_r'
     */
    HandControl_ARID_DEF.HandSetpoint_V[newSel] = (float32)
      HandControl_ARID_DEF.spMv[newSel] / 1000.0F;
  }

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' */
  (void)Rte_Write_HandEnable_HandEnable(HandControl_ARID_DEF.HandEnable);
  (void)Rte_Write_HandSetpoint_V_HandSetpoint_V
    (HandControl_ARID_DEF.HandSetpoint_V);
}

/* Model initialize function */
void HandControl_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' incorporates:
   *  SubSystem: '<Root>/HandControl_task10ms'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/HandCore' */
  HandControl_ARID_DEF.sel = 1U;
  HandControl_ARID_DEF.coarse = TRUE;
  HandControl_ARID_DEF.lastSel = 1U;

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/HandTask10ms' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
