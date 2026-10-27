/*
 * File: BattSimMode.c
 *
 * Code generated for Simulink model 'BattSimMode'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BattSimMode.h"

/* Model step function for TID1 */
void ModeTask10ms(void)                /* Explicit Task: ModeTask10ms */
{
  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* Outport: '<Root>/BattSimStatus' incorporates:
   *  SignalConversion generated from: '<S1>/BattSimStatus'
   */
  (void)Rte_Write_BattSimStatus_BattSimStatus(0U);

  /* Outport: '<Root>/OutputStatus' incorporates:
   *  SignalConversion generated from: '<S1>/OutputStatus'
   */
  (void)Rte_Write_OutputStatus_OutputStatus(0U);

  /* Outport: '<Root>/LedRemote' incorporates:
   *  SignalConversion generated from: '<S1>/LedRemote'
   */
  (void)Rte_Write_LedRemote_LedRemote(FALSE);

  /* Outport: '<Root>/OutputEnable' incorporates:
   *  SignalConversion generated from: '<S1>/OutputEnable'
   */
  (void)Rte_Write_OutputEnable_OutputEnable(FALSE);

  /* Outport: '<Root>/AliveStatusBattSim' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusBattSim'
   */
  (void)Rte_Write_AliveStatusBattSim_AliveStatusBattSim(0U);

  /* Outport: '<Root>/AliveStatusCells010203' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells010203'
   */
  (void)Rte_Write_AliveStatusCells010203_AliveStatusCells010203(0U);

  /* Outport: '<Root>/AliveStatusCells040506' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells040506'
   */
  (void)Rte_Write_AliveStatusCells040506_AliveStatusCells040506(0U);

  /* Outport: '<Root>/AliveStatusCells070809' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells070809'
   */
  (void)Rte_Write_AliveStatusCells070809_AliveStatusCells070809(0U);

  /* Outport: '<Root>/AliveStatusCells101112' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells101112'
   */
  (void)Rte_Write_AliveStatusCells101112_AliveStatusCells101112(0U);

  /* Outport: '<Root>/AliveStatusCells131415' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells131415'
   */
  (void)Rte_Write_AliveStatusCells131415_AliveStatusCells131415(0U);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */
}

/* Model initialize function */
void BattSimMode_Init(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
