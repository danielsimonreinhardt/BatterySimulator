/*
 * File: SupplyMonitor.c
 *
 * Code generated for Simulink model 'SupplyMonitor'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "SupplyMonitor.h"
#include "Platform_Types.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_SupplyMonitor_T SupplyMonitor_ARID_DEF;/* '<S1>/SupplyCore' */

/* Model step function for TID1 */
void SupplyTask10ms(void)              /* Explicit Task: SupplyTask10ms */
{
  float32 sample;
  uint32 tmpRead;
  uint8 tmpRead_0;
  boolean tmpRead_1;
  boolean tmpRead_2;

  /* Inport: '<Root>/ChannelsReleased' */
  (void)Rte_Read_ChannelsReleased_ChannelsReleased(&tmpRead_2);

  /* Inport: '<Root>/Limit' */
  (void)Rte_Read_Limit_Limit(&tmpRead_1);

  /* RootInportFunctionCallGenerator generated from: '<Root>/SupplyTask10ms' incorporates:
   *  SubSystem: '<Root>/SupplyMonitor_task10ms'
   */
  /* Outport: '<Root>/LimitActive' incorporates:
   *  MATLAB Function: '<S1>/SupplyCore'
   */
  (void)Rte_Write_LimitActive_LimitActive(tmpRead_1);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/SupplyTask10ms' */

  /* Inport: '<Root>/ISupplyQlf' */
  (void)Rte_Read_ISupplyQlf_ISupplyQlf(&tmpRead_0);

  /* RootInportFunctionCallGenerator generated from: '<Root>/SupplyTask10ms' incorporates:
   *  SubSystem: '<Root>/SupplyMonitor_task10ms'
   */
  /* MATLAB Function: '<S1>/SupplyCore' incorporates:
   *  Constant: '<S1>/MinCurrent_mA_Const'
   *  Constant: '<S1>/OverloadHyst_mA_Const'
   *  Constant: '<S1>/OverloadThd_mA_Const'
   *  Constant: '<S1>/QLF_OK_Const'
   *  Constant: '<S1>/SupplyCurrentFilter_Const'
   *  Constant: '<S1>/SupplyCurrentGain_Const'
   *  Inport: '<Root>/ISupply_mV'
   */
  if (tmpRead_0 == 1) {
    (void)Rte_Read_ISupply_mV_ISupply_mV(&tmpRead);
    sample = (float32)tmpRead * Rte_Prm_CalPrm_SupplyCurrentGain();
    SupplyMonitor_ARID_DEF.y += (sample - SupplyMonitor_ARID_DEF.y) *
      Rte_Prm_CalPrm_SupplyCurrentFilter();
    SupplyMonitor_ARID_DEF.bad = 0U;
    if (tmpRead_2 && (sample < Rte_Prm_CalPrm_MinCurrent_mA())) {
      if (SupplyMonitor_ARID_DEF.low < 255) {
        SupplyMonitor_ARID_DEF.low++;
      }
    } else {
      SupplyMonitor_ARID_DEF.low = 0U;
    }
  } else if (SupplyMonitor_ARID_DEF.bad < 255) {
    SupplyMonitor_ARID_DEF.bad++;
  }

  if (!tmpRead_2) {
    SupplyMonitor_ARID_DEF.low = 0U;
  }

  SupplyMonitor_ARID_DEF.ovl = ((SupplyMonitor_ARID_DEF.y >
    Rte_Prm_CalPrm_OverloadThd_mA()) || ((SupplyMonitor_ARID_DEF.y >=
    Rte_Prm_CalPrm_OverloadThd_mA() - Rte_Prm_CalPrm_OverloadHyst_mA()) &&
    SupplyMonitor_ARID_DEF.ovl));

  /* Outport: '<Root>/Overload' incorporates:
   *  MATLAB Function: '<S1>/SupplyCore'
   */
  (void)Rte_Write_Overload_Overload(SupplyMonitor_ARID_DEF.ovl);

  /* MATLAB Function: '<S1>/SupplyCore' */
  sample = SupplyMonitor_ARID_DEF.y * 0.001F;
  if (sample > 65.535F) {
    sample = 65.535F;
  }

  /* Outport: '<Root>/SupplyCurrent_A' incorporates:
   *  MATLAB Function: '<S1>/SupplyCore'
   */
  (void)Rte_Write_SupplyCurrent_A_SupplyCurrent_A(sample);

  /* Outport: '<Root>/CurrentInvalid' incorporates:
   *  Constant: '<S1>/CurrentInvalid_cycles_Const'
   *  Constant: '<S1>/MinCurrentTime_cycles_Const'
   *  MATLAB Function: '<S1>/SupplyCore'
   */
  (void)Rte_Write_CurrentInvalid_CurrentInvalid((SupplyMonitor_ARID_DEF.bad >=
    Rte_Prm_CalPrm_CurrentInvalid_cycles()) || (SupplyMonitor_ARID_DEF.low >=
    Rte_Prm_CalPrm_MinCurrentTime_cycles()));

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/SupplyTask10ms' */
}

/* Model initialize function */
void SupplyMonitor_Init(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
