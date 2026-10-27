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

/* Model step function for TID1 */
void CellTask10ms(void)                /* Explicit Task: CellTask10ms */
{
  /* RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' incorporates:
   *  SubSystem: '<Root>/CellChannels_task10ms'
   */
  /* Outport: '<Root>/En01' incorporates:
   *  SignalConversion generated from: '<S1>/En01'
   */
  (void)Rte_Write_En01_En01(FALSE);

  /* Outport: '<Root>/Sel01' incorporates:
   *  SignalConversion generated from: '<S1>/Sel01'
   */
  (void)Rte_Write_Sel01_Sel01(FALSE);

  /* Outport: '<Root>/Cell01VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell01VoltAct_V'
   */
  (void)Rte_Write_Cell01VoltAct_V_Cell01VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell01State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell01State'
   */
  (void)Rte_Write_Cell01State_Cell01State(0U);

  /* Outport: '<Root>/Duty01_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty01_permille'
   */
  (void)Rte_Write_Duty01_permille_Duty01_permille(0U);

  /* Outport: '<Root>/En02' incorporates:
   *  SignalConversion generated from: '<S1>/En02'
   */
  (void)Rte_Write_En02_En02(FALSE);

  /* Outport: '<Root>/Sel02' incorporates:
   *  SignalConversion generated from: '<S1>/Sel02'
   */
  (void)Rte_Write_Sel02_Sel02(FALSE);

  /* Outport: '<Root>/Cell02VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell02VoltAct_V'
   */
  (void)Rte_Write_Cell02VoltAct_V_Cell02VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell02State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell02State'
   */
  (void)Rte_Write_Cell02State_Cell02State(0U);

  /* Outport: '<Root>/Duty02_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty02_permille'
   */
  (void)Rte_Write_Duty02_permille_Duty02_permille(0U);

  /* Outport: '<Root>/En03' incorporates:
   *  SignalConversion generated from: '<S1>/En03'
   */
  (void)Rte_Write_En03_En03(FALSE);

  /* Outport: '<Root>/Sel03' incorporates:
   *  SignalConversion generated from: '<S1>/Sel03'
   */
  (void)Rte_Write_Sel03_Sel03(FALSE);

  /* Outport: '<Root>/Cell03VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell03VoltAct_V'
   */
  (void)Rte_Write_Cell03VoltAct_V_Cell03VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell03State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell03State'
   */
  (void)Rte_Write_Cell03State_Cell03State(0U);

  /* Outport: '<Root>/Duty03_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty03_permille'
   */
  (void)Rte_Write_Duty03_permille_Duty03_permille(0U);

  /* Outport: '<Root>/En04' incorporates:
   *  SignalConversion generated from: '<S1>/En04'
   */
  (void)Rte_Write_En04_En04(FALSE);

  /* Outport: '<Root>/Sel04' incorporates:
   *  SignalConversion generated from: '<S1>/Sel04'
   */
  (void)Rte_Write_Sel04_Sel04(FALSE);

  /* Outport: '<Root>/Cell04VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell04VoltAct_V'
   */
  (void)Rte_Write_Cell04VoltAct_V_Cell04VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell04State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell04State'
   */
  (void)Rte_Write_Cell04State_Cell04State(0U);

  /* Outport: '<Root>/Duty04_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty04_permille'
   */
  (void)Rte_Write_Duty04_permille_Duty04_permille(0U);

  /* Outport: '<Root>/En05' incorporates:
   *  SignalConversion generated from: '<S1>/En05'
   */
  (void)Rte_Write_En05_En05(FALSE);

  /* Outport: '<Root>/Sel05' incorporates:
   *  SignalConversion generated from: '<S1>/Sel05'
   */
  (void)Rte_Write_Sel05_Sel05(FALSE);

  /* Outport: '<Root>/Cell05VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell05VoltAct_V'
   */
  (void)Rte_Write_Cell05VoltAct_V_Cell05VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell05State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell05State'
   */
  (void)Rte_Write_Cell05State_Cell05State(0U);

  /* Outport: '<Root>/Duty05_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty05_permille'
   */
  (void)Rte_Write_Duty05_permille_Duty05_permille(0U);

  /* Outport: '<Root>/En06' incorporates:
   *  SignalConversion generated from: '<S1>/En06'
   */
  (void)Rte_Write_En06_En06(FALSE);

  /* Outport: '<Root>/Sel06' incorporates:
   *  SignalConversion generated from: '<S1>/Sel06'
   */
  (void)Rte_Write_Sel06_Sel06(FALSE);

  /* Outport: '<Root>/Cell06VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell06VoltAct_V'
   */
  (void)Rte_Write_Cell06VoltAct_V_Cell06VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell06State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell06State'
   */
  (void)Rte_Write_Cell06State_Cell06State(0U);

  /* Outport: '<Root>/Duty06_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty06_permille'
   */
  (void)Rte_Write_Duty06_permille_Duty06_permille(0U);

  /* Outport: '<Root>/En07' incorporates:
   *  SignalConversion generated from: '<S1>/En07'
   */
  (void)Rte_Write_En07_En07(FALSE);

  /* Outport: '<Root>/Sel07' incorporates:
   *  SignalConversion generated from: '<S1>/Sel07'
   */
  (void)Rte_Write_Sel07_Sel07(FALSE);

  /* Outport: '<Root>/Cell07VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell07VoltAct_V'
   */
  (void)Rte_Write_Cell07VoltAct_V_Cell07VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell07State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell07State'
   */
  (void)Rte_Write_Cell07State_Cell07State(0U);

  /* Outport: '<Root>/Duty07_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty07_permille'
   */
  (void)Rte_Write_Duty07_permille_Duty07_permille(0U);

  /* Outport: '<Root>/En08' incorporates:
   *  SignalConversion generated from: '<S1>/En08'
   */
  (void)Rte_Write_En08_En08(FALSE);

  /* Outport: '<Root>/Sel08' incorporates:
   *  SignalConversion generated from: '<S1>/Sel08'
   */
  (void)Rte_Write_Sel08_Sel08(FALSE);

  /* Outport: '<Root>/Cell08VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell08VoltAct_V'
   */
  (void)Rte_Write_Cell08VoltAct_V_Cell08VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell08State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell08State'
   */
  (void)Rte_Write_Cell08State_Cell08State(0U);

  /* Outport: '<Root>/Duty08_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty08_permille'
   */
  (void)Rte_Write_Duty08_permille_Duty08_permille(0U);

  /* Outport: '<Root>/En09' incorporates:
   *  SignalConversion generated from: '<S1>/En09'
   */
  (void)Rte_Write_En09_En09(FALSE);

  /* Outport: '<Root>/Sel09' incorporates:
   *  SignalConversion generated from: '<S1>/Sel09'
   */
  (void)Rte_Write_Sel09_Sel09(FALSE);

  /* Outport: '<Root>/Cell09VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell09VoltAct_V'
   */
  (void)Rte_Write_Cell09VoltAct_V_Cell09VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell09State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell09State'
   */
  (void)Rte_Write_Cell09State_Cell09State(0U);

  /* Outport: '<Root>/Duty09_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty09_permille'
   */
  (void)Rte_Write_Duty09_permille_Duty09_permille(0U);

  /* Outport: '<Root>/En10' incorporates:
   *  SignalConversion generated from: '<S1>/En10'
   */
  (void)Rte_Write_En10_En10(FALSE);

  /* Outport: '<Root>/Sel10' incorporates:
   *  SignalConversion generated from: '<S1>/Sel10'
   */
  (void)Rte_Write_Sel10_Sel10(FALSE);

  /* Outport: '<Root>/Cell10VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell10VoltAct_V'
   */
  (void)Rte_Write_Cell10VoltAct_V_Cell10VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell10State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell10State'
   */
  (void)Rte_Write_Cell10State_Cell10State(0U);

  /* Outport: '<Root>/Duty10_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty10_permille'
   */
  (void)Rte_Write_Duty10_permille_Duty10_permille(0U);

  /* Outport: '<Root>/En11' incorporates:
   *  SignalConversion generated from: '<S1>/En11'
   */
  (void)Rte_Write_En11_En11(FALSE);

  /* Outport: '<Root>/Sel11' incorporates:
   *  SignalConversion generated from: '<S1>/Sel11'
   */
  (void)Rte_Write_Sel11_Sel11(FALSE);

  /* Outport: '<Root>/Cell11VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell11VoltAct_V'
   */
  (void)Rte_Write_Cell11VoltAct_V_Cell11VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell11State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell11State'
   */
  (void)Rte_Write_Cell11State_Cell11State(0U);

  /* Outport: '<Root>/Duty11_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty11_permille'
   */
  (void)Rte_Write_Duty11_permille_Duty11_permille(0U);

  /* Outport: '<Root>/En12' incorporates:
   *  SignalConversion generated from: '<S1>/En12'
   */
  (void)Rte_Write_En12_En12(FALSE);

  /* Outport: '<Root>/Sel12' incorporates:
   *  SignalConversion generated from: '<S1>/Sel12'
   */
  (void)Rte_Write_Sel12_Sel12(FALSE);

  /* Outport: '<Root>/Cell12VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell12VoltAct_V'
   */
  (void)Rte_Write_Cell12VoltAct_V_Cell12VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell12State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell12State'
   */
  (void)Rte_Write_Cell12State_Cell12State(0U);

  /* Outport: '<Root>/Duty12_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty12_permille'
   */
  (void)Rte_Write_Duty12_permille_Duty12_permille(0U);

  /* Outport: '<Root>/En13' incorporates:
   *  SignalConversion generated from: '<S1>/En13'
   */
  (void)Rte_Write_En13_En13(FALSE);

  /* Outport: '<Root>/Sel13' incorporates:
   *  SignalConversion generated from: '<S1>/Sel13'
   */
  (void)Rte_Write_Sel13_Sel13(FALSE);

  /* Outport: '<Root>/Cell13VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell13VoltAct_V'
   */
  (void)Rte_Write_Cell13VoltAct_V_Cell13VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell13State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell13State'
   */
  (void)Rte_Write_Cell13State_Cell13State(0U);

  /* Outport: '<Root>/Duty13_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty13_permille'
   */
  (void)Rte_Write_Duty13_permille_Duty13_permille(0U);

  /* Outport: '<Root>/En14' incorporates:
   *  SignalConversion generated from: '<S1>/En14'
   */
  (void)Rte_Write_En14_En14(FALSE);

  /* Outport: '<Root>/Sel14' incorporates:
   *  SignalConversion generated from: '<S1>/Sel14'
   */
  (void)Rte_Write_Sel14_Sel14(FALSE);

  /* Outport: '<Root>/Cell14VoltAct_V' incorporates:
   *  SignalConversion generated from: '<S1>/Cell14VoltAct_V'
   */
  (void)Rte_Write_Cell14VoltAct_V_Cell14VoltAct_V(0.0F);

  /* Outport: '<Root>/Cell14State' incorporates:
   *  SignalConversion generated from: '<S1>/Cell14State'
   */
  (void)Rte_Write_Cell14State_Cell14State(0U);

  /* Outport: '<Root>/Duty14_permille' incorporates:
   *  SignalConversion generated from: '<S1>/Duty14_permille'
   */
  (void)Rte_Write_Duty14_permille_Duty14_permille(0U);

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/CellTask10ms' */
}

/* Model initialize function */
void CellChannels_Init(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
