//-----------------------------------------------------------------------------
//                            UNCLASSIFIED
//-----------------------------------------------------------------------------
//                    DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
//  This software and the accompanying documentation are provided to the U.S.
//  Government with unlimited rights as provided in DFARS � 252.227-7014.  The
//  contractor, Veraxx Engineering Corporation, retains ownership, the
//  copyrights, and all other rights.
//
//  � Veraxx Engineering Corporation, 2023.  All rights reserved.
// 
// DEVELOPED BY: 
//  Veraxx Engineering Corporation 
//  14130 Sullyfield Circle, Suite B 
//  Chantilly, VA 20151
//  www.Veraxx.com 
//  (703)880-9000 (Voice)
//  (703)880-9005 (Fax)
//-----------------------------------------------------------------------------
//
//                         Distribution Warning:
//  WARNING - This file contains technical data whose export is restricted by
//  the Arms Export Control Act (Title 22, U.S.C., Sec. 2751 et seq.) or
//  Executive Order 12470. Violations of these export laws are subject to severe
//  criminal penalties. Disseminate in accordance with provisions of DoD
//  Directive 5230.25
//
//-----------------------------------------------------------------------------
//  
//! Title:      IRES Menu
//! Class:      CPP Source
//! Filename:   IresMenu.cpp
//! Author:     Brian Woodard
//! Purpose:    Draws the menus for the L3 WESCAM MX-10 IRES
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IresMenu.h"
#include "IresTypes.h"
#include "PrintData.h"

const char* OffOnStr[] =
{
   "OFF",
   "ON",
   "XXX"
};

const char* FalseTrueStr[] =
{
   "FALSE",
   "TRUE",
   "XXX"
};

const char* DisableEnableStr[] =
{
   "DISABLED",
   "ENABLED",
   "XXX"
};

const char* TempStatusStr[] =
{
   "OK",
   "DISABLED",
   "XXX"
};

const char* EZoomStr[] =
{
   "1.0",
   "2.0",
   "4.0",
   "X.X"
};

const char* SceneSetupStr[] =
{
   "DEFAULT",
   "HIGH",
   "LOW",
   "XXX"
};

const char* GateSizeStr[] =
{
   "X-LARGE",
   "LARGE",
   "MEDIUM",
   "SMALL",
   "XXX"
};

const char* CameraExtenderStr[] =
{
   "NORMAL",
   "WIDE",
   "XXX"
};

const char* CalibrationTypeStr[] =
{
   "1-POINT",
   "2-POINT",
   "XXX"
};

const char* PolarityStr[] =
{
   "BLACK HOT",
   "WHITE HOT",
   "XXX"
};

const char* PseudoColorStr[] =
{
   "OFF",
   "COLOR MAP 1",
   "COLOR MAP 2",
   "COLOR MAP 3",
   "COLOR MAP 4",
   "XXX"
};

const char* DesiredModeStr[] =
{
   "STOWED",
   "OPERATIONAL",
   "XXX"
};

const char* ManAutoStr[] =
{
   "MAN",
   "AUTO",
   "XXX"
};

const char* RfLaserStateStr[] =
{
   "OFF",
   "CHARGING",
   "READY",
   "FIRE",
   "RANGING",
   "BIT",
   "XXX"
};

const char* LaserTargetStr[] =
{
   "FIRST",
   "LAST",
   "XXX"
};

const char* MtiModeStr[] =
{
   "LAND",
   "MARITIME",
   "XXX"
};

const char* LdPositionStr[] =
{
   "GROUND",
   "AIR",
   "XXX"
};

const char* Ld1_57ModeStr[] =
{
   "CLASS 0M",
   "CLASS 1M",
   "XXX"
};

CIresMenu::CIresMenu(glm::mat4& Projection)
   : mConfig{},
     mCursor{},
     mMenuStack(),
     mLine(),
     mBackground(),
     mText(nullptr),
     mBoldText(nullptr),
     mProjection(Projection),
     mTextColor(1.0f),
     mLineColor(1.0f),
     mBackgroundColor(1.0f),
     mClipEnabled(false)
{
   mMenuStack.reserve(5);
   mLine.setMVP(Projection);
   mPolygon.setMVP(Projection);
   mBackground.setMVP(Projection);
}

CIresMenu::~CIresMenu()
{
   if (mText)
   {
      delete mText;
   }

   if (mBoldText)
   {
      delete mBoldText;
   }
}

void CIresMenu::Draw()
{
   if (mConfig.MenuActive)
   {
      if (mClipEnabled)
      {
         // clip menu area by drawing a stencil object around the whole area
         GLCALL(glClear(GL_STENCIL_BUFFER_BIT));
         GLCALL(glEnable(GL_STENCIL_TEST));

         GLCALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
         GLCALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
         GLCALL(glStencilMask(0xFF));
         GLCALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
         GLCALL(glDepthMask(GL_FALSE));

         // draw stencil objects
         DrawBackground(true);

         GLCALL(glStencilFunc(GL_EQUAL, 1, 0xFF));
         GLCALL(glStencilMask(~0));
         GLCALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
         GLCALL(glDepthMask(GL_TRUE));
      }

      DrawBackground();

      if (mText)
      {
         // loop over stack of menus and draw each one
         for (size_t i = 0; i < mMenuStack.size(); i++)
         {
            float font_size = (float)mText->GetFontSize();
            glm::vec3 start = mMenuStack[i].Position;

            start.x += font_size * 2.0f; // TODO: Use advance instead of size? Monospace font?
            start.y += font_size;

            DrawMenuBorder(&mMenuStack[i]);
            DrawMenuItems(mMenuStack[i].Items, start, mMenuStack[i].Size.x, (i == mMenuStack.size()-1));
         }
      }

      if (mClipEnabled)
         glDisable(GL_STENCIL_TEST);
   }
}

float CIresMenu::DrawActionItem(const TIresMenuItem& Item, float X, float Y, float Width,  float FontSize, bool Bold)
{
   float width = 0.0f;

   if (Bold && mBoldText)
      width = mBoldText->Print(IresMenuStr[Item.Id], X, Y);
   else
      width = mText->Print(IresMenuStr[Item.Id], X, Y);

   // draw triangles on action menu items
   glm::vec3 p1 = glm::vec3(X - 4.0f, Y + 2.0f, 0.0f);
   glm::vec3 p2 = p1 + glm::vec3(0.0f, -FontSize*0.66f, 0.0f);
   glm::vec3 p3 = p1 + glm::vec3(-FontSize*0.33f, -FontSize*0.33f, 0.0f);
   mPolygon.DrawTriangle(p1, p2, p3);

   glm::vec3 p4 = glm::vec3(X + width + 4.0f, Y + 2.0f, 0.0f);
   glm::vec3 p5 = p4 + glm::vec3(FontSize*0.33f, -FontSize*0.33f, 0.0f);
   glm::vec3 p6 = p4 + glm::vec3(0.0f, -FontSize*0.66f, 0.0f);
   mPolygon.DrawTriangle(p4, p5, p6);

   mText->SetColor(mTextColor);
   mPolygon.SetColor(mTextColor);

   if (Item.Type == FIELD_ACTION && Item.DataType == IRES_MENU)
   {
      uint8_t mode_and_alarm[2];

      memcpy(mode_and_alarm, &Item.Data.Value, 2);

      if (mode_and_alarm[1] != ALARM_UNKNOWN)
      {
         float str_width = mText->GetWidth(IresAlarmStr[mode_and_alarm[1]]);
         mText->Print(IresAlarmStr[mode_and_alarm[1]], Width - 200.0f - str_width, Y);
      }

      if (mode_and_alarm[0] != MODE_UNKNOWN)
      {
         mText->Print(IresModeStr[mode_and_alarm[0]], Width - 200.0f + FontSize, Y);
      }
   }

   return width;
}

float CIresMenu::DrawSelectionItem(const TIresMenuItem& Item, float X, float Y, float Width,  float FontSize, bool Bold)
{
   float width = 0.0f;
   float sel_width = 0.0f;

   if (Bold && mBoldText)
      width = mBoldText->Print(IresMenuStr[Item.Id], X, Y);
   else
      width = mText->Print(IresMenuStr[Item.Id], X, Y);

   mText->SetColor(mTextColor);
   mPolygon.SetColor(mTextColor);

   switch (Item.Id)
   {
      case MENU_LVL2_EOW_TEMP_PROC:
      case MENU_LVL2_EOW_SPATIAL_PROC:
      case MENU_LVL2_IR_TEMP_PROC:
      case MENU_LVL2_IR_SPATIAL_PROC:
      case MENU_LVL2_IR_CAMERA_POWER:
      case MENU_LVL2_MTI_PROCESSING:
      case MENU_LVL2_MTI_BREADCRUMBS:
      case MENU_LVL2_MTI_LIFEJACKET_DETECTION:
      case MENU_LVL2_IF_MODE:
      case MENU_LVL2_SS_MODE:
      case MENU_LVL2_SS_FLASH_POLARITY:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(OffOnStr))
            temp = ArrayCount(OffOnStr)-1;

         sel_width = mText->GetWidth(OffOnStr[temp]);
         mText->Print(OffOnStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_EOW_EZOOM:
      case MENU_LVL2_IR_EZOOM:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(EZoomStr))
            temp = ArrayCount(EZoomStr)-1;

         sel_width = mText->GetWidth(EZoomStr[temp]);
         mText->Print(EZoomStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_EOW_LEVEL:
      case MENU_LVL2_IR_LEVEL:
      case MENU_LVL2_IR_THRESHOLD:
      {
         uint16_t temp = *(uint16_t*)&Item.Data.Value;
         char     text[10] = {};

         sprintf(text, "% *hd", 3, temp);
         sel_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_EOW_SCENE_SETUP:
      case MENU_LVL2_IR_SCENE_SETUP:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(SceneSetupStr))
            temp = ArrayCount(SceneSetupStr)-1;

         sel_width = mText->GetWidth(SceneSetupStr[temp]);
         mText->Print(SceneSetupStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_EOW_GATE:
      case MENU_LVL2_IR_GATE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(GateSizeStr))
            temp = ArrayCount(GateSizeStr)-1;

         sel_width = mText->GetWidth(GateSizeStr[temp]);
         mText->Print(GateSizeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_EOW_CAMERA_EXTENDER:
      case MENU_LVL2_IR_CAMERA_EXTENDER:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(CameraExtenderStr))
            temp = ArrayCount(CameraExtenderStr)-1;

         sel_width = mText->GetWidth(CameraExtenderStr[temp]);
         mText->Print(CameraExtenderStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IR_CALIBRATION_SEL:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(CalibrationTypeStr))
            temp = ArrayCount(CalibrationTypeStr)-1;

         sel_width = mText->GetWidth(CalibrationTypeStr[temp]);
         mText->Print(CalibrationTypeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IR_POLARITY:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(PolarityStr))
            temp = ArrayCount(PolarityStr)-1;

         sel_width = mText->GetWidth(PolarityStr[temp]);
         mText->Print(PolarityStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IR_PSEUDO_COLOR:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(PseudoColorStr))
            temp = ArrayCount(PseudoColorStr)-1;

         sel_width = mText->GetWidth(PseudoColorStr[temp]);
         mText->Print(PseudoColorStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IR_DESIRED_MODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(DesiredModeStr))
            temp = ArrayCount(DesiredModeStr)-1;

         sel_width = mText->GetWidth(DesiredModeStr[temp]);
         mText->Print(DesiredModeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_RF_TARGET:
      case MENU_LVL2_LD_TARGET:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(LaserTargetStr))
            temp = ArrayCount(LaserTargetStr)-1;

         sel_width = mText->GetWidth(LaserTargetStr[temp]);
         mText->Print(LaserTargetStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_RF_MIN_RANGE:
      case MENU_LVL2_RF_MAX_RANGE:
      case MENU_LVL2_ILLUM_AUTO_LOW_MED:
      case MENU_LVL2_ILLUM_AUTO_MED_HIGH:
      case MENU_LVL2_LD_MIN_RANGE:
      case MENU_LVL2_LD_MAX_RANGE:
      {
         uint16_t temp = *(uint16_t*)&Item.Data.Value;
         char     text[10] = {};

         sprintf(text, "% *hd", 5, temp);
         sel_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_AVT_ALGORITHM:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > AVT_ALG_COUNT)
            temp = AVT_ALG_COUNT-1;

         sel_width = mText->GetWidth(AvtAlgorithmStr[temp]);
         mText->Print(AvtAlgorithmStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_AVT_GATE_TYPE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > AVT_GATE_TYPE_COUNT)
            temp = AVT_GATE_TYPE_COUNT-1;

         sel_width = mText->GetWidth(AvtGateTypeStr[temp]);
         mText->Print(AvtGateTypeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_AVT_GATE_SIZE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > AVT_GATE_SIZE_COUNT)
            temp = AVT_GATE_SIZE_COUNT-1;

         sel_width = mText->GetWidth(AvtGateSizeStr[temp]);
         mText->Print(AvtGateSizeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_AVT_COAST_TIMEOUT:
      case MENU_LVL2_MTI_LAND_SENSITIVITY:
      case MENU_LVL2_MTI_MIN_BREADCRUMBS:
      case MENU_LVL2_MTI_MAX_BREADCRUMBS:
      case MENU_LVL2_MTI_MAX_BREADCRUMBS_LIFESPAN:
      case MENU_LVL2_MTI_MARITIME_SENSITIVITY:
      case MENU_LVL2_MTI_LIFEJACKET_SENSITIVITY:
      case MENU_LVL2_ILLUM_MAX_USAGE:
      case MENU_LVL2_LD_SELECT_CODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;
         char    text[10] = {};

         sprintf(text, "% *hd", 3, temp);
         sel_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_MTI_MARITIME_MODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(MtiModeStr))
            temp = ArrayCount(MtiModeStr)-1;

         sel_width = mText->GetWidth(MtiModeStr[temp]);
         mText->Print(MtiModeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_MTI_LAND_BOX_COLOR:
      case MENU_LVL2_MTI_MARITIME_BOX_COLOR:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > MTI_COLOR_COUNT)
            temp = MTI_COLOR_COUNT-1;

         sel_width = mText->GetWidth(MtiBoxColorStr[temp]);
         mText->Print(MtiBoxColorStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_ILLUM_MODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ILLUM_MODE_COUNT-1)
            temp = ILLUM_MODE_COUNT-1;

         sel_width = mText->GetWidth(IllumModeStr[temp]);
         mText->Print(IllumModeStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_LD_CODE1_ENTRY:
      case MENU_LVL2_LD_CODE2_ENTRY:
      case MENU_LVL2_LD_CODE3_ENTRY:
      case MENU_LVL2_LD_CODE4_ENTRY:
      case MENU_LVL2_LD_CODE5_ENTRY:
      {
         uint16_t temp = *(uint16_t*)&Item.Data.Value;
         char     text[10] = {};

         sprintf(text, "% *hd", 4, temp);
         sel_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_LD_BATTLE_OVERRIDE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(DisableEnableStr))
            temp = ArrayCount(DisableEnableStr)-1;

         sel_width = mText->GetWidth(DisableEnableStr[temp]);
         mText->Print(DisableEnableStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IF_BLEND_MODE:
      case MENU_LVL2_SS_INITIAL_COMBINATION:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(ManAutoStr))
            temp = ArrayCount(ManAutoStr)-1;

         sel_width = mText->GetWidth(ManAutoStr[temp]);
         mText->Print(ManAutoStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IF_MANUAL_BLEND:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;
         uint8_t secondary = 100 - temp;
         char    text[10] = {};

         sprintf(text, "% *hd:% *hd", 3, temp, 3, secondary);
         sel_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_IF_COMBINATION:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > FUSE_COMBO_COUNT-1)
            temp = FUSE_COMBO_COUNT-1;

         sel_width = mText->GetWidth(FusionCombinationStr[temp]);
         mText->Print(FusionCombinationStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_SS_COMBINATION:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > SEESPOT_COMBO_COUNT-1)
            temp = SEESPOT_COMBO_COUNT-1;

         sel_width = mText->GetWidth(SeeSpotCombinationStr[temp]);
         mText->Print(SeeSpotCombinationStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      case MENU_LVL2_SS_SPOT_COLOR:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > SEESPOT_COLOR_COUNT-1)
            temp = SEESPOT_COLOR_COUNT-1;

         sel_width = mText->GetWidth(SeeSpotColorStr[temp]);
         mText->Print(SeeSpotColorStr[temp], Width - 200.0f - sel_width, Y);
         break;
      }
      default:
         break;
   }

   // draw triangles on selection menu items
   float x = Width - 212.0f - sel_width;
   float y = Y + 2.0f;
   glm::vec3 p1 = glm::vec3(x, y, 0.0f);
   glm::vec3 p2 = p1 + glm::vec3(FontSize*0.33f, -FontSize*0.33f, 0.0f);
   glm::vec3 p3 = p1 + glm::vec3(0.0f, -FontSize*0.66f, 0.0f);
   mPolygon.DrawTriangle(p1, p2, p3);

   x += sel_width + 24.0f;
   glm::vec3 p4 = glm::vec3(x, y, 0.0f);
   glm::vec3 p5 = p4 + glm::vec3(0.0f, -FontSize*0.66f, 0.0f);
   glm::vec3 p6 = p4 + glm::vec3(-FontSize*0.33f, -FontSize*0.33f, 0.0f);
   mPolygon.DrawTriangle(p4, p5, p6);

   return width;
}

float CIresMenu::DrawStatusItem(const TIresMenuItem& Item, float X, float Y, float Width,  float FontSize, bool Bold)
{
   float width = 0.0f;

   if (Bold && mBoldText)
      width = mBoldText->Print(IresMenuStr[Item.Id], X, Y);
   else
      width = mText->Print(IresMenuStr[Item.Id], X, Y);

   mText->SetColor(mTextColor);
   mPolygon.SetColor(mTextColor);

   switch (Item.Id)
   {
      case MENU_LVL2_GIMBAL_MODE:
      case MENU_LVL2_EOW_MODE:
      case MENU_LVL2_IR_MODE:
      case MENU_LVL2_RF_MODE:
      case MENU_LVL2_AVT_MODE:
      case MENU_LVL2_MTI_MODE:
      case MENU_LVL2_LD_MODE:
      {
         uint8_t mode = *(uint8_t*)&Item.Data.Value;

         if (mode > MODE_UNKNOWN)
            mode = MODE_UNKNOWN;

         float str_width = mText->GetWidth(IresModeStr[mode]);
         mText->Print(IresModeStr[mode], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_ALARMS_STATUS:
      case MENU_LVL2_EOW_ALARMS_STATUS:
      case MENU_LVL2_IR_ALARMS_STATUS:
      case MENU_LVL2_RF_ALARMS_STATUS:
      case MENU_LVL2_AVT_ALARMS_STATUS:
      case MENU_LVL2_MTI_ALARMS_STATUS:
      case MENU_LVL2_LD_ALARMS_STATUS:
      {
         uint8_t alarm = *(uint8_t*)&Item.Data.Value;

         if (alarm > ALARM_UNKNOWN)
            alarm = ALARM_UNKNOWN;

         float str_width = mText->GetWidth(IresAlarmStr[alarm]);
         mText->Print(IresAlarmStr[alarm], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_PROCESSOR_TEMP:
      case MENU_LVL2_ILLUM_TEMPERATURE:
      {
         float temp = *(float*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 0, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("degC", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_HUMIDITY:
      {
         float temp = *(float*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 0, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("%", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_VOLTAGE:
      {
         float temp = *(float*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 1, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("V", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_AZ_STAB:
      case MENU_LVL2_GIMBAL_EL_STAB:
      {
         double temp = *(double*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 2, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("uRad", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_GIMBAL_AZ:
      case MENU_LVL2_GIMBAL_EL:
      {
         double temp = *(double*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 2, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("deg", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_EOW_FOCAL_LENGTH:
      case MENU_LVL2_IR_FOCAL_LENGTH:
      {
         float temp = *(float*)&Item.Data.Value;
         char  text[10] = {};

         sprintf(text, "%.*f", 1, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("mm", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_EOW_FOCUS:
      case MENU_LVL2_IR_FOCUS:
      case MENU_LVL2_LD_TOTAL_BORESIGHTS:
      {
         uint16_t temp = *(uint16_t*)&Item.Data.Value;
         char     text[10] = {};

         sprintf(text, "% *hd", 6, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_EOW_SENSITIVITY:
      case MENU_LVL2_IR_SENSITIVITY:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;
         char    text[10] = {};

         if (temp == 0)
            strcpy(text, "AUTO");
         else
            sprintf(text, "%d", temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_EOW_TEMP_STATUS:
      case MENU_LVL2_IR_TEMP_STATUS:
      case MENU_LVL2_RF_TEMP_STATUS:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(TempStatusStr))
            temp = ArrayCount(TempStatusStr)-1;

         float str_width = mText->GetWidth(TempStatusStr[temp]);
         mText->Print(TempStatusStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_RF_RANGE_MODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(ManAutoStr))
            temp = ArrayCount(ManAutoStr)-1;

         float str_width = mText->GetWidth(ManAutoStr[temp]);
         mText->Print(ManAutoStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_RF_LASER_STATE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(RfLaserStateStr))
            temp = ArrayCount(RfLaserStateStr)-1;

         float str_width = mText->GetWidth(RfLaserStateStr[temp]);
         mText->Print(RfLaserStateStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_RF_TARGET_FOUND:
      case MENU_LVL2_LD_BORESIGHT_INSTALLED:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(FalseTrueStr))
            temp = ArrayCount(FalseTrueStr)-1;

         float str_width = mText->GetWidth(FalseTrueStr[temp]);
         mText->Print(FalseTrueStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_RF_RANGE:
      {
         uint16_t temp = *(uint16_t*)&Item.Data.Value;
         char     text[10] = {};

         sprintf(text, "% *hd", 4, temp);
         float str_width = mText->GetWidth(text);
         mText->Print(text, Width - 200.0f - str_width, Y);
         mText->Print("m", Width - 200.0f + FontSize, Y);
         break;
      }
      case MENU_LVL2_AVT_TRACKING_STATUS:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > AVT_TRACK_COUNT)
            temp = AVT_TRACK_COUNT-1;

         float str_width = mText->GetWidth(AvtTrackingStatusStr[temp]);
         mText->Print(AvtTrackingStatusStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_ILLUM_STATE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ILLUM_COUNT)
            temp = ILLUM_COUNT-1;

         float str_width = mText->GetWidth(IllumStateStr[temp]);
         mText->Print(IllumStateStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_LD_POSITION:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(LdPositionStr))
            temp = ArrayCount(LdPositionStr)-1;

         float str_width = mText->GetWidth(LdPositionStr[temp]);
         mText->Print(LdPositionStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      case MENU_LVL2_LD_1_57_MODE:
      {
         uint8_t temp = *(uint8_t*)&Item.Data.Value;

         if (temp > ArrayCount(Ld1_57ModeStr))
            temp = ArrayCount(Ld1_57ModeStr)-1;

         float str_width = mText->GetWidth(Ld1_57ModeStr[temp]);
         mText->Print(Ld1_57ModeStr[temp], Width - 200.0f - str_width, Y);
         break;
      }
      default:
         break;
   }

   return width;
}

void CIresMenu::DrawBackground(bool BackgroundOnly)
{
   glm::vec3 p1 = glm::vec3(mConfig.X, mConfig.Y, 0.0f);
   glm::vec3 p2 = p1 + glm::vec3(mConfig.Width, 0.0f, 0.0f);
   glm::vec3 p3 = p1 + glm::vec3(mConfig.Width, mConfig.Height, 0.0f);
   glm::vec3 p4 = p1 + glm::vec3(0.0f,          mConfig.Height, 0.0f);

   mBackground.DrawQuad((float)mConfig.X, (float)mConfig.Y, (float)mConfig.Width, (float)mConfig.Height);

   if (!BackgroundOnly)
   {
      mLine.SetPosition(p1, p2);
      mLine.Draw();
      mLine.SetPosition(p2, p3);
      mLine.Draw();
      mLine.SetPosition(p3, p4);
      mLine.Draw();
      mLine.SetPosition(p4, p1);
      mLine.Draw();
   }
}

void CIresMenu::DrawMenuBorder(TIresMenuData* Menu)
{
   glm::vec3 p1 = Menu->Position;
   glm::vec3 p2 = p1 + glm::vec3(Menu->Size.x, 0.0f, 0.0f);
   glm::vec3 p3 = p1 + glm::vec3(Menu->Size.x, Menu->Size.y, 0.0f);
   glm::vec3 p4 = p1 + glm::vec3(0.0f,         Menu->Size.y, 0.0f);

   float font_size = (float)mText->GetFontSize();
   float x = Menu->Position.x + 3;
   float y = Menu->Position.y + font_size;

   //printf("Menu: Draw %s\n", Menu->Title);

   mBackground.DrawQuad(Menu->Position.x, Menu->Position.y, Menu->Size.x, Menu->Size.y);

   mLine.SetPosition(p1, p2);
   mLine.Draw();
   mLine.SetPosition(p2, p3);
   mLine.Draw();
   mLine.SetPosition(p3, p4);
   mLine.Draw();
   mLine.SetPosition(p4, p1);
   mLine.Draw();

   // draw line under title
   p1.y += font_size + 3;
   p2.y += font_size + 3;
   mLine.SetPosition(p1, p2);
   mLine.Draw();

   mText->Print(Menu->Title, x, y);
}

void CIresMenu::DrawMenuItems(const std::vector<TIresMenuItem>& Items, glm::vec3 Start, float Width, bool Bottom)
{
   float font_size = (float)mText->GetFontSize() + 3.0f;
   float x = Start.x + 3.0f;
   float y = Start.y + font_size * 2.0f;
   float bot_width = 0.0f;
   bool  first_bottom = true;

   // do an initial pass to get the width of the bottom action items
   for (size_t i  = 0; i < Items.size(); i++)
   {
      if (Items[i].Type == FIELD_ACTION_BOT)
         bot_width += mText->GetWidth(IresMenuStr[Items[i].Id]) + font_size * 3.0f;
   }

   for (size_t i = 0; i < Items.size(); i++)
   {
      float width = 0;

      // only draw bottom menu items on last menu in stack
      if (Items[i].Type == FIELD_ACTION_BOT && Bottom)
      {
         // draw action item at bottom of menu
         bool  bold = (i == mCursor.CursorLocation && mCursor.CursorType == CURSOR_BOLD);

         if (first_bottom)
         {
            y = mConfig.Height + 3.0f;
            x = mConfig.X + mConfig.Width - bot_width; // right-align bottom action items
            first_bottom = false;
         }
         
         width = DrawActionItem(Items[i], x, y, Width, font_size, bold);

         x += width + font_size * 3.0f;
      }
      else
      {
         bool  bold = (i == mCursor.CursorLocation && mCursor.CursorType == CURSOR_BOLD);
         int   indent_level = Items[i].Appearance & 0x7;
         float indent = font_size * indent_level;

         switch (Items[i].Type)
         {
            case FIELD_ACTION:
               width = DrawActionItem(Items[i], x + indent, y, Width, font_size, bold);
               break;
            case FIELD_STATUS:
               width = DrawStatusItem(Items[i], x + indent, y, Width, font_size, bold);
               break;
            case FIELD_SINGLE_SELECTION:
            case FIELD_LIST_SELECTION:
            case FIELD_RANGE_SELECTION:
               width = DrawSelectionItem(Items[i], x + indent, y, Width, font_size, bold);
               break;
            case FIELD_LINE:
            {
               glm::vec3 start = glm::vec3(Start.x + 5.0f - font_size * 2.0f, y, 0.0f);
               glm::vec3 end = start;

               end.x += Width;

               mLine.SetPosition(start, end);
               mLine.Draw();
               break;
            }
            default:
               break;
         }

         y += font_size;
      }

      if (Bottom)
      {
         if (i == mCursor.CursorLocation)
         {
            float cursor_x;
            float cursor_y;
            
            if (Items[i].Type == FIELD_ACTION_BOT)
            {
               cursor_x = x - font_size - 4.0f - (width + font_size * 3.0f);
               cursor_y = y;
            }
            else
            {
               cursor_x = x - font_size - 4.0f;
               cursor_y = y - font_size;
            }

            switch (mCursor.CursorType)
            {
               case CURSOR_CARAT:
                  mBoldText->Print(">", cursor_x, cursor_y);
                  break;
               case CURSOR_DIAMOND:
               {
                  glm::vec3 p1 = glm::vec3(cursor_x, cursor_y, 0.0f);
                  glm::vec3 p2 = p1 + glm::vec3(font_size/4.0, -font_size/4.0, 0.0f);
                  glm::vec3 p3 = p1 + glm::vec3(-font_size/4.0, -font_size/4.0, 0.0f);
                  glm::vec3 p4 = p1 + glm::vec3(0.0f, -font_size/2.0, 0.0f);
                  mPolygon.DrawQuad(p1, p2, p3, p4);
                  break;
               }
               case CURSOR_HIGHLIGHT:
               {
                  float length = (width + font_size * 4.25f) - font_size*3;
                  int   indent_level = Items[i].Appearance & 0x7;
                  float indent = font_size * indent_level;

                  mPolygon.DrawQuad((font_size + 4.0f) + cursor_x - 14.0f + indent, cursor_y + 4.0f, length, -font_size);

                  mText->SetColor(mBackgroundColor);
                  mPolygon.SetColor(mBackgroundColor);

                  switch (Items[i].Type)
                  {
                     case FIELD_ACTION:
                     case FIELD_ACTION_BOT:
                        DrawActionItem(Items[i], (font_size + 4.0f) + cursor_x + indent, cursor_y, Width, font_size, false);
                        break;
                     case FIELD_STATUS:
                        DrawStatusItem(Items[i], (font_size + 4.0f) + cursor_x + indent, cursor_y, Width, font_size, false);
                        break;
                     case FIELD_SINGLE_SELECTION:
                     case FIELD_LIST_SELECTION:
                     case FIELD_RANGE_SELECTION:
                        DrawSelectionItem(Items[i], (font_size + 4.0f) + cursor_x + indent, cursor_y, Width, font_size, false);
                        break;
                     default:
                        break;
                  }

                  mText->SetColor(mTextColor);
                  mPolygon.SetColor(mTextColor);
                  break;
               }
               case CURSOR_BOLD:
                  // done above
                  break;
               default:
                  break;
            }
         }
      }
   }
}

void CIresMenu::ProcessPdu(char* Buffer, int Size)
{
   if (Size > 0)
   {
      bool processing = true;
      int  index = 0;

      //printf("%s\n", CPrintData::GetDataAsString(Buffer, Size));

      while (processing)
      {
         TIresMenuHeader* header = (TIresMenuHeader*)&Buffer[index];

         switch (header->Type)
         {
            case MENU_CONFIG_PDU:
            {
               TIresMenuConfigPdu* config;

               index += sizeof(TIresMenuHeader);

               config = (TIresMenuConfigPdu*)&Buffer[index];
               if (config->MenuActive != mConfig.MenuActive)
                  printf("Menu: %s\n", config->MenuActive ? "Active" : "Inactive");

               mConfig = *config;

               if (mText)
               {
                  delete mText;
               }

               if (mBoldText)
               {
                  delete mBoldText;
               }

               // create a font for the menu
               if (mConfig.Font >= 0 && mConfig.Font < FONT_COUNT)
               {
                  mText = new CText(mProjection, mConfig.FontSize, IresMenuFontStr[mConfig.Font]);
                  if (!mText->IsInitialized())
                  {
                     delete mText;
                     mText = nullptr;
                  }

                  mBoldText = new CText(mProjection, mConfig.FontSize, IresMenuFontStr[mConfig.Font+1]);
                  if (!mBoldText->IsInitialized())
                  {
                     delete mBoldText;
                     mBoldText = nullptr;
                  }
               }

               if (!config->MenuActive && mMenuStack.size())
                  mMenuStack.clear();

               // set background color
               mBackgroundColor = glm::vec4((float)mConfig.BackgroundColor.Red / 255.0f,
                                            (float)mConfig.BackgroundColor.Green / 255.0f,
                                            (float)mConfig.BackgroundColor.Blue / 255.0f,
                                            (float)mConfig.BackgroundColor.Alpha / 255.0f);

               mBackground.SetColor(mBackgroundColor);

               // set text color
               mTextColor = glm::vec4((float)mConfig.TextColor.Red / 255.0f,
                                      (float)mConfig.TextColor.Green / 255.0f,
                                      (float)mConfig.TextColor.Blue / 255.0f,
                                      (float)mConfig.TextColor.Alpha / 255.0f);

               if (mText)
               {
                  mText->SetColor(mTextColor);
                  mText->SetInvertY(true);
               }

               if (mBoldText)
               {
                  mBoldText->SetColor(mTextColor);
                  mBoldText->SetInvertY(true);
               }

               mPolygon.SetColor(mTextColor);

               // set line color
               mLineColor = glm::vec4((float)mConfig.LineColor.Red / 255.0f,
                                      (float)mConfig.LineColor.Green / 255.0f,
                                      (float)mConfig.LineColor.Blue / 255.0f,
                                      (float)mConfig.LineColor.Alpha / 255.0f);

               mLine.SetColor(mLineColor);
               mLine.SetLineWidth(mConfig.LineWidth);

               index += sizeof(TIresMenuConfigPdu);

               break;
            }
            case MENU_PDU:
            {
               TIresMenuPdu* menu;
               char*         title;
               TIresMenuData menu_data;

               index += sizeof(TIresMenuHeader);

               menu = (TIresMenuPdu*)&Buffer[index];

               index += sizeof(TIresMenuPdu);
               title = &Buffer[index];

               menu_data.Position = glm::vec3(menu->X, menu->Y, 0.0f);
               menu_data.Size = glm::vec3(menu->Width, menu->Height, 0.0f);
               strcpy(menu_data.Title, title);

               if (menu->CloseMenu)
               {
                  // a sub-menu has been closed, pop off 2 menus because this
                  // PDU will re-populate the top level menu
                  if (mMenuStack.size() > 0)
                     mMenuStack.pop_back();

                  if (mMenuStack.size() > 0)
                     mMenuStack.pop_back();
               }

               index += (int)strlen(title)+1;

               // unpack menu items
               for (int i = 0; i < menu->NumMenuItems; i++)
               {
                  TIresMenuItem item = {};
                  TIresMenuItemPdu* item_pdu = (TIresMenuItemPdu*)&Buffer[index];

                  assert(item_pdu->Index == menu_data.Items.size());

                  index += UnpackMenuItemPdu(item_pdu, item);

                  menu_data.Items.push_back(item);
               }

               mMenuStack.push_back(menu_data);

               printf("Menu: got menu PDU for %s with %zu items, stack size %zu\n", title, menu_data.Items.size(), mMenuStack.size());

               break;
            }
            case MENU_ITEM_PDU:
            {
               index += sizeof(TIresMenuHeader);

               TIresMenuItemPdu* item_pdu = (TIresMenuItemPdu*)&Buffer[index];
               size_t            menu_index = mMenuStack.size();

               assert(menu_index > 0);
               assert(item_pdu->Index < mMenuStack[menu_index-1].Items.size());

               index += UnpackMenuItemPdu(item_pdu, mMenuStack[menu_index-1].Items[item_pdu->Index]);

               break;
            }
            case MENU_CURSOR_PDU:
            {
               TIresMenuCursorPdu* cursor;

               index += sizeof(TIresMenuHeader);

               cursor = (TIresMenuCursorPdu*)&Buffer[index];
               mCursor = *cursor;

               index += sizeof(TIresMenuCursorPdu);
               break;
            }
            default:
               processing = false;
               break;
         }

         if (index >= Size)
            processing = false;
      }
   }
}

int CIresMenu::UnpackMenuItemPdu(const TIresMenuItemPdu* ItemPdu, TIresMenuItem& Item)
{
   int   pdu_size = sizeof(TIresMenuItemPdu);
   int   bytes_to_copy = 0;
   char* buffer = (char*)ItemPdu;

   Item.Id = (eIresMenu)ItemPdu->Id;
   Item.Type = (eIresFieldType)ItemPdu->Type;
   Item.DataType = (eIresDataType)ItemPdu->DataType;
   Item.Index = ItemPdu->Index;
   Item.Appearance = (eIresMenuAppearance)ItemPdu->Appearance;

   buffer += pdu_size;

   switch (ItemPdu->DataType)
   {
      case IRES_B8:
      case IRES_U8:
      case IRES_I8:
         bytes_to_copy = 1;
         break;
      case IRES_U16:
      case IRES_I16:
      case IRES_MENU: // 2 bytes are stuffed onto menus for (alarm/mode)
         bytes_to_copy = 2;
         break;
      case IRES_U32:
      case IRES_I32:
      case IRES_F32:
         bytes_to_copy = 4;
         break;
      case IRES_U64:
      case IRES_I64:
      case IRES_F64:
         bytes_to_copy = 8;
         break;
      default:
         break;
   }

   if (bytes_to_copy > 0)
      memcpy(&Item.Data.Value, buffer, bytes_to_copy);

   pdu_size += bytes_to_copy;

   return pdu_size;
}
