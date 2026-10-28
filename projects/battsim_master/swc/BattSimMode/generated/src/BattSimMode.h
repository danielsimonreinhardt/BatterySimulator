/*
 * File: BattSimMode.h
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

#ifndef BattSimMode_h_
#define BattSimMode_h_
#ifndef BattSimMode_COMMON_INCLUDES_
#define BattSimMode_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_BattSimMode.h"
#endif                                 /* BattSimMode_COMMON_INCLUDES_ */

#include "BattSimMode_types.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  uint8 st;                            /* '<S1>/ModeCore' */
  uint8 reason;                        /* '<S1>/ModeCore' */
  uint8 aliveCnt;                      /* '<S1>/ModeCore' */
  uint8 tick;                          /* '<S1>/ModeCore' */
  uint8 cntA;                          /* '<S1>/ModeCore' */
  uint8 cntR;                          /* '<S1>/ModeCore' */
  boolean outBlock;                    /* '<S1>/ModeCore' */
  boolean levA;                        /* '<S1>/ModeCore' */
  boolean levR;                        /* '<S1>/ModeCore' */
  boolean wdDone;                      /* '<S1>/ModeCore' */
} ARID_DEF_BattSimMode_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_BattSimMode_T BattSimMode_ARID_DEF;/* '<S1>/ModeCore' */

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
 * '<Root>' : 'BattSimMode'
 * '<S1>'   : 'BattSimMode/BattSimMode_task10ms'
 * '<S2>'   : 'BattSimMode/BattSimMode_task10ms/ModeCore'
 */
#endif                                 /* BattSimMode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
