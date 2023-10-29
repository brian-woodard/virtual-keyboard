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
// Title: IRES Interface Types String array definitions
// Class: C++ Source
// Filename: IresTypesStrings.cpp
// Author: Brian Woodard
// Purpose: This module performs the following tasks:
//
// Provides string arrays used by all IRES modules.
//
//-----------------------------------------------------------------------------

#include "IresTypes.h"

// constant strings to match eIresAlarm enum
const char* IresAlarmStr[] =
{
   "OK",
   "MINOR",
   "MAJOR",
   "UNKNOWN"
};

// constant strings to match eIresMode enum
const char* IresModeStr[] =
{
   "STARTUP",
   "BIT",
   "OPERATIONAL",
   "MAINTENANCE",
   "FAILED",
   "STOWED",
   "UNKNOWN"
};

// constant strings to match eAvtTrackingStatus enum
const char* AvtTrackingStatusStr[] =
{
   "IDLE",
   "INIT ACQN",
   "ACQUIRING",
   "COASTING",
   "LOCKED",
   "XXX"
};

// constant strings to match eAvtAlgorithm enum
const char* AvtAlgorithmStr[] =
{
   "CENT",
   "COR-A",
   "COR-M",
   "SCENE",
   "MAN",
   "AUTO",
   "XXX"
};

// constant strings to match eAvtGateType enum
const char* AvtGateTypeStr[] =
{
   "PRESET",
   "DYNAMIC",
   "XXX"
};

// constant strings to match eAvtGateSize enum
const char* AvtGateSizeStr[] =
{
   "SMALL",
   "MEDIUM",
   "LARGE",
   "XXX"
};

// constant strings to match eMtiBoxColor enum
const char* MtiBoxColorStr[] =
{
   "WHITE",
   "GREEN",
   "BLUE",
   "YELLOW",
   "MAGENTA",
   "CYAN",
   "XXX"
};

// constant strings to match eIllumState enum
const char* IllumStateStr[] =
{
   "DISARM",
   "ARM",
   "LASE",
   "CUTOUT",
   "XXX"
};

// constant strings to match eIllumMode enum
const char* IllumModeStr[] =
{
   "OFF",
   "LOW",
   "HIGH",
   "PULSE",
   "ALIGN",
   "XXX"
};

// constant strings to match eFusionCombination enum
const char* FusionCombinationStr[] =
{
   "IR : EO-DL",
   "IR : EO-LL",
   "IR : EO-SW",
   "EO-DL : EO-SW",
   "EO-LL : EON-SW",
   "XXX"
};

// constant strings to match eSeeSpotCombination enum
const char* SeeSpotCombinationStr[] =
{
   "ALL",
   "IR : SW",
   "EOW : SW",
   "XXX"
};

// constant strings to match eSeeSpotColor enum
const char* SeeSpotColorStr[] =
{
   "WHITE",
   "RED",
   "GREEN",
   "XXX"
};
