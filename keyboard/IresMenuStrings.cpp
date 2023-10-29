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
// Title: IRES Interface Menu Types String array definitions
// Class: C++ Source
// Filename: IresMenuStrings.cpp
// Author: Brian Woodard
// Purpose: This module performs the following tasks:
//
// Provides string arrays used by the CIresMenu class.
//
// NOTE: This file needs to match between the Host and Symbology DLL code!
//
//-----------------------------------------------------------------------------

#include "IresMenuTypes.h"

// constant strings to match eIresMenu enum
const char* IresMenuStr[] =
{
   "None",

   // Level 1
   "GIMBAL",
   "EOW",
   "IR",
   "RANGE FINDER",
   "AVT",
   "MOVING TARGET INDICATOR",
   "ILLUM",
   "LD",
   "VIDEO I/O",
   "IMAGE FUSION",
   "SEE SPOT",
   "OPERATOR GIMBAL",
   "OPERATOR PAYLOAD",
   "MISSION",
   "DIGITAL OVERLAYS",
   "ANALOG OVERLAYS",
   "STEERING MODES",
   "PROFILES",
   "MISC",
   "BORESIGHT",
   "ALARMS",
   "SYSTEM BIT",
   "DONE",

   // Level 2 Gimbal
   "MODE",
   "ALARMS",
   "PROCESSOR TEMP",
   "HUMIDITY",
   "VOLTAGE",
   "AZ STAB",
   "EL STAB",
   "AZ",
   "EL",
   "ALARMS",
   "BIT",
   "DONE",

   // Level 2 EOW
   "MODE",
   "ALARMS",
   "FOCAL LENGTH",
   "FOCUS",
   "SENSITIVITY",
   "TEMP STATUS",
   "TEMP PROC",
   "SPATIAL PROC",
   "EZOOM",
   "LEVEL",
   "SCENE SETUP",
   "GATE",
   "CAMERA EXTENDER",
   "MISC",
   "ALARMS",
   "BIT",
   "DONE",

   // Level 2 IR
   "MODE",
   "ALARMS",
   "FOCAL LENGTH",
   "FOCUS",
   "SENSITIVITY",
   "TEMP STATUS",
   "TEMP PROC",
   "SPATIAL PROC",
   "EZOOM",
   "CALIBRATION",
   "LEVEL",
   "SCENE SETUP",
   "GATE",
   "POLARITY",
   "CAMERA EXTENDER",
   "PSEUDO COLOR",
   "THRESHOLD",
   "CAMERA POWER",
   "DESIRED MODE",
   "CALIBRATION",
   "MISC",
   "ALARMS",
   "BIT",
   "DONE",

   // Level 2 Range Finder
   "MODE",
   "ALARMS",
   "RANGE MODE",
   "LASER STATE",
   "TARGET FOUND",
   "RANGE",
   "TEMP STATUS",
   "TARGET",
   "MIN RANGE [m]",
   "MAX RANGE [m]",
   "ALARMS",
   "BIT",
   "DONE",

   // Level 2 Advanced Video Tracker
   "MODE",
   "ALARMS",
   "TRACKING STATUS",
   "ALGORITHM",
   "GATE TYPE",
   "GATE SIZE",
   "COAST TIMEOUT",
   "ALARMS",
   "BIT",
   "DONE",

   // Level 2 Moving Target Indicator
   "MODE",
   "ALARMS",
   "MTI PROCESSING",
   "MODE",
   "LAND SETTINGS",
   "SENSITIVITY",
   "BOX COLOR",
   "BREADCRUMBS",
   "MIN BREADCRUMBS",
   "MAX BREADCRUMBS",
   "MAX BREADCRUMB LIFESPAN",
   "MARITIME SETTINGS",
   "SENSITIVITY",
   "BOX COLOR",
   "LIFEJACKET DETECTION",
   "LIFEJACKET SENSITIVITY",
   "ALARMS",
   "FACTORY DFLT",
   "BIT",
   "DONE",

   // Level 2 Laser Illuminator
   "TEMPERATURE",
   "STATE",
   "MAX USAGE [MINS]",
   "AUTO LOW/MED RANGE [M]",
   "AUTO MED/HIGH RANGE [M]",
   "MODE",
   "DONE",

   // Level 2 Laser Designator
   "MODE",
   "ALARMS",
   "POSITION",
   "BORESIGHT INSTALLED",
   "1.57 MODE",
   "LAST BORESIGHT",
   "TOTAL BORESIGHTS",
   "TARGET",
   "MIN RANGE [m]",
   "MAX RANGE [m]",
   "1ST CODE ENTRY",
   "2ND CODE ENTRY",
   "3RD CODE ENTRY",
   "4TH CODE ENTRY",
   "5TH CODE ENTRY",
   "SELECT CODE",
   "BATTLE OVERRIDE",
   "MISC",
   "ALARMS",
   "BIT",
   "DONE",

   "MODE",
   "BLEND MODE",
   "MANUAL BLEND",
   "COMBINATION",
   "FACTORY DFLT",
   "DONE",

   "LAST CALIBRATION",
   "IR : SW NFOV",
   "IR : SW VNFOV",
   "MODE",
   "INITIAL COMBINATION",
   "COMBINATION",
   "SPOT COLOR",
   "FLASH POLARITY",
   "CALIBRATE",
   "FACTORY DFLT",
   "DONE",

};

// constant strings to match eIresMenuFont enum
const char* IresMenuFontStr[] =
{
   "fonts//arial.ttf",
   "fonts//arialbd.ttf",
   "fonts//Lato-Regular.ttf",
   "fonts//Lato-Bold.ttf",
   "fonts//OpenSans-Regular.ttf",
   "fonts//OpenSans-Bold.ttf",
   "fonts//LiberationMono-Regular.ttf",
   "fonts//LiberationMono-Bold.ttf"
};
