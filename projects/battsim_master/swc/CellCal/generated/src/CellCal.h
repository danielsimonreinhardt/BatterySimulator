/*
 * File: CellCal.h
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

#ifndef CellCal_h_
#define CellCal_h_
#ifndef CellCal_COMMON_INCLUDES_
#define CellCal_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_CellCal.h"
#endif                                 /* CellCal_COMMON_INCLUDES_ */

#include "CellCal_types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  float32 CalSetOffset_V[Rte_SysCon_CellCount];/* '<Root>/CalSetOffset_V' */
  float32 CalSetGain[Rte_SysCon_CellCount];/* '<Root>/CalSetGain' */
  float32 CalMeasOffset_V[Rte_SysCon_CellCount];/* '<Root>/CalMeasOffset_V' */
  float32 CalMeasGain[Rte_SysCon_CellCount];/* '<Root>/CalMeasGain' */
  sint16 NvSetOffsetOut[Rte_SysCon_CellCount];/* '<Root>/NvSetOffsetOut' */
  sint16 NvMeasOffsetOut[Rte_SysCon_CellCount];/* '<Root>/NvMeasOffsetOut' */
  sint16 aSO[14];                      /* '<S1>/CalCore' */
  sint16 aMO[14];                      /* '<S1>/CalCore' */
  uint16 NvSetGainOut[Rte_SysCon_CellCount];/* '<Root>/NvSetGainOut' */
  uint16 NvMeasGainOut[Rte_SysCon_CellCount];/* '<Root>/NvMeasGainOut' */
  uint16 aSG[14];                      /* '<S1>/CalCore' */
  uint16 aMG[14];                      /* '<S1>/CalCore' */
  uint8 last;                          /* '<S1>/CalCore' */
  uint8 res;                           /* '<S1>/CalCore' */
  uint8 bad;                           /* '<S1>/CalCore' */
  uint8 st;                            /* '<S1>/CalCore' */
  boolean started_not_empty;           /* '<S1>/CalCore' */
} ARID_DEF_CellCal_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_CellCal_T CellCal_ARID_DEF;/* '<Root>/CalSetOffset_V' */

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
 * '<Root>' : 'CellCal'
 * '<S1>'   : 'CellCal/CellCal_task10ms'
 * '<S2>'   : 'CellCal/CellCal_task10ms/CalCore'
 */
#endif                                 /* CellCal_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
