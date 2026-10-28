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
  float32 c;
  uint32 tmpRead;
  uint8 tmpRead_0;
  boolean tmpRead_1;

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
   *  Constant: '<S1>/OverloadHyst_mA_Const'
   *  Constant: '<S1>/OverloadThd_mA_Const'
   *  Constant: '<S1>/QLF_OK_Const'
   *  Constant: '<S1>/SupplyCurrentFilter_Const'
   *  Constant: '<S1>/SupplyCurrentGain_Const'
   *  Inport: '<Root>/ISupply_mV'
   */
  if (tmpRead_0 == 1) {
    (void)Rte_Read_ISupply_mV_ISupply_mV(&tmpRead);
    SupplyMonitor_ARID_DEF.y += ((float32)tmpRead *
      Rte_Prm_CalPrm_SupplyCurrentGain() - SupplyMonitor_ARID_DEF.y) *
      Rte_Prm_CalPrm_SupplyCurrentFilter();
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
  c = SupplyMonitor_ARID_DEF.y * 0.001F;
  if (c > 65.535F) {
    c = 65.535F;
  }

  /* Outport: '<Root>/SupplyCurrent_A' incorporates:
   *  MATLAB Function: '<S1>/SupplyCore'
   */
  (void)Rte_Write_SupplyCurrent_A_SupplyCurrent_A(c);

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
