/*
 * File: CellCal.c
 *
 * Code generated for Simulink model 'CellCal'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CellCal.h"
#include "Platform_Types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_CellCal_T CellCal_ARID_DEF;   /* '<Root>/CalSetOffset_V' */

/* Forward declaration for local functions */
static void CellCal_loadGains(const uint16 inV[14], uint8 q, uint16 lo, uint16
  hi, uint8 QlfOk, uint8 QlfNa, uint16 v[14], uint8 *s);

/* Function for MATLAB Function: '<S1>/CalCore' */
static void CellCal_loadGains(const uint16 inV[14], uint8 q, uint16 lo, uint16
  hi, uint8 QlfOk, uint8 QlfNa, uint16 v[14], uint8 *s)
{
  sint32 i;
  for (i = 0; i < 14; i++) {
    v[i] = 10000U;
  }

  *s = 0U;
  if (q == QlfOk) {
    *s = 1U;
    for (i = 0; i < 14; i++) {
      uint16 inV_0;
      inV_0 = inV[i];
      if (inV_0 >= lo) {
        if (inV_0 <= hi) {
          v[i] = inV_0;
        } else {
          *s = 2U;
        }
      } else {
        *s = 2U;
      }
    }
  } else if (q != QlfNa) {
    *s = 2U;
  }
}

/* Model step function for TID1 */
void CalTask10ms(void)                 /* Explicit Task: CalTask10ms */
{
  sint32 i;
  sint32 k;
  sint16 rtb_TmpSignalConversionAtSFunctionInport3[14];
  sint16 rtb_TmpSignalConversionAtSFunctionInport5[14];
  sint16 tmpRead_1[Rte_SysCon_CellCount];
  sint16 tmpRead_5[Rte_SysCon_CellCount];
  sint16 aSO;
  sint16 tmpRead_9;
  uint16 rtb_TmpSignalConversionAtSFunctionInport4[14];
  uint16 rtb_TmpSignalConversionAtSFunctionInport6[14];
  uint16 tmpRead_3[Rte_SysCon_CellCount];
  uint16 tmpRead_7[Rte_SysCon_CellCount];
  uint16 aSG;
  uint16 rtb_TmpSignalConversionAtSFunctionInport4_0;
  uint8 tmpRead_0;
  uint8 tmpRead_2;
  uint8 tmpRead_4;
  uint8 tmpRead_6;
  uint8 tmpRead_8;
  uint8 x;
  boolean req[14];
  boolean differs;
  boolean exitg1;
  boolean tmpRead;

  /* Inport: '<Root>/NvMeasGainIn' */
  (void)Rte_Read_NvMeasGainIn_NvMeasGainIn(tmpRead_7);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/StMeasGain01'
   *  Inport: '<Root>/StMeasGain02'
   *  Inport: '<Root>/StMeasGain03'
   *  Inport: '<Root>/StMeasGain04'
   *  Inport: '<Root>/StMeasGain05'
   *  Inport: '<Root>/StMeasGain06'
   *  Inport: '<Root>/StMeasGain07'
   *  Inport: '<Root>/StMeasGain08'
   *  Inport: '<Root>/StMeasGain09'
   *  Inport: '<Root>/StMeasGain10'
   *  Inport: '<Root>/StMeasGain11'
   *  Inport: '<Root>/StMeasGain12'
   *  Inport: '<Root>/StMeasGain13'
   *  Inport: '<Root>/StMeasGain14'
   */
  (void)Rte_Read_StMeasGain14_StMeasGain14
    (&rtb_TmpSignalConversionAtSFunctionInport6[13]);
  (void)Rte_Read_StMeasGain13_StMeasGain13
    (&rtb_TmpSignalConversionAtSFunctionInport6[12]);
  (void)Rte_Read_StMeasGain12_StMeasGain12
    (&rtb_TmpSignalConversionAtSFunctionInport6[11]);
  (void)Rte_Read_StMeasGain11_StMeasGain11
    (&rtb_TmpSignalConversionAtSFunctionInport6[10]);
  (void)Rte_Read_StMeasGain10_StMeasGain10
    (&rtb_TmpSignalConversionAtSFunctionInport6[9]);
  (void)Rte_Read_StMeasGain09_StMeasGain09
    (&rtb_TmpSignalConversionAtSFunctionInport6[8]);
  (void)Rte_Read_StMeasGain08_StMeasGain08
    (&rtb_TmpSignalConversionAtSFunctionInport6[7]);
  (void)Rte_Read_StMeasGain07_StMeasGain07
    (&rtb_TmpSignalConversionAtSFunctionInport6[6]);
  (void)Rte_Read_StMeasGain06_StMeasGain06
    (&rtb_TmpSignalConversionAtSFunctionInport6[5]);
  (void)Rte_Read_StMeasGain05_StMeasGain05
    (&rtb_TmpSignalConversionAtSFunctionInport6[4]);
  (void)Rte_Read_StMeasGain04_StMeasGain04
    (&rtb_TmpSignalConversionAtSFunctionInport6[3]);
  (void)Rte_Read_StMeasGain03_StMeasGain03
    (&rtb_TmpSignalConversionAtSFunctionInport6[2]);
  (void)Rte_Read_StMeasGain02_StMeasGain02
    (&rtb_TmpSignalConversionAtSFunctionInport6[1]);
  (void)Rte_Read_StMeasGain01_StMeasGain01
    (&rtb_TmpSignalConversionAtSFunctionInport6[0]);

  /* Inport: '<Root>/NvMeasOffsetIn' */
  (void)Rte_Read_NvMeasOffsetIn_NvMeasOffsetIn(tmpRead_5);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/StMeasOffset01'
   *  Inport: '<Root>/StMeasOffset02'
   *  Inport: '<Root>/StMeasOffset03'
   *  Inport: '<Root>/StMeasOffset04'
   *  Inport: '<Root>/StMeasOffset05'
   *  Inport: '<Root>/StMeasOffset06'
   *  Inport: '<Root>/StMeasOffset07'
   *  Inport: '<Root>/StMeasOffset08'
   *  Inport: '<Root>/StMeasOffset09'
   *  Inport: '<Root>/StMeasOffset10'
   *  Inport: '<Root>/StMeasOffset11'
   *  Inport: '<Root>/StMeasOffset12'
   *  Inport: '<Root>/StMeasOffset13'
   *  Inport: '<Root>/StMeasOffset14'
   */
  (void)Rte_Read_StMeasOffset14_StMeasOffset14
    (&rtb_TmpSignalConversionAtSFunctionInport5[13]);
  (void)Rte_Read_StMeasOffset13_StMeasOffset13
    (&rtb_TmpSignalConversionAtSFunctionInport5[12]);
  (void)Rte_Read_StMeasOffset12_StMeasOffset12
    (&rtb_TmpSignalConversionAtSFunctionInport5[11]);
  (void)Rte_Read_StMeasOffset11_StMeasOffset11
    (&rtb_TmpSignalConversionAtSFunctionInport5[10]);
  (void)Rte_Read_StMeasOffset10_StMeasOffset10
    (&rtb_TmpSignalConversionAtSFunctionInport5[9]);
  (void)Rte_Read_StMeasOffset09_StMeasOffset09
    (&rtb_TmpSignalConversionAtSFunctionInport5[8]);
  (void)Rte_Read_StMeasOffset08_StMeasOffset08
    (&rtb_TmpSignalConversionAtSFunctionInport5[7]);
  (void)Rte_Read_StMeasOffset07_StMeasOffset07
    (&rtb_TmpSignalConversionAtSFunctionInport5[6]);
  (void)Rte_Read_StMeasOffset06_StMeasOffset06
    (&rtb_TmpSignalConversionAtSFunctionInport5[5]);
  (void)Rte_Read_StMeasOffset05_StMeasOffset05
    (&rtb_TmpSignalConversionAtSFunctionInport5[4]);
  (void)Rte_Read_StMeasOffset04_StMeasOffset04
    (&rtb_TmpSignalConversionAtSFunctionInport5[3]);
  (void)Rte_Read_StMeasOffset03_StMeasOffset03
    (&rtb_TmpSignalConversionAtSFunctionInport5[2]);
  (void)Rte_Read_StMeasOffset02_StMeasOffset02
    (&rtb_TmpSignalConversionAtSFunctionInport5[1]);
  (void)Rte_Read_StMeasOffset01_StMeasOffset01
    (&rtb_TmpSignalConversionAtSFunctionInport5[0]);

  /* Inport: '<Root>/NvSetGainIn' */
  (void)Rte_Read_NvSetGainIn_NvSetGainIn(tmpRead_3);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/StSetGain01'
   *  Inport: '<Root>/StSetGain02'
   *  Inport: '<Root>/StSetGain03'
   *  Inport: '<Root>/StSetGain04'
   *  Inport: '<Root>/StSetGain05'
   *  Inport: '<Root>/StSetGain06'
   *  Inport: '<Root>/StSetGain07'
   *  Inport: '<Root>/StSetGain08'
   *  Inport: '<Root>/StSetGain09'
   *  Inport: '<Root>/StSetGain10'
   *  Inport: '<Root>/StSetGain11'
   *  Inport: '<Root>/StSetGain12'
   *  Inport: '<Root>/StSetGain13'
   *  Inport: '<Root>/StSetGain14'
   */
  (void)Rte_Read_StSetGain14_StSetGain14
    (&rtb_TmpSignalConversionAtSFunctionInport4[13]);
  (void)Rte_Read_StSetGain13_StSetGain13
    (&rtb_TmpSignalConversionAtSFunctionInport4[12]);
  (void)Rte_Read_StSetGain12_StSetGain12
    (&rtb_TmpSignalConversionAtSFunctionInport4[11]);
  (void)Rte_Read_StSetGain11_StSetGain11
    (&rtb_TmpSignalConversionAtSFunctionInport4[10]);
  (void)Rte_Read_StSetGain10_StSetGain10
    (&rtb_TmpSignalConversionAtSFunctionInport4[9]);
  (void)Rte_Read_StSetGain09_StSetGain09
    (&rtb_TmpSignalConversionAtSFunctionInport4[8]);
  (void)Rte_Read_StSetGain08_StSetGain08
    (&rtb_TmpSignalConversionAtSFunctionInport4[7]);
  (void)Rte_Read_StSetGain07_StSetGain07
    (&rtb_TmpSignalConversionAtSFunctionInport4[6]);
  (void)Rte_Read_StSetGain06_StSetGain06
    (&rtb_TmpSignalConversionAtSFunctionInport4[5]);
  (void)Rte_Read_StSetGain05_StSetGain05
    (&rtb_TmpSignalConversionAtSFunctionInport4[4]);
  (void)Rte_Read_StSetGain04_StSetGain04
    (&rtb_TmpSignalConversionAtSFunctionInport4[3]);
  (void)Rte_Read_StSetGain03_StSetGain03
    (&rtb_TmpSignalConversionAtSFunctionInport4[2]);
  (void)Rte_Read_StSetGain02_StSetGain02
    (&rtb_TmpSignalConversionAtSFunctionInport4[1]);
  (void)Rte_Read_StSetGain01_StSetGain01
    (&rtb_TmpSignalConversionAtSFunctionInport4[0]);

  /* Inport: '<Root>/NvSetOffsetIn' */
  (void)Rte_Read_NvSetOffsetIn_NvSetOffsetIn(tmpRead_1);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/StSetOffset01'
   *  Inport: '<Root>/StSetOffset02'
   *  Inport: '<Root>/StSetOffset03'
   *  Inport: '<Root>/StSetOffset04'
   *  Inport: '<Root>/StSetOffset05'
   *  Inport: '<Root>/StSetOffset06'
   *  Inport: '<Root>/StSetOffset07'
   *  Inport: '<Root>/StSetOffset08'
   *  Inport: '<Root>/StSetOffset09'
   *  Inport: '<Root>/StSetOffset10'
   *  Inport: '<Root>/StSetOffset11'
   *  Inport: '<Root>/StSetOffset12'
   *  Inport: '<Root>/StSetOffset13'
   *  Inport: '<Root>/StSetOffset14'
   */
  (void)Rte_Read_StSetOffset14_StSetOffset14
    (&rtb_TmpSignalConversionAtSFunctionInport3[13]);
  (void)Rte_Read_StSetOffset13_StSetOffset13
    (&rtb_TmpSignalConversionAtSFunctionInport3[12]);
  (void)Rte_Read_StSetOffset12_StSetOffset12
    (&rtb_TmpSignalConversionAtSFunctionInport3[11]);
  (void)Rte_Read_StSetOffset11_StSetOffset11
    (&rtb_TmpSignalConversionAtSFunctionInport3[10]);
  (void)Rte_Read_StSetOffset10_StSetOffset10
    (&rtb_TmpSignalConversionAtSFunctionInport3[9]);
  (void)Rte_Read_StSetOffset09_StSetOffset09
    (&rtb_TmpSignalConversionAtSFunctionInport3[8]);
  (void)Rte_Read_StSetOffset08_StSetOffset08
    (&rtb_TmpSignalConversionAtSFunctionInport3[7]);
  (void)Rte_Read_StSetOffset07_StSetOffset07
    (&rtb_TmpSignalConversionAtSFunctionInport3[6]);
  (void)Rte_Read_StSetOffset06_StSetOffset06
    (&rtb_TmpSignalConversionAtSFunctionInport3[5]);
  (void)Rte_Read_StSetOffset05_StSetOffset05
    (&rtb_TmpSignalConversionAtSFunctionInport3[4]);
  (void)Rte_Read_StSetOffset04_StSetOffset04
    (&rtb_TmpSignalConversionAtSFunctionInport3[3]);
  (void)Rte_Read_StSetOffset03_StSetOffset03
    (&rtb_TmpSignalConversionAtSFunctionInport3[2]);
  (void)Rte_Read_StSetOffset02_StSetOffset02
    (&rtb_TmpSignalConversionAtSFunctionInport3[1]);
  (void)Rte_Read_StSetOffset01_StSetOffset01
    (&rtb_TmpSignalConversionAtSFunctionInport3[0]);

  /* Inport: '<Root>/ApplyRequest' */
  (void)Rte_Read_ApplyRequest_ApplyRequest(&tmpRead_0);

  /* RootInportFunctionCallGenerator generated from: '<Root>/CalTask10ms' incorporates:
   *  SubSystem: '<Root>/CellCal_task10ms'
   */
  /* MATLAB Function: '<S1>/CalCore' incorporates:
   *  Constant: '<S1>/GainMax_code_Const'
   *  Constant: '<S1>/GainMin_code_Const'
   *  Constant: '<S1>/OffsetLimit_code_Const'
   *  Constant: '<S1>/QLF_NOT_AVAILABLE_Const'
   *  Constant: '<S1>/QLF_OK_Const'
   *  Inport: '<Root>/NvMeasGainQlf'
   *  Inport: '<Root>/NvMeasOffsetQlf'
   *  Inport: '<Root>/NvSetGainQlf'
   *  Inport: '<Root>/NvSetOffsetQlf'
   *  Inport: '<Root>/OutputsOn'
   *  Reshape: '<S1>/NvMeasGainIn_r'
   *  Reshape: '<S1>/NvMeasOffsetIn_r'
   *  Reshape: '<S1>/NvSetGainIn_r'
   *  Reshape: '<S1>/NvSetOffsetIn_r'
   */
  if (!CellCal_ARID_DEF.started_not_empty) {
    CellCal_ARID_DEF.started_not_empty = TRUE;
    for (i = 0; i < 14; i++) {
      CellCal_ARID_DEF.aSO[i] = 0;
    }

    x = 0U;
    (void)Rte_Read_NvSetOffsetQlf_NvSetOffsetQlf(&tmpRead_2);
    if (tmpRead_2 == 1) {
      x = 1U;
      for (k = 0; k < 14; k++) {
        tmpRead_9 = tmpRead_1[k];
        if (tmpRead_9 < 0) {
          i = -tmpRead_9;
        } else {
          i = tmpRead_9;
        }

        if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
          CellCal_ARID_DEF.aSO[k] = tmpRead_9;
        } else {
          x = 2U;
        }
      }
    } else if (tmpRead_2 != 64) {
      x = 2U;
    }

    (void)Rte_Read_NvSetGainQlf_NvSetGainQlf(&tmpRead_4);
    CellCal_loadGains(tmpRead_3, tmpRead_4, Rte_Prm_CalPrm_GainMin_code(),
                      Rte_Prm_CalPrm_GainMax_code(), 1, 64, CellCal_ARID_DEF.aSG,
                      &tmpRead_2);
    for (i = 0; i < 14; i++) {
      CellCal_ARID_DEF.aMO[i] = 0;
    }

    tmpRead_4 = 0U;
    (void)Rte_Read_NvMeasOffsetQlf_NvMeasOffsetQlf(&tmpRead_6);
    if (tmpRead_6 == 1) {
      tmpRead_4 = 1U;
      for (k = 0; k < 14; k++) {
        tmpRead_9 = tmpRead_5[k];
        if (tmpRead_9 < 0) {
          i = -tmpRead_9;
        } else {
          i = tmpRead_9;
        }

        if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
          CellCal_ARID_DEF.aMO[k] = tmpRead_9;
        } else {
          tmpRead_4 = 2U;
        }
      }
    } else if (tmpRead_6 != 64) {
      tmpRead_4 = 2U;
    }

    (void)Rte_Read_NvMeasGainQlf_NvMeasGainQlf(&tmpRead_8);
    CellCal_loadGains(tmpRead_7, tmpRead_8, Rte_Prm_CalPrm_GainMin_code(),
                      Rte_Prm_CalPrm_GainMax_code(), 1, 64, CellCal_ARID_DEF.aMG,
                      &CellCal_ARID_DEF.st);
    if (x >= tmpRead_2) {
      tmpRead_2 = x;
    }

    if (tmpRead_4 >= CellCal_ARID_DEF.st) {
      CellCal_ARID_DEF.st = tmpRead_4;
    }

    if (tmpRead_2 >= CellCal_ARID_DEF.st) {
      CellCal_ARID_DEF.st = tmpRead_2;
    }

    CellCal_ARID_DEF.last = tmpRead_0;
  }

  if (tmpRead_0 != CellCal_ARID_DEF.last) {
    CellCal_ARID_DEF.last = tmpRead_0;
    CellCal_ARID_DEF.res = 3U;
    CellCal_ARID_DEF.bad = 0U;
    (void)Rte_Read_OutputsOn_OutputsOn(&tmpRead);
    if (tmpRead) {
      CellCal_ARID_DEF.res = 1U;
    } else {
      for (k = 0; k < 14; k++) {
        req[k] = FALSE;
        tmpRead_9 = rtb_TmpSignalConversionAtSFunctionInport3[k];
        if (tmpRead_9 != 0) {
          tmpRead = TRUE;
        } else if (rtb_TmpSignalConversionAtSFunctionInport4[k] != 0) {
          tmpRead = TRUE;
        } else if (rtb_TmpSignalConversionAtSFunctionInport5[k] != 0) {
          tmpRead = TRUE;
        } else {
          tmpRead = (rtb_TmpSignalConversionAtSFunctionInport6[k] != 0);
        }

        if (tmpRead_9 != CellCal_ARID_DEF.aSO[k]) {
          differs = TRUE;
        } else if (rtb_TmpSignalConversionAtSFunctionInport4[k] !=
                   CellCal_ARID_DEF.aSG[k]) {
          differs = TRUE;
        } else if (rtb_TmpSignalConversionAtSFunctionInport5[k] !=
                   CellCal_ARID_DEF.aMO[k]) {
          differs = TRUE;
        } else {
          differs = (rtb_TmpSignalConversionAtSFunctionInport6[k] !=
                     CellCal_ARID_DEF.aMG[k]);
        }

        if (tmpRead && differs) {
          req[k] = TRUE;
          if (tmpRead_9 < 0) {
            i = -tmpRead_9;
          } else {
            i = tmpRead_9;
          }

          if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
            tmpRead_9 = rtb_TmpSignalConversionAtSFunctionInport5[k];
            if (tmpRead_9 < 0) {
              i = -tmpRead_9;
            } else {
              i = tmpRead_9;
            }

            if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
              rtb_TmpSignalConversionAtSFunctionInport4_0 =
                rtb_TmpSignalConversionAtSFunctionInport4[k];
              if (rtb_TmpSignalConversionAtSFunctionInport4_0 >=
                  Rte_Prm_CalPrm_GainMin_code()) {
                if (rtb_TmpSignalConversionAtSFunctionInport4_0 <=
                    Rte_Prm_CalPrm_GainMax_code()) {
                  rtb_TmpSignalConversionAtSFunctionInport4_0 =
                    rtb_TmpSignalConversionAtSFunctionInport6[k];
                  tmpRead = ((rtb_TmpSignalConversionAtSFunctionInport4_0 >=
                              Rte_Prm_CalPrm_GainMin_code()) &&
                             (rtb_TmpSignalConversionAtSFunctionInport4_0 <=
                              Rte_Prm_CalPrm_GainMax_code()));
                } else {
                  tmpRead = FALSE;
                }
              } else {
                tmpRead = FALSE;
              }
            } else {
              tmpRead = FALSE;
            }
          } else {
            tmpRead = FALSE;
          }

          if ((!tmpRead) && (CellCal_ARID_DEF.bad == 0)) {
            CellCal_ARID_DEF.bad = (uint8)(k + 1);
          }
        }
      }

      if (CellCal_ARID_DEF.bad != 0) {
        CellCal_ARID_DEF.res = 2U;
      } else {
        tmpRead = FALSE;
        i = 0;
        exitg1 = FALSE;
        while ((!exitg1) && (i < 14)) {
          if (req[i]) {
            tmpRead = TRUE;
            exitg1 = TRUE;
          } else {
            i++;
          }
        }

        if (tmpRead) {
          for (i = 0; i < 14; i++) {
            if (req[i]) {
              CellCal_ARID_DEF.aSO[i] =
                rtb_TmpSignalConversionAtSFunctionInport3[i];
              CellCal_ARID_DEF.aSG[i] =
                rtb_TmpSignalConversionAtSFunctionInport4[i];
              CellCal_ARID_DEF.aMO[i] =
                rtb_TmpSignalConversionAtSFunctionInport5[i];
              CellCal_ARID_DEF.aMG[i] =
                rtb_TmpSignalConversionAtSFunctionInport6[i];
            }
          }

          CellCal_ARID_DEF.res = 0U;
          if (CellCal_ARID_DEF.st == 0) {
            CellCal_ARID_DEF.st = 1U;
          }
        }
      }
    }
  }

  /* Outport: '<Root>/ApplyResult' incorporates:
   *  MATLAB Function: '<S1>/CalCore'
   */
  (void)Rte_Write_ApplyResult_ApplyResult(CellCal_ARID_DEF.res);

  /* Outport: '<Root>/ApplyBadChannel' incorporates:
   *  MATLAB Function: '<S1>/CalCore'
   */
  (void)Rte_Write_ApplyBadChannel_ApplyBadChannel(CellCal_ARID_DEF.bad);

  /* Outport: '<Root>/CalState' incorporates:
   *  MATLAB Function: '<S1>/CalCore'
   */
  (void)Rte_Write_CalState_CalState(CellCal_ARID_DEF.st);
  for (i = 0; i < Rte_SysCon_CellCount; i++) {
    /* MATLAB Function: '<S1>/CalCore' */
    rtb_TmpSignalConversionAtSFunctionInport4_0 = CellCal_ARID_DEF.aMG[i];

    /* Outport: '<Root>/CalMeasGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalMeasGain_r'
     */
    CellCal_ARID_DEF.CalMeasGain[i] = (float32)
      rtb_TmpSignalConversionAtSFunctionInport4_0 * 0.0001F;

    /* MATLAB Function: '<S1>/CalCore' */
    tmpRead_9 = CellCal_ARID_DEF.aMO[i];

    /* Outport: '<Root>/CalMeasOffset_V' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalMeasOffset_V_r'
     */
    CellCal_ARID_DEF.CalMeasOffset_V[i] = (float32)tmpRead_9 * 0.0001F;

    /* MATLAB Function: '<S1>/CalCore' */
    aSG = CellCal_ARID_DEF.aSG[i];

    /* Outport: '<Root>/CalSetGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalSetGain_r'
     */
    CellCal_ARID_DEF.CalSetGain[i] = (float32)aSG * 0.0001F;

    /* MATLAB Function: '<S1>/CalCore' */
    aSO = CellCal_ARID_DEF.aSO[i];

    /* Outport: '<Root>/CalSetOffset_V' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalSetOffset_V_r'
     */
    CellCal_ARID_DEF.CalSetOffset_V[i] = (float32)aSO * 0.0001F;

    /* Outport: '<Root>/NvMeasGainOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvMeasGainOut_r'
     */
    CellCal_ARID_DEF.NvMeasGainOut[i] =
      rtb_TmpSignalConversionAtSFunctionInport4_0;

    /* Outport: '<Root>/NvMeasOffsetOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvMeasOffsetOut_r'
     */
    CellCal_ARID_DEF.NvMeasOffsetOut[i] = tmpRead_9;

    /* Outport: '<Root>/NvSetGainOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvSetGainOut_r'
     */
    CellCal_ARID_DEF.NvSetGainOut[i] = aSG;

    /* Outport: '<Root>/NvSetOffsetOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvSetOffsetOut_r'
     */
    CellCal_ARID_DEF.NvSetOffsetOut[i] = aSO;
  }

  /* Outport: '<Root>/AcMeasGain02' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain02'
   */
  (void)Rte_Write_AcMeasGain02_AcMeasGain02(CellCal_ARID_DEF.aMG[1]);

  /* Outport: '<Root>/AcMeasGain03' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain03'
   */
  (void)Rte_Write_AcMeasGain03_AcMeasGain03(CellCal_ARID_DEF.aMG[2]);

  /* Outport: '<Root>/AcMeasGain04' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain04'
   */
  (void)Rte_Write_AcMeasGain04_AcMeasGain04(CellCal_ARID_DEF.aMG[3]);

  /* Outport: '<Root>/AcMeasGain05' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain05'
   */
  (void)Rte_Write_AcMeasGain05_AcMeasGain05(CellCal_ARID_DEF.aMG[4]);

  /* Outport: '<Root>/AcMeasGain06' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain06'
   */
  (void)Rte_Write_AcMeasGain06_AcMeasGain06(CellCal_ARID_DEF.aMG[5]);

  /* Outport: '<Root>/AcMeasGain07' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain07'
   */
  (void)Rte_Write_AcMeasGain07_AcMeasGain07(CellCal_ARID_DEF.aMG[6]);

  /* Outport: '<Root>/AcMeasGain08' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain08'
   */
  (void)Rte_Write_AcMeasGain08_AcMeasGain08(CellCal_ARID_DEF.aMG[7]);

  /* Outport: '<Root>/AcMeasGain09' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain09'
   */
  (void)Rte_Write_AcMeasGain09_AcMeasGain09(CellCal_ARID_DEF.aMG[8]);

  /* Outport: '<Root>/AcMeasGain10' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain10'
   */
  (void)Rte_Write_AcMeasGain10_AcMeasGain10(CellCal_ARID_DEF.aMG[9]);

  /* Outport: '<Root>/AcMeasGain11' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain11'
   */
  (void)Rte_Write_AcMeasGain11_AcMeasGain11(CellCal_ARID_DEF.aMG[10]);

  /* Outport: '<Root>/AcMeasGain12' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain12'
   */
  (void)Rte_Write_AcMeasGain12_AcMeasGain12(CellCal_ARID_DEF.aMG[11]);

  /* Outport: '<Root>/AcMeasGain13' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain13'
   */
  (void)Rte_Write_AcMeasGain13_AcMeasGain13(CellCal_ARID_DEF.aMG[12]);

  /* Outport: '<Root>/AcMeasGain14' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasGain14'
   */
  (void)Rte_Write_AcMeasGain14_AcMeasGain14(CellCal_ARID_DEF.aMG[13]);

  /* Outport: '<Root>/AcMeasOffset02' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset02'
   */
  (void)Rte_Write_AcMeasOffset02_AcMeasOffset02(CellCal_ARID_DEF.aMO[1]);

  /* Outport: '<Root>/AcMeasOffset03' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset03'
   */
  (void)Rte_Write_AcMeasOffset03_AcMeasOffset03(CellCal_ARID_DEF.aMO[2]);

  /* Outport: '<Root>/AcMeasOffset04' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset04'
   */
  (void)Rte_Write_AcMeasOffset04_AcMeasOffset04(CellCal_ARID_DEF.aMO[3]);

  /* Outport: '<Root>/AcMeasOffset05' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset05'
   */
  (void)Rte_Write_AcMeasOffset05_AcMeasOffset05(CellCal_ARID_DEF.aMO[4]);

  /* Outport: '<Root>/AcMeasOffset06' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset06'
   */
  (void)Rte_Write_AcMeasOffset06_AcMeasOffset06(CellCal_ARID_DEF.aMO[5]);

  /* Outport: '<Root>/AcMeasOffset07' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset07'
   */
  (void)Rte_Write_AcMeasOffset07_AcMeasOffset07(CellCal_ARID_DEF.aMO[6]);

  /* Outport: '<Root>/AcMeasOffset08' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset08'
   */
  (void)Rte_Write_AcMeasOffset08_AcMeasOffset08(CellCal_ARID_DEF.aMO[7]);

  /* Outport: '<Root>/AcMeasOffset09' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset09'
   */
  (void)Rte_Write_AcMeasOffset09_AcMeasOffset09(CellCal_ARID_DEF.aMO[8]);

  /* Outport: '<Root>/AcMeasOffset10' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset10'
   */
  (void)Rte_Write_AcMeasOffset10_AcMeasOffset10(CellCal_ARID_DEF.aMO[9]);

  /* Outport: '<Root>/AcMeasOffset11' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset11'
   */
  (void)Rte_Write_AcMeasOffset11_AcMeasOffset11(CellCal_ARID_DEF.aMO[10]);

  /* Outport: '<Root>/AcMeasOffset12' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset12'
   */
  (void)Rte_Write_AcMeasOffset12_AcMeasOffset12(CellCal_ARID_DEF.aMO[11]);

  /* Outport: '<Root>/AcMeasOffset13' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset13'
   */
  (void)Rte_Write_AcMeasOffset13_AcMeasOffset13(CellCal_ARID_DEF.aMO[12]);

  /* Outport: '<Root>/AcMeasOffset14' incorporates:
   *  SignalConversion generated from: '<S1>/AcMeasOffset14'
   */
  (void)Rte_Write_AcMeasOffset14_AcMeasOffset14(CellCal_ARID_DEF.aMO[13]);

  /* Outport: '<Root>/AcSetGain02' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain02'
   */
  (void)Rte_Write_AcSetGain02_AcSetGain02(CellCal_ARID_DEF.aSG[1]);

  /* Outport: '<Root>/AcSetGain03' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain03'
   */
  (void)Rte_Write_AcSetGain03_AcSetGain03(CellCal_ARID_DEF.aSG[2]);

  /* Outport: '<Root>/AcSetGain04' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain04'
   */
  (void)Rte_Write_AcSetGain04_AcSetGain04(CellCal_ARID_DEF.aSG[3]);

  /* Outport: '<Root>/AcSetGain05' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain05'
   */
  (void)Rte_Write_AcSetGain05_AcSetGain05(CellCal_ARID_DEF.aSG[4]);

  /* Outport: '<Root>/AcSetGain06' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain06'
   */
  (void)Rte_Write_AcSetGain06_AcSetGain06(CellCal_ARID_DEF.aSG[5]);

  /* Outport: '<Root>/AcSetGain07' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain07'
   */
  (void)Rte_Write_AcSetGain07_AcSetGain07(CellCal_ARID_DEF.aSG[6]);

  /* Outport: '<Root>/AcSetGain08' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain08'
   */
  (void)Rte_Write_AcSetGain08_AcSetGain08(CellCal_ARID_DEF.aSG[7]);

  /* Outport: '<Root>/AcSetGain09' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain09'
   */
  (void)Rte_Write_AcSetGain09_AcSetGain09(CellCal_ARID_DEF.aSG[8]);

  /* Outport: '<Root>/AcSetGain10' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain10'
   */
  (void)Rte_Write_AcSetGain10_AcSetGain10(CellCal_ARID_DEF.aSG[9]);

  /* Outport: '<Root>/AcSetGain11' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain11'
   */
  (void)Rte_Write_AcSetGain11_AcSetGain11(CellCal_ARID_DEF.aSG[10]);

  /* Outport: '<Root>/AcSetGain12' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain12'
   */
  (void)Rte_Write_AcSetGain12_AcSetGain12(CellCal_ARID_DEF.aSG[11]);

  /* Outport: '<Root>/AcSetGain13' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain13'
   */
  (void)Rte_Write_AcSetGain13_AcSetGain13(CellCal_ARID_DEF.aSG[12]);

  /* Outport: '<Root>/AcSetGain14' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetGain14'
   */
  (void)Rte_Write_AcSetGain14_AcSetGain14(CellCal_ARID_DEF.aSG[13]);

  /* Outport: '<Root>/AcSetOffset02' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset02'
   */
  (void)Rte_Write_AcSetOffset02_AcSetOffset02(CellCal_ARID_DEF.aSO[1]);

  /* Outport: '<Root>/AcSetOffset03' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset03'
   */
  (void)Rte_Write_AcSetOffset03_AcSetOffset03(CellCal_ARID_DEF.aSO[2]);

  /* Outport: '<Root>/AcSetOffset04' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset04'
   */
  (void)Rte_Write_AcSetOffset04_AcSetOffset04(CellCal_ARID_DEF.aSO[3]);

  /* Outport: '<Root>/AcSetOffset05' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset05'
   */
  (void)Rte_Write_AcSetOffset05_AcSetOffset05(CellCal_ARID_DEF.aSO[4]);

  /* Outport: '<Root>/AcSetOffset06' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset06'
   */
  (void)Rte_Write_AcSetOffset06_AcSetOffset06(CellCal_ARID_DEF.aSO[5]);

  /* Outport: '<Root>/AcSetOffset07' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset07'
   */
  (void)Rte_Write_AcSetOffset07_AcSetOffset07(CellCal_ARID_DEF.aSO[6]);

  /* Outport: '<Root>/AcSetOffset08' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset08'
   */
  (void)Rte_Write_AcSetOffset08_AcSetOffset08(CellCal_ARID_DEF.aSO[7]);

  /* Outport: '<Root>/AcSetOffset09' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset09'
   */
  (void)Rte_Write_AcSetOffset09_AcSetOffset09(CellCal_ARID_DEF.aSO[8]);

  /* Outport: '<Root>/AcSetOffset10' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset10'
   */
  (void)Rte_Write_AcSetOffset10_AcSetOffset10(CellCal_ARID_DEF.aSO[9]);

  /* Outport: '<Root>/AcSetOffset11' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset11'
   */
  (void)Rte_Write_AcSetOffset11_AcSetOffset11(CellCal_ARID_DEF.aSO[10]);

  /* Outport: '<Root>/AcSetOffset12' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset12'
   */
  (void)Rte_Write_AcSetOffset12_AcSetOffset12(CellCal_ARID_DEF.aSO[11]);

  /* Outport: '<Root>/AcSetOffset13' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset13'
   */
  (void)Rte_Write_AcSetOffset13_AcSetOffset13(CellCal_ARID_DEF.aSO[12]);

  /* Outport: '<Root>/AcSetOffset14' incorporates:
   *  SignalConversion generated from: '<S1>/AcSetOffset14'
   */
  (void)Rte_Write_AcSetOffset14_AcSetOffset14(CellCal_ARID_DEF.aSO[13]);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/CalTask10ms' */

  /* Outport: '<Root>/AcSetOffset01' */
  (void)Rte_Write_AcSetOffset01_AcSetOffset01(CellCal_ARID_DEF.aSO[0]);

  /* Outport: '<Root>/AcSetGain01' */
  (void)Rte_Write_AcSetGain01_AcSetGain01(CellCal_ARID_DEF.aSG[0]);

  /* Outport: '<Root>/AcMeasOffset01' */
  (void)Rte_Write_AcMeasOffset01_AcMeasOffset01(CellCal_ARID_DEF.aMO[0]);

  /* Outport: '<Root>/AcMeasGain01' */
  (void)Rte_Write_AcMeasGain01_AcMeasGain01(CellCal_ARID_DEF.aMG[0]);
  (void)Rte_Write_CalMeasGain_CalMeasGain(CellCal_ARID_DEF.CalMeasGain);
  (void)Rte_Write_CalMeasOffset_V_CalMeasOffset_V
    (CellCal_ARID_DEF.CalMeasOffset_V);
  (void)Rte_Write_CalSetGain_CalSetGain(CellCal_ARID_DEF.CalSetGain);
  (void)Rte_Write_CalSetOffset_V_CalSetOffset_V(CellCal_ARID_DEF.CalSetOffset_V);
  (void)Rte_Write_NvMeasGainOut_NvMeasGainOut(CellCal_ARID_DEF.NvMeasGainOut);
  (void)Rte_Write_NvMeasOffsetOut_NvMeasOffsetOut
    (CellCal_ARID_DEF.NvMeasOffsetOut);
  (void)Rte_Write_NvSetGainOut_NvSetGainOut(CellCal_ARID_DEF.NvSetGainOut);
  (void)Rte_Write_NvSetOffsetOut_NvSetOffsetOut(CellCal_ARID_DEF.NvSetOffsetOut);
}

/* Model initialize function */
void CellCal_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/CalTask10ms' incorporates:
   *  SubSystem: '<Root>/CellCal_task10ms'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/CalCore' */
  CellCal_ARID_DEF.res = 3U;

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/CalTask10ms' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
