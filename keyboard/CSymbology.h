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
//  � Veraxx Engineering Corporation, 2022.  All rights reserved.
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
//! Title:      Tss Symbology DLL
//! Class:      CPP Header
//! Filename:   CSymbology.cpp
//! Author:     JP Scheff
//! Purpose:    Symbology for the TSS/Britestar Sensor
//
//-----------------------------------------------------------------------------

//
#pragma once

#include <iostream>
#include <fstream>

#include "Symbology.h"
#include "Line.h"
#include "CText.h"
#include "CImage.h"

using namespace std;

// Only allow symbol and word IDs from 0 to 255 so we can use 1-byte in
// the host interface to identify these objects.
#define MAX_SYMBOL_ID 255
#define MAX_WORD_ID 255

#define TVC 1
#define FLIR 0
#define TWOPI 6.2831256
#define PI 3.14159265359

#define SOCKET_BUFFER_SIZE 2000

// ----------------------------------------------------------
// CSymbology class declaration
//

static float mBsHeadingDegPerTick = 0.00220f;

class CSymbology
{

public:
   CSymbology(glm::mat4 &projection, glm::vec3 &windowSize, unsigned int size,
              const std::string &imgFile);

   virtual ~CSymbology();

   static const float HOST_COORD_X_MAX;
   static const float HOST_COORD_Y_MAX;

   float mLeftCoordLimit;
   float mRightCoordLimit;
   float mTopCoordLimit;
   float mBottomCoordLimit;

   // char  mFontFile[256];
   // char  mIpAddress[256];
   // short mPort;
   // bool  mInitialized;

   // char  mSocketBuffer[SOCKET_BUFFER_SIZE];

   T_FromHostToSymbologyAH *FromHostAH;
   T_FromHostToSymbologyUH *FromHostUH;

   int lpvid = 0;
   int vidcnt = 0;
   float so = 0;

   inline float TO_X_PIXEL(float x)
   {
      float ret = x * (mRightCoordLimit - mLeftCoordLimit) / HOST_COORD_X_MAX;
      return ret + mLeftCoordLimit;
   }

   inline float TO_Y_PIXEL(float y)
   {
      float ret = y * (mTopCoordLimit - mBottomCoordLimit) / HOST_COORD_Y_MAX;
      return ret + mBottomCoordLimit;
   }

   void DrawPrimaryLockedTargetReticle(void);
   void DrawStatusMsg(void);
   void DrawStandardLOSReticle(void);
   void DrawPotentialTargetRecticule(void);
   void DrawLaserIlluminatorReticle(void);
   void DrawLaserDesignatorReticle(void);
   void DrawNextZoomMarkerGraphic(void);
   void DrawStandardNorthIndicator(void);
   void DrawCtrArrowNortIndicator(void);
   void DrawCtrLineNortIndicator(void);
   void DrawPointStyleLOSReticle(void);
   void DrawAquisitionGateReticle(void);
   void DrawLaserangeFinderReticle(void);
   void DrawAzimuthValues(void);
   void DrawAzimuthPointer(void);
   void DrawElevationValues(void);
   void DrawElevationPointer(void);
   void DrawElevationPictograph(void);
   void DrawAzimuthPictograph(void);
   void DrawBSCompassTape(void);
   void DrawScaleBarReticle(void);

   void UpdateHost(T_FromHostToSymbologyUH *_FromHostUH);
   void UpdateHost(T_FromHostToSymbologyAH *_FromHostAH);

   // for testing
   // void GetVerticies(float& x, float& y, short idx);
   void GetTLSection(float &_X, float &_Y);
   void GetMLSection(float &_X, float &_Y);
   void GetBLSection(float &_X, float &_Y);
   void GetTCSection(float &_X, float &_Y);
   void GetMCSection(float &_X, float &_Y);
   void GetBCSection(float &_X, float &_Y);
   void GetRTSection(float &_X, float &_Y);
   void GetRMSection(float &_X, float &_Y);
   void GetRBSection(float &_X, float &_Y);

protected:
   void PrintText(std::string &_string);
   float GetTextHeight(std::string &msg);
   float GetTextWidth(std::string &msg);
   void DrawLine(vec3 &start, vec3 &end, bool compat = false);
   void UpdatePoints(float &xin, float &yin, int idx, int size);
   void DrawTopPictograph(float mx, float my);

private:
   bool mPositionInit;
   float mMaxPos_x;
   float mMaxPos_y;
   float mMaxNeg_x;
   float mMaxNeg_y;
   float mInfoBoxMaxWidth;
   float mInfoBoxMaxHeight;
   float mYseparation;
   float mXseparation;
   int mWidth;
   int mHeight;
   glm::mat4 mProjection;
   bool mCompatibility = false;
   unsigned int mFontSize;
   CText mText;
   CLine mLine;
};
