/*
 * File: CellChannels.h
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

#ifndef CellChannels_h_
#define CellChannels_h_
#ifndef CellChannels_COMMON_INCLUDES_
#define CellChannels_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_CellChannels.h"
#endif                                 /* CellChannels_COMMON_INCLUDES_ */

#include "CellChannels_types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  float32 ActualVolt_V[Rte_SysCon_CellCount];/* '<Root>/ActualVolt_V' */
  float32 y[14];                       /* '<S1>/CellCore' */
  float32 integ[14];                   /* '<S1>/CellCore' */
  float32 corrHold[14];                /* '<S1>/CellCore' */
  float32 prevV[14];                   /* '<S1>/CellCore' */
  uint8 ovCnt[14];                     /* '<S1>/CellCore' */
  uint8 holdCnt[14];                   /* '<S1>/CellCore' */
  boolean seen[14];                    /* '<S1>/CellCore' */
} ARID_DEF_CellChannels_T;

/* Invariant block signals (default storage) */
typedef struct {
  const uint8 Cell15State_Const;       /* '<S1>/Cell15State_Const' */
  const uint8 Cell16State_Const;       /* '<S1>/Cell16State_Const' */
} ConstB_CellChannels_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_CellChannels_T CellChannels_ARID_DEF;/* '<Root>/ActualVolt_V' */
extern const ConstB_CellChannels_T CellChannels_ConstB;/* constant block i/o */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'CellChannels'
 * '<S1>'   : 'CellChannels/CellChannels_task10ms'
 * '<S2>'   : 'CellChannels/CellChannels_task10ms/CellCore'
 */
#endif                                 /* CellChannels_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
