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
static void CellCal_loadOffsets(const sint16 inV[14], uint8 q, uint16 lim, uint8
  QlfOk, uint8 QlfNa, sint16 v[14], uint8 *s);
static void CellCal_loadGains(const uint16 inV[14], uint8 q, uint16 lo, uint16
  hi, uint8 QlfOk, uint8 QlfNa, uint16 v[14], uint8 *s);

/* Function for MATLAB Function: '<S1>/CalCore' */
static void CellCal_loadOffsets(const sint16 inV[14], uint8 q, uint16 lim, uint8
  QlfOk, uint8 QlfNa, sint16 v[14], uint8 *s)
{
  sint32 i;
  for (i = 0; i < 14; i++) {
    v[i] = 0;
  }

  *s = 0U;
  if ((q == QlfOk) || (q == QlfNa)) {
    if (q == QlfOk) {
      *s = 1U;
    }

    for (i = 0; i < 14; i++) {
      sint32 tmp;
      sint16 inV_0;
      inV_0 = inV[i];
      if (inV_0 < 0) {
        tmp = -inV_0;
      } else {
        tmp = inV_0;
      }

      if (tmp <= lim) {
        v[i] = inV_0;
      } else {
        *s = 2U;
      }
    }
  } else if (q != QlfNa) {
    *s = 2U;
  }
}

/* Function for MATLAB Function: '<S1>/CalCore' */
static void CellCal_loadGains(const uint16 inV[14], uint8 q, uint16 lo, uint16
  hi, uint8 QlfOk, uint8 QlfNa, uint16 v[14], uint8 *s)
{
  sint32 i;
  for (i = 0; i < 14; i++) {
    v[i] = 10000U;
  }

  *s = 0U;
  if ((q == QlfOk) || (q == QlfNa)) {
    if (q == QlfOk) {
      *s = 1U;
    }

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
  sint16 tmpRead_1[Rte_SysCon_CellCount];
  sint16 tmpRead_2[Rte_SysCon_CellCount];
  sint16 tmpRead_7[Rte_SysCon_CellCount];
  sint16 tmpRead_8[Rte_SysCon_CellCount];
  sint16 aSO;
  sint16 tmpRead_d;
  uint16 tmpRead_4[Rte_SysCon_CellCount];
  uint16 tmpRead_5[Rte_SysCon_CellCount];
  uint16 tmpRead_a[Rte_SysCon_CellCount];
  uint16 tmpRead_b[Rte_SysCon_CellCount];
  uint16 aSG;
  uint16 tmpRead_e;
  uint8 tmpRead_0;
  uint8 tmpRead_3;
  uint8 tmpRead_6;
  uint8 tmpRead_9;
  uint8 tmpRead_c;
  uint8 x;
  boolean req[14];
  boolean differs;
  boolean exitg1;
  boolean tmpRead;

  /* Inport: '<Root>/NvMeasGainIn' */
  (void)Rte_Read_NvMeasGainIn_NvMeasGainIn(tmpRead_b);

  /* Inport: '<Root>/StMeasGain' */
  (void)Rte_Read_StMeasGain_StMeasGain(tmpRead_a);

  /* Inport: '<Root>/NvMeasOffsetIn' */
  (void)Rte_Read_NvMeasOffsetIn_NvMeasOffsetIn(tmpRead_8);

  /* Inport: '<Root>/StMeasOffset' */
  (void)Rte_Read_StMeasOffset_StMeasOffset(tmpRead_7);

  /* Inport: '<Root>/NvSetGainIn' */
  (void)Rte_Read_NvSetGainIn_NvSetGainIn(tmpRead_5);

  /* Inport: '<Root>/StSetGain' */
  (void)Rte_Read_StSetGain_StSetGain(tmpRead_4);

  /* Inport: '<Root>/NvSetOffsetIn' */
  (void)Rte_Read_NvSetOffsetIn_NvSetOffsetIn(tmpRead_2);

  /* Inport: '<Root>/StSetOffset' */
  (void)Rte_Read_StSetOffset_StSetOffset(tmpRead_1);

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
   *  Inport: '<Root>/OutputEnable'
   *  Reshape: '<S1>/NvMeasGainIn_r'
   *  Reshape: '<S1>/NvMeasOffsetIn_r'
   *  Reshape: '<S1>/NvSetGainIn_r'
   *  Reshape: '<S1>/NvSetOffsetIn_r'
   *  Reshape: '<S1>/StMeasGain_r'
   *  Reshape: '<S1>/StMeasOffset_r'
   *  Reshape: '<S1>/StSetGain_r'
   *  Reshape: '<S1>/StSetOffset_r'
   */
  if (!CellCal_ARID_DEF.started_not_empty) {
    (void)Rte_Read_NvMeasGainQlf_NvMeasGainQlf(&tmpRead_c);
    (void)Rte_Read_NvMeasOffsetQlf_NvMeasOffsetQlf(&tmpRead_9);
    (void)Rte_Read_NvSetGainQlf_NvSetGainQlf(&tmpRead_6);
    (void)Rte_Read_NvSetOffsetQlf_NvSetOffsetQlf(&tmpRead_3);
    CellCal_ARID_DEF.started_not_empty = TRUE;
    CellCal_loadOffsets(tmpRead_2, tmpRead_3, Rte_Prm_CalPrm_OffsetLimit_code(),
                        1, 64, CellCal_ARID_DEF.aSO, &x);
    CellCal_loadGains(tmpRead_5, tmpRead_6, Rte_Prm_CalPrm_GainMin_code(),
                      Rte_Prm_CalPrm_GainMax_code(), 1, 64, CellCal_ARID_DEF.aSG,
                      &tmpRead_3);
    CellCal_loadOffsets(tmpRead_8, tmpRead_9, Rte_Prm_CalPrm_OffsetLimit_code(),
                        1, 64, CellCal_ARID_DEF.aMO, &tmpRead_6);
    CellCal_loadGains(tmpRead_b, tmpRead_c, Rte_Prm_CalPrm_GainMin_code(),
                      Rte_Prm_CalPrm_GainMax_code(), 1, 64, CellCal_ARID_DEF.aMG,
                      &CellCal_ARID_DEF.st);
    if (x >= tmpRead_3) {
      tmpRead_3 = x;
    }

    if (tmpRead_6 >= CellCal_ARID_DEF.st) {
      CellCal_ARID_DEF.st = tmpRead_6;
    }

    if (tmpRead_3 >= CellCal_ARID_DEF.st) {
      CellCal_ARID_DEF.st = tmpRead_3;
    }

    CellCal_ARID_DEF.last = tmpRead_0;
  }

  if (tmpRead_0 != CellCal_ARID_DEF.last) {
    CellCal_ARID_DEF.last = tmpRead_0;
    CellCal_ARID_DEF.res = 3U;
    CellCal_ARID_DEF.bad = 0U;
    (void)Rte_Read_OutputEnable_OutputEnable(&tmpRead);
    if (tmpRead) {
      CellCal_ARID_DEF.res = 1U;
    } else {
      for (k = 0; k < 14; k++) {
        req[k] = FALSE;
        tmpRead_d = tmpRead_1[k];
        if (tmpRead_d != 0) {
          tmpRead = TRUE;
        } else if (tmpRead_4[k] != 0) {
          tmpRead = TRUE;
        } else if (tmpRead_7[k] != 0) {
          tmpRead = TRUE;
        } else {
          tmpRead = (tmpRead_a[k] != 0);
        }

        if (tmpRead_d != CellCal_ARID_DEF.aSO[k]) {
          differs = TRUE;
        } else if (tmpRead_4[k] != CellCal_ARID_DEF.aSG[k]) {
          differs = TRUE;
        } else if (tmpRead_7[k] != CellCal_ARID_DEF.aMO[k]) {
          differs = TRUE;
        } else {
          differs = (tmpRead_a[k] != CellCal_ARID_DEF.aMG[k]);
        }

        if (tmpRead && differs) {
          req[k] = TRUE;
          if (tmpRead_d < 0) {
            i = -tmpRead_d;
          } else {
            i = tmpRead_d;
          }

          if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
            tmpRead_d = tmpRead_7[k];
            if (tmpRead_d < 0) {
              i = -tmpRead_d;
            } else {
              i = tmpRead_d;
            }

            if (i <= Rte_Prm_CalPrm_OffsetLimit_code()) {
              tmpRead_e = tmpRead_4[k];
              if (tmpRead_e >= Rte_Prm_CalPrm_GainMin_code()) {
                if (tmpRead_e <= Rte_Prm_CalPrm_GainMax_code()) {
                  tmpRead_e = tmpRead_a[k];
                  tmpRead = ((tmpRead_e >= Rte_Prm_CalPrm_GainMin_code()) &&
                             (tmpRead_e <= Rte_Prm_CalPrm_GainMax_code()));
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
        k = 0;
        exitg1 = FALSE;
        while ((!exitg1) && (k < 14)) {
          if (req[k]) {
            tmpRead = TRUE;
            exitg1 = TRUE;
          } else {
            k++;
          }
        }

        if (tmpRead) {
          for (k = 0; k < 14; k++) {
            if (req[k]) {
              CellCal_ARID_DEF.aSO[k] = tmpRead_1[k];
              CellCal_ARID_DEF.aSG[k] = tmpRead_4[k];
              CellCal_ARID_DEF.aMO[k] = tmpRead_7[k];
              CellCal_ARID_DEF.aMG[k] = tmpRead_a[k];
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
    tmpRead_e = CellCal_ARID_DEF.aMG[i];

    /* Outport: '<Root>/AcMeasGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/AcMeasGain_r'
     */
    CellCal_ARID_DEF.AcMeasGain[i] = tmpRead_e;

    /* MATLAB Function: '<S1>/CalCore' */
    tmpRead_d = CellCal_ARID_DEF.aMO[i];

    /* Outport: '<Root>/AcMeasOffset' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/AcMeasOffset_r'
     */
    CellCal_ARID_DEF.AcMeasOffset[i] = tmpRead_d;

    /* MATLAB Function: '<S1>/CalCore' */
    aSG = CellCal_ARID_DEF.aSG[i];

    /* Outport: '<Root>/AcSetGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/AcSetGain_r'
     */
    CellCal_ARID_DEF.AcSetGain[i] = aSG;

    /* MATLAB Function: '<S1>/CalCore' */
    aSO = CellCal_ARID_DEF.aSO[i];

    /* Outport: '<Root>/AcSetOffset' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/AcSetOffset_r'
     */
    CellCal_ARID_DEF.AcSetOffset[i] = aSO;

    /* Outport: '<Root>/CalMeasGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalMeasGain_r'
     */
    CellCal_ARID_DEF.CalMeasGain[i] = (float32)tmpRead_e * 0.0001F;

    /* Outport: '<Root>/CalMeasOffset_V' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalMeasOffset_V_r'
     */
    CellCal_ARID_DEF.CalMeasOffset_V[i] = (float32)tmpRead_d * 0.0001F;

    /* Outport: '<Root>/CalSetGain' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalSetGain_r'
     */
    CellCal_ARID_DEF.CalSetGain[i] = (float32)aSG * 0.0001F;

    /* Outport: '<Root>/CalSetOffset_V' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/CalSetOffset_V_r'
     */
    CellCal_ARID_DEF.CalSetOffset_V[i] = (float32)aSO * 0.0001F;

    /* Outport: '<Root>/NvMeasGainOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvMeasGainOut_r'
     */
    CellCal_ARID_DEF.NvMeasGainOut[i] = tmpRead_e;

    /* Outport: '<Root>/NvMeasOffsetOut' incorporates:
     *  MATLAB Function: '<S1>/CalCore'
     *  Reshape: '<S1>/NvMeasOffsetOut_r'
     */
    CellCal_ARID_DEF.NvMeasOffsetOut[i] = tmpRead_d;

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

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/CalTask10ms' */
  (void)Rte_Write_AcMeasGain_AcMeasGain(CellCal_ARID_DEF.AcMeasGain);
  (void)Rte_Write_AcMeasOffset_AcMeasOffset(CellCal_ARID_DEF.AcMeasOffset);
  (void)Rte_Write_AcSetGain_AcSetGain(CellCal_ARID_DEF.AcSetGain);
  (void)Rte_Write_AcSetOffset_AcSetOffset(CellCal_ARID_DEF.AcSetOffset);
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
