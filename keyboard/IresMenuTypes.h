//-----------------------------------------------------------------------------
// UNCLASSIFIED
//-----------------------------------------------------------------------------
// DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
// This software and the accompanying documentation are provided to the U.S. 
// Government with unlimited rights as provided in DFARS § 252.227-7014. The 
// contractor, Veraxx Engineering Corporation, retains ownership, the 
// copyrights, and all other rights.
//
// © Veraxx Engineering Corporation 2023. All rights reserved.
// 
// DEVELOPED BY: 
// Veraxx Engineering Corporation 
// 14221A Willard Road, Suite 200 
// Chantilly, VA 20151 
// (703)880-9000 (Voice) 
// (703)880-9005 (Fax) 
//-----------------------------------------------------------------------------
// Distribution Warning:
// WARNING - This document contains technical data whose export is restricted by
// the Arms Export Control Act (Title 22, U.S.C., Sec. 2751 et seq.) or
// Executive Order 12470. Violations of these export laws are subject to severe
// criminal penalties. Disseminate in accordance with provisions of DoD
// Directive 5230.25
//
//-----------------------------------------------------------------------------
// Title: IRES Interface Menu Types
// Class: C++ Header
// Filename: IresMenuTypes.h
// Author: Brian Woodard
// Purpose: This module performs the following tasks:
//
// Provides types used by the CIresMenu class.
//
// NOTE: This file needs to match between the Host and Symbology DLL code!
//
//-----------------------------------------------------------------------------

#ifndef IRESMENUTYPES__H
#define IRESMENUTYPES__H

#include <vector>
#include <stddef.h>
#include <stdint.h>

const int MENU_FRAME_RATE    = 20;
const int MAX_MENU_NAME_SIZE = 128;
const int MAX_MENU_BUFFER    = 4096;
const int MENU_CLOSE_DELAY   = 2 * MENU_FRAME_RATE; // 2 seconds

// NOTE: Keep in order of menu levels and type of menu
//   The difference is used to reserve allocation in the
//   std::vector when building menus
enum eIresMenu
{
   MENU_NONE,
   MENU_LVL1_GIMBAL,
   MENU_LVL1_EOW,
   MENU_LVL1_IR,
   MENU_LVL1_RANGE_FINDER,
   MENU_LVL1_AVT,
   MENU_LVL1_MTI,
   MENU_LVL1_ILLUM, // possibly more than 1 menu if more than one illuminator is installed
   MENU_LVL1_LD,
   MENU_LVL1_VIDEO_IO,
   MENU_LVL1_IMAGE_FUSION,
   MENU_LVL1_SEE_SPOT,
   MENU_LVL1_OPERATOR_GIMBAL,
   MENU_LVL1_OPERATOR_PAYLOAD,
   MENU_LVL1_MISSION,
   MENU_LVL1_DIGITAL_OVERLAYS,
   MENU_LVL1_ANALOG_OVERLAYS,
   MENU_LVL1_STEERING_MODES,
   MENU_LVL1_PROFILES,
   MENU_LVL1_MISC,
   MENU_LVL1_BORESIGHT,
   MENU_LVL1_ALARMS,
   MENU_LVL1_SYSTEM_BIT,
   MENU_LVL1_DONE,

   MENU_LVL2_GIMBAL_MODE,
   MENU_LVL2_GIMBAL_ALARMS_STATUS,
   MENU_LVL2_GIMBAL_PROCESSOR_TEMP,
   MENU_LVL2_GIMBAL_HUMIDITY,
   MENU_LVL2_GIMBAL_VOLTAGE,
   MENU_LVL2_GIMBAL_AZ_STAB,
   MENU_LVL2_GIMBAL_EL_STAB,
   MENU_LVL2_GIMBAL_AZ,
   MENU_LVL2_GIMBAL_EL,
   MENU_LVL2_GIMBAL_ALARMS,
   MENU_LVL2_GIMBAL_BIT,
   MENU_LVL2_GIMBAL_DONE,

   MENU_LVL2_EOW_MODE,
   MENU_LVL2_EOW_ALARMS_STATUS,
   MENU_LVL2_EOW_FOCAL_LENGTH,
   MENU_LVL2_EOW_FOCUS,
   MENU_LVL2_EOW_SENSITIVITY,
   MENU_LVL2_EOW_TEMP_STATUS,
   MENU_LVL2_EOW_TEMP_PROC,
   MENU_LVL2_EOW_SPATIAL_PROC,
   MENU_LVL2_EOW_EZOOM,
   MENU_LVL2_EOW_LEVEL,
   MENU_LVL2_EOW_SCENE_SETUP,
   MENU_LVL2_EOW_GATE,
   MENU_LVL2_EOW_CAMERA_EXTENDER,
   MENU_LVL2_EOW_MISC,
   MENU_LVL2_EOW_ALARMS,
   MENU_LVL2_EOW_BIT,
   MENU_LVL2_EOW_DONE,

   MENU_LVL2_IR_MODE,
   MENU_LVL2_IR_ALARMS_STATUS,
   MENU_LVL2_IR_FOCAL_LENGTH,
   MENU_LVL2_IR_FOCUS,
   MENU_LVL2_IR_SENSITIVITY,
   MENU_LVL2_IR_TEMP_STATUS,
   MENU_LVL2_IR_TEMP_PROC,
   MENU_LVL2_IR_SPATIAL_PROC,
   MENU_LVL2_IR_EZOOM,
   MENU_LVL2_IR_CALIBRATION_SEL,
   MENU_LVL2_IR_LEVEL,
   MENU_LVL2_IR_SCENE_SETUP,
   MENU_LVL2_IR_GATE,
   MENU_LVL2_IR_POLARITY,
   MENU_LVL2_IR_CAMERA_EXTENDER,
   MENU_LVL2_IR_PSEUDO_COLOR,
   MENU_LVL2_IR_THRESHOLD,
   MENU_LVL2_IR_CAMERA_POWER,
   MENU_LVL2_IR_DESIRED_MODE,
   MENU_LVL2_IR_CALIBRATION,
   MENU_LVL2_IR_MISC,
   MENU_LVL2_IR_ALARMS,
   MENU_LVL2_IR_BIT,
   MENU_LVL2_IR_DONE,

   MENU_LVL2_RF_MODE,
   MENU_LVL2_RF_ALARMS_STATUS,
   MENU_LVL2_RF_RANGE_MODE,
   MENU_LVL2_RF_LASER_STATE,
   MENU_LVL2_RF_TARGET_FOUND,
   MENU_LVL2_RF_RANGE,
   MENU_LVL2_RF_TEMP_STATUS,
   MENU_LVL2_RF_TARGET,
   MENU_LVL2_RF_MIN_RANGE,
   MENU_LVL2_RF_MAX_RANGE,
   MENU_LVL2_RF_ALARMS,
   MENU_LVL2_RF_BIT,
   MENU_LVL2_RF_DONE,

   MENU_LVL2_AVT_MODE,
   MENU_LVL2_AVT_ALARMS_STATUS,
   MENU_LVL2_AVT_TRACKING_STATUS,
   MENU_LVL2_AVT_ALGORITHM,
   MENU_LVL2_AVT_GATE_TYPE,
   MENU_LVL2_AVT_GATE_SIZE,
   MENU_LVL2_AVT_COAST_TIMEOUT,
   MENU_LVL2_AVT_ALARMS,
   MENU_LVL2_AVT_BIT,
   MENU_LVL2_AVT_DONE,

   MENU_LVL2_MTI_MODE,
   MENU_LVL2_MTI_ALARMS_STATUS,
   MENU_LVL2_MTI_PROCESSING,
   MENU_LVL2_MTI_MARITIME_MODE,
   MENU_LVL2_MTI_LAND_SETTINGS,
   MENU_LVL2_MTI_LAND_SENSITIVITY,
   MENU_LVL2_MTI_LAND_BOX_COLOR,
   MENU_LVL2_MTI_BREADCRUMBS,
   MENU_LVL2_MTI_MIN_BREADCRUMBS,
   MENU_LVL2_MTI_MAX_BREADCRUMBS,
   MENU_LVL2_MTI_MAX_BREADCRUMBS_LIFESPAN,
   MENU_LVL2_MTI_MARITIME_SETTINGS,
   MENU_LVL2_MTI_MARITIME_SENSITIVITY,
   MENU_LVL2_MTI_MARITIME_BOX_COLOR,
   MENU_LVL2_MTI_LIFEJACKET_DETECTION,
   MENU_LVL2_MTI_LIFEJACKET_SENSITIVITY,
   MENU_LVL2_MTI_ALARMS,
   MENU_LVL2_MTI_FACTORY_DEFAULT,
   MENU_LVL2_MTI_BIT,
   MENU_LVL2_MTI_DONE,

   MENU_LVL2_ILLUM_TEMPERATURE,
   MENU_LVL2_ILLUM_STATE,
   MENU_LVL2_ILLUM_MAX_USAGE,
   MENU_LVL2_ILLUM_AUTO_LOW_MED,
   MENU_LVL2_ILLUM_AUTO_MED_HIGH,
   MENU_LVL2_ILLUM_MODE,
   MENU_LVL2_ILLUM_DONE,

   MENU_LVL2_LD_MODE,
   MENU_LVL2_LD_ALARMS_STATUS,
   MENU_LVL2_LD_POSITION,
   MENU_LVL2_LD_BORESIGHT_INSTALLED,
   MENU_LVL2_LD_1_57_MODE,
   MENU_LVL2_LD_LAST_BORESIGHT,
   MENU_LVL2_LD_TOTAL_BORESIGHTS,
   MENU_LVL2_LD_TARGET,
   MENU_LVL2_LD_MIN_RANGE,
   MENU_LVL2_LD_MAX_RANGE,
   MENU_LVL2_LD_CODE1_ENTRY,
   MENU_LVL2_LD_CODE2_ENTRY,
   MENU_LVL2_LD_CODE3_ENTRY,
   MENU_LVL2_LD_CODE4_ENTRY,
   MENU_LVL2_LD_CODE5_ENTRY,
   MENU_LVL2_LD_SELECT_CODE,
   MENU_LVL2_LD_BATTLE_OVERRIDE,
   MENU_LVL2_LD_MISC,
   MENU_LVL2_LD_ALARMS,
   MENU_LVL2_LD_BIT,
   MENU_LVL2_LD_DONE,

   MENU_LVL2_IF_MODE,
   MENU_LVL2_IF_BLEND_MODE,
   MENU_LVL2_IF_MANUAL_BLEND,
   MENU_LVL2_IF_COMBINATION,
   MENU_LVL2_IF_FACTORY_DEFAULT,
   MENU_LVL2_IF_DONE,

   MENU_LVL2_SS_LAST_CALIBRATION,
   MENU_LVL2_SS_LAST_CALIBRATION_IR_NFOV,
   MENU_LVL2_SS_LAST_CALIBRATION_IR_VNFOV,
   MENU_LVL2_SS_MODE,
   MENU_LVL2_SS_INITIAL_COMBINATION,
   MENU_LVL2_SS_COMBINATION,
   MENU_LVL2_SS_SPOT_COLOR,
   MENU_LVL2_SS_FLASH_POLARITY,
   MENU_LVL2_SS_CALIBRATE,
   MENU_LVL2_SS_FACTORY_DEFAULT,
   MENU_LVL2_SS_DONE,

   MENU_COUNT,
};

// constant strings to match eIresMenu enum
extern const char* IresMenuStr[];

enum eIresFieldType
{
   FIELD_NONE,
   FIELD_ACTION,
   FIELD_ACTION_BOT,
   FIELD_STATUS,
   FIELD_SINGLE_SELECTION,
   FIELD_LIST_SELECTION,
   FIELD_RANGE_SELECTION,
   FIELD_OPERATOR_DEFINE,
   FIELD_BLANK,
   FIELD_LINE,

   FIELD_COUNT
};

enum eIresDataType
{
   IRES_NONE,
   IRES_MENU,
   IRES_CLOSE_MENU,
   IRES_B8,
   IRES_U8,
   IRES_U16,
   IRES_U32,
   IRES_U64,
   IRES_I8,
   IRES_I16,
   IRES_I32,
   IRES_I64,
   IRES_F32,
   IRES_F64,
   IRES_STRING,

   IRES_DATA_TYPE_COUNT
};

enum eIresMenuAppearance
{
   APPEARANCE_NONE = 0,

   // indentation 3 bits for up to 7 indentation levels
   // only one of these should be or'd with the other flags
   APPEARANCE_INDENT_ONE     = 0x00000001,
   APPEARANCE_INDENT_TWO     = 0x00000002,
   APPEARANCE_INDENT_THREE   = 0x00000003,
   APPEARANCE_INDENT_FOUR    = 0x00000004,
   APPEARANCE_INDENT_FIVE    = 0x00000005,
   APPEARANCE_INDENT_SIX     = 0x00000006,
   APPEARANCE_INDENT_SEVEN   = 0x00000007,

   APPEARANCE_LINE_ABOVE     = 0x00000008,
   APPEARANCE_LINE_BELOW     = 0x00000010,
   APPEARANCE_LINE_LEFT      = 0x00000020,
   APPEARANCE_LINE_RIGHT     = 0x00000040,

   APPEARANCE_BOLD           = 0x00000080,
   APPEARANCE_HIGHLIGHT_LINE = 0x00000100,
};

union TIresData
{
   uint8_t Value;
   void*   Ptr;
};

struct TIresMenuItem
{
   eIresMenu           Id;
   eIresFieldType      Type;
   eIresDataType       DataType;
   eIresMenuAppearance Appearance;
   uint8_t             Index;
   uint32_t            Min;
   uint32_t            Max;
   uint32_t            Step;
   TIresData           Data;
   TIresData           PrevData;
};

struct TIresMenu
{
   char                       Title[MAX_MENU_NAME_SIZE];
   uint8_t                    CursorLocation;
   std::vector<TIresMenuItem> Items;
};

enum eIresMenuPdu
{
   MENU_INVALID_PDU,
   MENU_CONFIG_PDU = 1000,
   MENU_PDU,
   MENU_ITEM_PDU,
   MENU_CURSOR_PDU,
};

enum eIresCursor : uint8_t
{
   CURSOR_NONE,
   CURSOR_CARAT,
   CURSOR_DIAMOND,
   CURSOR_HIGHLIGHT,
   CURSOR_BOLD,

   CURSOR_COUNT
};

enum eIresMenuFont : uint8_t
{
   FONT_ARIAL,
   FONT_BOLD,
   FONT_LATO_REGULAR,
   FONT_LATO_BOLD,
   FONT_OPENSANS_REGULAR,
   FONT_OPENSANS_BOLD,
   FONT_LIBERATION_MONO_REGULAR,
   FONT_LIBERATION_MONO_BOLD,

   FONT_COUNT
};

// constant strings to match eIresMenuFont enum
extern const char* IresMenuFontStr[];

#pragma pack(1)
struct TIresMenuHeader
{
   uint16_t Type;
   uint16_t Length;
};

struct TIresMenuColor
{
   uint8_t Red;
   uint8_t Green;
   uint8_t Blue;
   uint8_t Alpha;
};

struct TIresMenuConfigPdu
{
   uint8_t        MenuActive;
   uint8_t        LineWidth;
   uint8_t        FontSize;
   uint8_t        Font;
   int32_t        X;
   int32_t        Y;
   int32_t        Width;
   int32_t        Height;
   TIresMenuColor TextColor;
   TIresMenuColor BackgroundColor;
   TIresMenuColor LineColor;
};

struct TIresMenuPdu
{
   int32_t X;
   int32_t Y;
   int32_t Width;
   int32_t Height;
   uint8_t NumMenuItems;
   uint8_t CursorLocation;
   bool    CloseMenu;
   // char* Title (variable sized, null-terminated)
   // TIresMenuItemPdu Items[] (variable sized, number of items)
};

struct TIresMenuItemPdu
{
   uint16_t Id;
   uint16_t Type;
   uint8_t  DataType;
   uint8_t  Index;
   uint32_t Appearance;
   // variable data packed based on DataType
};

struct TIresMenuCursorPdu
{
   uint8_t CursorType;
   uint8_t CursorLocation;
};

#pragma pack()

#endif 
