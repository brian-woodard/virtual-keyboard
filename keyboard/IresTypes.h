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
// Title: IRES Interface Types
// Class: C++ Header
// Filename: IresTypes.h
// Author: Brian Woodard
// Purpose: This module performs the following tasks:
//
// Provides types used by all IRES modules.
//
//-----------------------------------------------------------------------------

#ifndef IRESTYPES__H
#define IRESTYPES__H

#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))

enum eIresAlarm
{
   ALARM_OK,
   ALARM_MINOR,
   ALARM_MAJOR,
   ALARM_UNKNOWN,

   ALARM_COUNT
};

// constant strings to match eIresAlarm enum
extern const char* IresAlarmStr[];

enum eIresMode
{
   MODE_STARTUP,
   MODE_BIT,
   MODE_OPERATIONAL,
   MODE_MAINTENANCE,
   MODE_FAILED,
   MODE_STOWED,
   MODE_UNKNOWN,

   MODE_COUNT
};

// constant strings to match eIresMode enum
extern const char* IresModeStr[];

///////////////////////////////////////////////////////////////////////////////
// AVT types and enums
///////////////////////////////////////////////////////////////////////////////
enum eAvtTrackingStatus
{
   AVT_TRACK_IDLE,
   AVT_TRACK_INITIATE_ACQUISITION,
   AVT_TRACK_ACQUIRING,
   AVT_TRACK_COASTING,
   AVT_TRACK_LOCKED,

   AVT_TRACK_COUNT
};

// constant strings to match eAvtTrackingStatus enum
extern const char* AvtTrackingStatusStr[];

enum eAvtAlgorithm
{
   AVT_ALG_CENTROID,
   AVT_ALG_COR_A,
   AVT_ALG_COR_M,
   AVT_ALG_SCENE,
   AVT_ALG_MAN,
   AVT_ALG_AUTO,

   AVT_ALG_COUNT
};

// constant strings to match eAvtAlgorithm enum
extern const char* AvtAlgorithmStr[];

enum eAvtGateType
{
   AVT_GATE_TYPE_PRESET,
   AVT_GATE_TYPE_DYNAMIC,

   AVT_GATE_TYPE_COUNT
};

// constant strings to match eAvtGateType enum
extern const char* AvtGateTypeStr[];

enum eAvtGateSize
{
   AVT_GATE_SIZE_SMALL,
   AVT_GATE_SIZE_MEDIUM,
   AVT_GATE_SIZE_LARGE,

   AVT_GATE_SIZE_COUNT
};

// constant strings to match eAvtGateSize enum
extern const char* AvtGateSizeStr[];

///////////////////////////////////////////////////////////////////////////////
// MTI types and enums
///////////////////////////////////////////////////////////////////////////////
enum eMtiBoxColor
{
   MTI_COLOR_WHITE,
   MTI_COLOR_GREEN,
   MTI_COLOR_BLUE,
   MTI_COLOR_YELLOW,
   MTI_COLOR_MAGENTA,
   MTI_COLOR_CYAN,

   MTI_COLOR_COUNT
};

// constant strings to match eMtiBoxColor enum
extern const char* MtiBoxColorStr[];

///////////////////////////////////////////////////////////////////////////////
// Laser Illuminator types and enums
///////////////////////////////////////////////////////////////////////////////
enum eIllumState
{
   ILLUM_DISARMED,
   ILLUM_ARMED,
   ILLUM_LASING,
   ILLUM_CUTOUT,

   ILLUM_COUNT
};

// constant strings to match eIllumState enum
extern const char* IllumStateStr[];

enum eIllumMode
{
   ILLUM_MODE_OFF,
   ILLUM_MODE_LOW,
   ILLUM_MODE_HIGH,
   ILLUM_MODE_PULSE,
   ILLUM_MODE_ALIGN,

   ILLUM_MODE_COUNT
};

// constant strings to match eIllumMode enum
extern const char* IllumModeStr[];

///////////////////////////////////////////////////////////////////////////////
// Image Fusion types and enums
///////////////////////////////////////////////////////////////////////////////
enum eFusionCombination
{
   FUSE_COMBO_IR_EODL,
   FUSE_COMBO_IR_EOLL,
   FUSE_COMBO_IR_EOSW,
   FUSE_COMBO_EODL_EOSW,
   FUSE_COMBO_EOLL_EONSW,

   FUSE_COMBO_COUNT
};

// constant strings to match eFusionCombination enum
extern const char* FusionCombinationStr[];

///////////////////////////////////////////////////////////////////////////////
// See Spot types and enums
///////////////////////////////////////////////////////////////////////////////
enum eSeeSpotCombination
{
   SEESPOT_COMBO_ALL,
   SEESPOT_COMBO_IR_SW,
   SEESPOT_COMBO_EOW_SW,

   SEESPOT_COMBO_COUNT
};

// constant strings to match eSeeSpotCombination enum
extern const char* SeeSpotCombinationStr[];

enum eSeeSpotColor
{
   SEESPOT_COLOR_WHITE,
   SEESPOT_COLOR_RED,
   SEESPOT_COLOR_GREEN,

   SEESPOT_COLOR_COUNT
};

// constant strings to match eSeeSpotColor enum
extern const char* SeeSpotColorStr[];

#endif 
