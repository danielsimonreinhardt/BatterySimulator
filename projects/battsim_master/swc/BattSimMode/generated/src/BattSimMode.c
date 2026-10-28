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
#include "Platform_Types.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_BattSimMode_T BattSimMode_ARID_DEF;/* '<S1>/ModeCore' */

/* Model step function for TID1 */
void ModeTask10ms(void)                /* Explicit Task: ModeTask10ms */
{
  sint32 i;
  uint8 rtb_TmpSignalConversionAtSFunctionInport4[9];
  uint8 alive;
  uint8 m;
  uint8 o;
  uint8 r;
  uint8 tmpRead;
  boolean qlfOk[9];
  boolean tmpRead_0;
  boolean tmpRead_1;
  boolean tmpRead_2;
  boolean valid;

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/ControlDynCellSim1Qlf'
   *  Inport: '<Root>/ControlDynCellSim2Qlf'
   */
  (void)Rte_Read_ControlDynCellSim2Qlf_ControlDynCellSim2Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[8]);
  (void)Rte_Read_ControlDynCellSim1Qlf_ControlDynCellSim1Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[7]);

  /* Inport: '<Root>/ControlBattSimQlf' */
  (void)Rte_Read_ControlBattSimQlf_ControlBattSimQlf(&alive);

  /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
   *  Inport: '<Root>/ControlCell010203Qlf'
   *  Inport: '<Root>/ControlCell040506Qlf'
   *  Inport: '<Root>/ControlCell070809Qlf'
   *  Inport: '<Root>/ControlCell101112Qlf'
   *  Inport: '<Root>/ControlCell131415Qlf'
   *  Inport: '<Root>/ControlCell16Qlf'
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Read_ControlCell16Qlf_ControlCell16Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[5]);
  (void)Rte_Read_ControlCell131415Qlf_ControlCell131415Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[4]);
  (void)Rte_Read_ControlCell101112Qlf_ControlCell101112Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[3]);
  (void)Rte_Read_ControlCell070809Qlf_ControlCell070809Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[2]);
  (void)Rte_Read_ControlCell040506Qlf_ControlCell040506Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[1]);
  (void)Rte_Read_ControlCell010203Qlf_ControlCell010203Qlf
    (&rtb_TmpSignalConversionAtSFunctionInport4[0]);

  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  rtb_TmpSignalConversionAtSFunctionInport4[6] = alive;

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */

  /* Inport: '<Root>/ResetQlf' */
  (void)Rte_Read_ResetQlf_ResetQlf(&r);

  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* MATLAB Function: '<S1>/ModeCore' incorporates:
   *  Constant: '<S1>/QLF_OK_Const'
   *  Inport: '<Root>/ResetWatchdog'
   */
  if ((!BattSimMode_ARID_DEF.wdDone) && (r == 1)) {
    BattSimMode_ARID_DEF.wdDone = TRUE;
    (void)Rte_Read_ResetWatchdog_ResetWatchdog(&tmpRead_2);
    if (tmpRead_2 && (BattSimMode_ARID_DEF.st == 0) &&
        (BattSimMode_ARID_DEF.reason == 0)) {
      BattSimMode_ARID_DEF.reason = 4U;
    }
  }

  tmpRead_2 = FALSE;

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */

  /* Inport: '<Root>/BtnAllOnOff' */
  (void)Rte_Read_BtnAllOnOff_BtnAllOnOff(&tmpRead_0);

  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* MATLAB Function: '<S1>/ModeCore' incorporates:
   *  Constant: '<S1>/ButtonDebounce_cycles_Const'
   */
  if (tmpRead_0 != BattSimMode_ARID_DEF.levA) {
    i = (sint32)(BattSimMode_ARID_DEF.cntA + 1U);
    if (BattSimMode_ARID_DEF.cntA + 1U > 255U) {
      i = 255;
    }

    BattSimMode_ARID_DEF.cntA = (uint8)i;
    if ((uint8)i >= Rte_Prm_CalPrm_ButtonDebounce_cycles()) {
      BattSimMode_ARID_DEF.levA = tmpRead_0;
      BattSimMode_ARID_DEF.cntA = 0U;
      tmpRead_2 = tmpRead_0;
    }
  } else {
    BattSimMode_ARID_DEF.cntA = 0U;
  }

  tmpRead_0 = FALSE;

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */

  /* Inport: '<Root>/BtnRemote' */
  (void)Rte_Read_BtnRemote_BtnRemote(&valid);

  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* MATLAB Function: '<S1>/ModeCore' incorporates:
   *  Constant: '<S1>/ButtonDebounce_cycles_Const'
   *  Constant: '<S1>/QLF_OK_Const'
   *  Inport: '<Root>/ModeControl'
   *  Inport: '<Root>/OutputControl'
   *  Inport: '<Root>/RemoteControl'
   *  SignalConversion generated from: '<S2>/ SFunction '
   */
  if (valid != BattSimMode_ARID_DEF.levR) {
    i = (sint32)(BattSimMode_ARID_DEF.cntR + 1U);
    if (BattSimMode_ARID_DEF.cntR + 1U > 255U) {
      i = 255;
    }

    BattSimMode_ARID_DEF.cntR = (uint8)i;
    if ((uint8)i >= Rte_Prm_CalPrm_ButtonDebounce_cycles()) {
      BattSimMode_ARID_DEF.levR = valid;
      BattSimMode_ARID_DEF.cntR = 0U;
      tmpRead_0 = valid;
    }
  } else {
    BattSimMode_ARID_DEF.cntR = 0U;
  }

  valid = (alive == 1);
  r = 3U;
  m = 7U;
  o = 3U;
  if (valid) {
    (void)Rte_Read_OutputControl_OutputControl(&tmpRead);
    (void)Rte_Read_ModeControl_ModeControl(&m);
    (void)Rte_Read_RemoteControl_RemoteControl(&r);
    o = tmpRead;
    BattSimMode_ARID_DEF.outBlock = ((tmpRead == 2) &&
      BattSimMode_ARID_DEF.outBlock);
  }

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */

  /* Inport: '<Root>/Overload' */
  (void)Rte_Read_Overload_Overload(&tmpRead_1);

  /* RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* MATLAB Function: '<S1>/ModeCore' */
  if (tmpRead_1 && (BattSimMode_ARID_DEF.st != 3)) {
    BattSimMode_ARID_DEF.st = 3U;
    BattSimMode_ARID_DEF.reason = 3U;
  } else {
    valid = ((BattSimMode_ARID_DEF.st == 2) && (!valid));
    if (valid && ((alive & 24U) != 0U)) {
      BattSimMode_ARID_DEF.st = 3U;
      BattSimMode_ARID_DEF.reason = 1U;
    } else if (valid && ((alive & 2U) != 0U)) {
      BattSimMode_ARID_DEF.st = 3U;
      BattSimMode_ARID_DEF.reason = 2U;
    } else {
      switch (BattSimMode_ARID_DEF.st) {
       case 0U:
        if (tmpRead_0 || ((r == 2) && (m == 2))) {
          BattSimMode_ARID_DEF.st = 1U;
          if (BattSimMode_ARID_DEF.reason == 4) {
            BattSimMode_ARID_DEF.reason = 0U;
          }
        }
        break;

       case 1U:
        if (tmpRead_0 || ((r == 1) || ((r == 2) && (m == 1)))) {
          BattSimMode_ARID_DEF.st = 0U;
        } else if ((o == 2) && (!BattSimMode_ARID_DEF.outBlock)) {
          BattSimMode_ARID_DEF.st = 2U;
        }
        break;

       case 2U:
        if (tmpRead_0 || ((r == 1) || ((r == 2) && (m == 1)))) {
          BattSimMode_ARID_DEF.st = 0U;
        } else if (tmpRead_2) {
          BattSimMode_ARID_DEF.st = 1U;
          BattSimMode_ARID_DEF.outBlock = TRUE;
        } else if (o == 1) {
          BattSimMode_ARID_DEF.st = 1U;
        }
        break;

       default:
        if ((!tmpRead_1) && (((r == 2) && (m == 1)) || tmpRead_2)) {
          BattSimMode_ARID_DEF.st = 0U;
          BattSimMode_ARID_DEF.reason = 0U;
        }
        break;
      }
    }
  }

  /* Outport: '<Root>/BattSimStatus' incorporates:
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Write_BattSimStatus_BattSimStatus(1U);

  /* MATLAB Function: '<S1>/ModeCore' */
  valid = ((BattSimMode_ARID_DEF.st == 1) || (BattSimMode_ARID_DEF.st == 2));
  if (valid) {
    /* Outport: '<Root>/BattSimStatus' */
    (void)Rte_Write_BattSimStatus_BattSimStatus(3U);
  } else if (BattSimMode_ARID_DEF.st == 3) {
    /* Outport: '<Root>/BattSimStatus' */
    (void)Rte_Write_BattSimStatus_BattSimStatus(5U);
  }

  /* Outport: '<Root>/OutputStatus' incorporates:
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Write_OutputStatus_OutputStatus(1U);

  /* MATLAB Function: '<S1>/ModeCore' */
  tmpRead_1 = (BattSimMode_ARID_DEF.st == 2);
  if (tmpRead_1) {
    /* Outport: '<Root>/OutputStatus' */
    (void)Rte_Write_OutputStatus_OutputStatus(2U);
  }

  /* Outport: '<Root>/LedRemote' incorporates:
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Write_LedRemote_LedRemote(valid || ((BattSimMode_ARID_DEF.st == 3) &&
    (BattSimMode_ARID_DEF.tick < 20)));

  /* Outport: '<Root>/FailReason' incorporates:
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Write_FailReason_FailReason(BattSimMode_ARID_DEF.reason);

  /* MATLAB Function: '<S1>/ModeCore' incorporates:
   *  Constant: '<S1>/QLF_OK_Const'
   */
  alive = BattSimMode_ARID_DEF.aliveCnt;
  for (i = 0; i < 9; i++) {
    qlfOk[i] = (rtb_TmpSignalConversionAtSFunctionInport4[i] == 1);
  }

  if ((uint8)(BattSimMode_ARID_DEF.tick - (uint32)((sint32)
        (BattSimMode_ARID_DEF.tick / 10U) * 10)) == 9) {
    if (BattSimMode_ARID_DEF.aliveCnt >= 14) {
      BattSimMode_ARID_DEF.aliveCnt = 1U;
    } else {
      BattSimMode_ARID_DEF.aliveCnt++;
    }
  }

  if (BattSimMode_ARID_DEF.tick >= 39) {
    BattSimMode_ARID_DEF.tick = 0U;
  } else {
    BattSimMode_ARID_DEF.tick++;
  }

  /* Outport: '<Root>/OutputEnable' incorporates:
   *  MATLAB Function: '<S1>/ModeCore'
   */
  (void)Rte_Write_OutputEnable_OutputEnable(tmpRead_1);

  /* Outport: '<Root>/AliveStatusBattSimExt' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusBattSimExt'
   */
  (void)Rte_Write_AliveStatusBattSimExt_AliveStatusBattSimExt(alive);

  /* Outport: '<Root>/AliveStatusCell16' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCell16'
   */
  (void)Rte_Write_AliveStatusCell16_AliveStatusCell16(alive);

  /* Outport: '<Root>/AliveStatusCells010203' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells010203'
   */
  (void)Rte_Write_AliveStatusCells010203_AliveStatusCells010203(alive);

  /* Outport: '<Root>/AliveStatusCells040506' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells040506'
   */
  (void)Rte_Write_AliveStatusCells040506_AliveStatusCells040506(alive);

  /* Outport: '<Root>/AliveStatusCells070809' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells070809'
   */
  (void)Rte_Write_AliveStatusCells070809_AliveStatusCells070809(alive);

  /* Outport: '<Root>/AliveStatusCells101112' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells101112'
   */
  (void)Rte_Write_AliveStatusCells101112_AliveStatusCells101112(alive);

  /* Outport: '<Root>/AliveStatusCells131415' incorporates:
   *  SignalConversion generated from: '<S1>/AliveStatusCells131415'
   */
  (void)Rte_Write_AliveStatusCells131415_AliveStatusCells131415(alive);

  /* Outport: '<Root>/ValidBattSim' incorporates:
   *  SignalConversion generated from: '<S1>/ValidBattSim'
   */
  (void)Rte_Write_ValidBattSim_ValidBattSim(qlfOk[6]);

  /* Outport: '<Root>/ValidCell040506' incorporates:
   *  SignalConversion generated from: '<S1>/ValidCell040506'
   */
  (void)Rte_Write_ValidCell040506_ValidCell040506(qlfOk[1]);

  /* Outport: '<Root>/ValidCell070809' incorporates:
   *  SignalConversion generated from: '<S1>/ValidCell070809'
   */
  (void)Rte_Write_ValidCell070809_ValidCell070809(qlfOk[2]);

  /* Outport: '<Root>/ValidCell101112' incorporates:
   *  SignalConversion generated from: '<S1>/ValidCell101112'
   */
  (void)Rte_Write_ValidCell101112_ValidCell101112(qlfOk[3]);

  /* Outport: '<Root>/ValidCell131415' incorporates:
   *  SignalConversion generated from: '<S1>/ValidCell131415'
   */
  (void)Rte_Write_ValidCell131415_ValidCell131415(qlfOk[4]);

  /* Outport: '<Root>/ValidCell16' incorporates:
   *  SignalConversion generated from: '<S1>/ValidCell16'
   */
  (void)Rte_Write_ValidCell16_ValidCell16(qlfOk[5]);

  /* Outport: '<Root>/ValidDynCellSim1' incorporates:
   *  SignalConversion generated from: '<S1>/ValidDynCellSim1'
   */
  (void)Rte_Write_ValidDynCellSim1_ValidDynCellSim1(qlfOk[7]);

  /* Outport: '<Root>/ValidDynCellSim2' incorporates:
   *  SignalConversion generated from: '<S1>/ValidDynCellSim2'
   */
  (void)Rte_Write_ValidDynCellSim2_ValidDynCellSim2(qlfOk[8]);

  /* Outport: '<Root>/FwVersionMajor' incorporates:
   *  Constant: '<S1>/VersionMajor_Const'
   *  SignalConversion generated from: '<S1>/FwVersionMajor'
   */
  (void)Rte_Write_FwVersionMajor_FwVersionMajor(Rte_Prm_CalPrm_VersionMajor());

  /* Outport: '<Root>/FwVersionMinor' incorporates:
   *  Constant: '<S1>/VersionMinor_Const'
   *  SignalConversion generated from: '<S1>/FwVersionMinor'
   */
  (void)Rte_Write_FwVersionMinor_FwVersionMinor(Rte_Prm_CalPrm_VersionMinor());

  /* Outport: '<Root>/FwVersionPatch' incorporates:
   *  Constant: '<S1>/VersionPatch_Const'
   *  SignalConversion generated from: '<S1>/FwVersionPatch'
   */
  (void)Rte_Write_FwVersionPatch_FwVersionPatch(Rte_Prm_CalPrm_VersionPatch());

  /* End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */

  /* Outport: '<Root>/AliveStatusBattSim' */
  (void)Rte_Write_AliveStatusBattSim_AliveStatusBattSim(alive);

  /* Outport: '<Root>/ValidCell010203' */
  (void)Rte_Write_ValidCell010203_ValidCell010203(qlfOk[0]);
}

/* Model initialize function */
void BattSimMode_Init(void)
{
  /* SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' incorporates:
   *  SubSystem: '<Root>/BattSimMode_task10ms'
   */
  /* SystemInitialize for MATLAB Function: '<S1>/ModeCore' */
  BattSimMode_ARID_DEF.aliveCnt = 1U;

  /* SystemInitialize for Outport: '<Root>/FwVersionMajor' incorporates:
   *  Constant: '<S1>/VersionMajor_Const'
   *  SignalConversion generated from: '<S1>/FwVersionMajor'
   */
  (void)Rte_Write_FwVersionMajor_FwVersionMajor(Rte_Prm_CalPrm_VersionMajor());

  /* SystemInitialize for Outport: '<Root>/FwVersionMinor' incorporates:
   *  Constant: '<S1>/VersionMinor_Const'
   *  SignalConversion generated from: '<S1>/FwVersionMinor'
   */
  (void)Rte_Write_FwVersionMinor_FwVersionMinor(Rte_Prm_CalPrm_VersionMinor());

  /* SystemInitialize for Outport: '<Root>/FwVersionPatch' incorporates:
   *  Constant: '<S1>/VersionPatch_Const'
   *  SignalConversion generated from: '<S1>/FwVersionPatch'
   */
  (void)Rte_Write_FwVersionPatch_FwVersionPatch(Rte_Prm_CalPrm_VersionPatch());

  /* End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/ModeTask10ms' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
