#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "z64camera.h"

#define IS_CRITICAL_HEALTH (gSaveContext.save.saveInfo.playerData.health <= 0x10)
#define CFG_CRIT_WIGGLE_DISABLED (recomp_get_config_u32("disable_crit_wiggle"))

typedef enum CfgBool {
  CFG_NO,
  CFG_YES
} CfgBool;

Camera* sCamera;


RECOMP_HOOK("Camera_Normal1")
void on_Camera_Normal1(Camera* camera)
{
  sCamera = camera;
}

f32 Camera_ScaledStepToCeilF(f32 target, f32 cur, f32 stepScale, f32 minDiff);
s16 Camera_ScaledStepToCeilS(s16 target, s16 cur, f32 stepScale, s16 minDiff);

RECOMP_HOOK_RETURN("Camera_Normal1")
void after_Camera_Normal1(void)
{
  Normal1ReadOnlyData* roData = &sCamera->paramData.norm1.roData;
  if(IS_CRITICAL_HEALTH && CFG_CRIT_WIGGLE_DISABLED == CFG_YES)
  {
    sCamera->inputDir.y -= ((s32)(sCamera->play->state.frames << 0x18) >> 0x15) & 0xFD68;
    sCamera->fov = Camera_ScaledStepToCeilF(roData->unk_18, sCamera->fov, sCamera->fovUpdateRate, 0.1f);
    sCamera->roll = Camera_ScaledStepToCeilS(0, sCamera->roll, 0.2f, 5);
  }
}
