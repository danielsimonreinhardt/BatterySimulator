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

/* Model step function for TID1 */
void SupplyTask10ms(void)              /* Explicit Task: SupplyTask10ms */
{
  /* RootInportFunctionCallGenerator generated from: '<Root>/SupplyTask10ms' incorporates:
   *  SubSystem: '<Root>/SupplyMonitor_task10ms'
   */
  /* Outport: '<Root>/Overload' incorporates:
   *  SignalConversion generated from: '<S1>/Overload'
   */
  (void)Rte_Write_Overload_Overload(FALSE);

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
