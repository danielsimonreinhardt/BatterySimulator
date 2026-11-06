/*
 * File: DisplayDriver.c
 *
 * Code generated for Simulink model 'DisplayDriver'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "DisplayDriver.h"
#include "Platform_Types.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_DisplayDriver_T DisplayDriver_ARID_DEF;/* '<S1>/DispCore' */

/* Model step function for TID1 */
void DispTask10ms(void)                /* Explicit Task: DispTask10ms */
{
  sint32 qY;
  uint32 b_x_tmp;
  uint16 digits_idx_1;
  uint16 digits_idx_2;
  uint16 digits_idx_3;
  uint16 v;
  uint8 n;
  boolean guard1;
  boolean tmpRead;

  /* Inport: '<Root>/DispTxDone' */
  (void)Rte_Read_DispTxDone_DispTxDone(&n);

  /* RootInportFunctionCallGenerator generated from: '<Root>/DispTask10ms' incorporates:
   *  SubSystem: '<Root>/DisplayDriver_task10ms'
   */
  /* MATLAB Function: '<S1>/DispCore' incorporates:
   *  Constant: '<S1>/DisplayRetry_cycles_Const'
   *  Inport: '<Root>/DispTxError'
   *  Inport: '<Root>/DisplayValue_mV'
   */
  if (DisplayDriver_ARID_DEF.inflight && (n == DisplayDriver_ARID_DEF.req)) {
    DisplayDriver_ARID_DEF.inflight = FALSE;
    (void)Rte_Read_DispTxError_DispTxError(&tmpRead);
    if (tmpRead) {
      DisplayDriver_ARID_DEF.step = 0U;
      DisplayDriver_ARID_DEF.needClear = TRUE;
      DisplayDriver_ARID_DEF.shown = ((uint16)(65535U));
      DisplayDriver_ARID_DEF.wait = Rte_Prm_CalPrm_DisplayRetry_cycles();
    }
  }

  if (DisplayDriver_ARID_DEF.wait > 0) {
    /* MW:begin MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA CPP rule violation' */
    qY = (sint32)(DisplayDriver_ARID_DEF.wait - /*MW:OvSatOk*/ 1U);

    /* MW:end MISRA2012:D4.1 CERT-C:INT30-C */
    if (DisplayDriver_ARID_DEF.wait - 1U > DisplayDriver_ARID_DEF.wait) {
      qY = 0;
    }

    DisplayDriver_ARID_DEF.wait = (uint16)qY;
  }

  if (DisplayDriver_ARID_DEF.age < 65535) {
    DisplayDriver_ARID_DEF.age++;
  }

  if ((!DisplayDriver_ARID_DEF.inflight) && (DisplayDriver_ARID_DEF.wait == 0))
  {
    guard1 = FALSE;
    if (DisplayDriver_ARID_DEF.step == 0) {
      (void)Rte_Read_DisplayValue_mV_DisplayValue_mV(&v);
      if (v > 9999) {
        v = 9999U;
      }

      if (DisplayDriver_ARID_DEF.needClear || (v != DisplayDriver_ARID_DEF.shown)
          || (DisplayDriver_ARID_DEF.age >= Rte_Prm_CalPrm_DisplayRefresh_cycles
              ())) {
        n = 0U;
        if (DisplayDriver_ARID_DEF.needClear) {
          n = 1U;
          DisplayDriver_ARID_DEF.frame[0] = 118U;
        }

        digits_idx_1 = (uint16)(v / 100U);
        b_x_tmp = v / 10U;
        digits_idx_1 = (uint16)(digits_idx_1 - (uint32)((sint32)(digits_idx_1 /
          10U) * 10));
        digits_idx_2 = (uint16)((uint16)b_x_tmp - (uint32)((sint32)((uint16)
          b_x_tmp / 10U) * 10));
        digits_idx_3 = (uint16)(v - (uint32)((sint32)b_x_tmp * 10));
        DisplayDriver_ARID_DEF.frame[n] = 121U;
        DisplayDriver_ARID_DEF.frame[n + 1] = 0U;
        DisplayDriver_ARID_DEF.frame[n + 2] = (uint8)(v / 1000U);
        n = (uint8)(n + 3);
        DisplayDriver_ARID_DEF.frame[n] = 121U;
        DisplayDriver_ARID_DEF.frame[n + 1] = 1U;
        if (digits_idx_1 > 255) {
          digits_idx_1 = 255U;
        }

        DisplayDriver_ARID_DEF.frame[n + 2] = (uint8)digits_idx_1;
        n = (uint8)(n + 3);
        DisplayDriver_ARID_DEF.frame[n] = 121U;
        DisplayDriver_ARID_DEF.frame[n + 1] = 2U;
        if (digits_idx_2 > 255) {
          digits_idx_2 = 255U;
        }

        DisplayDriver_ARID_DEF.frame[n + 2] = (uint8)digits_idx_2;
        n = (uint8)(n + 3);
        DisplayDriver_ARID_DEF.frame[n] = 121U;
        DisplayDriver_ARID_DEF.frame[n + 1] = 3U;
        if (digits_idx_3 > 255) {
          digits_idx_3 = 255U;
        }

        DisplayDriver_ARID_DEF.frame[n + 2] = (uint8)digits_idx_3;
        DisplayDriver_ARID_DEF.frame[n + 3] = 119U;
        DisplayDriver_ARID_DEF.frame[n + 4] = 1U;
        DisplayDriver_ARID_DEF.len = (uint8)(n + 5);
        DisplayDriver_ARID_DEF.step = 1U;
        DisplayDriver_ARID_DEF.shown = v;
        guard1 = TRUE;
      }
    } else {
      guard1 = TRUE;
    }

    if (guard1) {
      DisplayDriver_ARID_DEF.outByte =
        DisplayDriver_ARID_DEF.frame[DisplayDriver_ARID_DEF.step - 1];
      qY = (sint32)(DisplayDriver_ARID_DEF.req + 1U);
      if (DisplayDriver_ARID_DEF.req + 1U > 255U) {
        qY = 255;
      }

      DisplayDriver_ARID_DEF.req = (uint8)qY;
      DisplayDriver_ARID_DEF.inflight = TRUE;
      if (DisplayDriver_ARID_DEF.step == DisplayDriver_ARID_DEF.len) {
        DisplayDriver_ARID_DEF.step = 0U;
        DisplayDriver_ARID_DEF.age = 0U;
        DisplayDriver_ARID_DEF.needClear = FALSE;
      } else {
        qY = (sint32)(DisplayDriver_ARID_DEF.step + 1U);
        if (DisplayDriver_ARID_DEF.step + 1U > 255U) {
          qY = 255;
        }

        DisplayDriver_ARID_DEF.step = (uint8)qY;
      }
    }
  }

  /* Outport: '<Root>/DispTxByte' incorporates:
   *  MATLAB Function: '<S1>/DispCore'
   */
  (void)Rte_Write_DispTxByte_DispTxByte(DisplayDriver_ARID_DEF.outByte);

  /* Outport: '<Root>/DispTxRequest' incorporates:
   *  MATLAB Function: '<S1>/DispCore'
   */
  (void)Rte_Write_DispTxRequest_DispTxRequest(DisplayDriver_ARID_DEF.req);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/DispTask10ms' */
}

/* Model initialize function */
void DisplayDriver_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/DispTask10ms' incorporates:
   *  SubSystem: '<Root>/DisplayDriver_task10ms'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/DispCore' */
  DisplayDriver_ARID_DEF.shown = ((uint16)(65535U));
  DisplayDriver_ARID_DEF.needClear = TRUE;

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/DispTask10ms' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
