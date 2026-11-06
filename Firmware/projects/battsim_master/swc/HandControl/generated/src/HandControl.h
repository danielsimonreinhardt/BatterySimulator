/*
 * File: HandControl.h
 *
 * Code generated for Simulink model 'HandControl'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef HandControl_h_
#define HandControl_h_
#ifndef HandControl_COMMON_INCLUDES_
#define HandControl_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_HandControl.h"
#endif                                 /* HandControl_COMMON_INCLUDES_ */

#include "HandControl_types.h"
#include "Rte_Cfg.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  float32 HandSetpoint_V[Rte_SysCon_CellCount];/* '<Root>/HandSetpoint_V' */
  sint32 spMv[14];                     /* '<S1>/HandCore' */
  sint32 accV;                         /* '<S1>/HandCore' */
  sint32 accS;                         /* '<S1>/HandCore' */
  uint16 lastV;                        /* '<S1>/HandCore' */
  uint16 lastS;                        /* '<S1>/HandCore' */
  uint8 cntK[14];                      /* '<S1>/HandCore' */
  uint8 sel;                           /* '<S1>/HandCore' */
  uint8 cntV;                          /* '<S1>/HandCore' */
  uint8 cntS;                          /* '<S1>/HandCore' */
  boolean HandEnable[Rte_SysCon_CellCount];/* '<Root>/HandEnable' */
  boolean en0[14];                     /* '<S1>/HandCore' */
  boolean started_not_empty;           /* '<S1>/HandCore' */
  boolean coarse;                      /* '<S1>/HandCore' */
  boolean selAll;                      /* '<S1>/HandCore' */
  boolean wasActive;                   /* '<S1>/HandCore' */
} ARID_DEF_HandControl_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_HandControl_T HandControl_ARID_DEF;/* '<Root>/HandSetpoint_V' */

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
 * '<Root>' : 'HandControl'
 * '<S1>'   : 'HandControl/HandControl_task10ms'
 * '<S2>'   : 'HandControl/HandControl_task10ms/HandCore'
 */
#endif                                 /* HandControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
