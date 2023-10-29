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
//! Class:      CPP Header
//! Filename:   IresMenu.h
//! Author:     Brian Woodard
//! Purpose:    Draws the menus for the L3 WESCAM MX-10 IRES
//
//-----------------------------------------------------------------------------

#pragma once

#include <glm/glm.hpp>
#include "IresMenuTypes.h"
#include "Line.h"
#include "CText.h"

class CIresMenu
{
public:

   struct TIresMenuData
   {
      glm::vec3                  Position;
      glm::vec3                  Size;
      char                       Title[MAX_MENU_NAME_SIZE];
      std::vector<TIresMenuItem> Items;
   };

   CIresMenu(glm::mat4& Projection);
   virtual ~CIresMenu();

   void Draw();
   void ProcessPdu(char* Buffer, int Size);

private:

   float DrawActionItem(const TIresMenuItem& Item, float X, float Y, float Width, float FontSize, bool Bold);
   float DrawSelectionItem(const TIresMenuItem& Item, float X, float Y, float Width, float FontSize, bool Bold);
   float DrawStatusItem(const TIresMenuItem& Item, float X, float Y, float Width, float FontSize, bool Bold);
   void DrawBackground(bool BackgroundOnly = false);
   void DrawMenuBorder(TIresMenuData* Menu);
   void DrawMenuItems(const std::vector<TIresMenuItem>& Items, glm::vec3 Start, float Width, bool Bottom);
   int UnpackMenuItemPdu(const TIresMenuItemPdu* ItemPdu, TIresMenuItem& Item);

   TIresMenuConfigPdu         mConfig;
   TIresMenuCursorPdu         mCursor;
   std::vector<TIresMenuData> mMenuStack;
   CLine                      mLine;
   CLine                      mBackground;
   CLine                      mPolygon;
   CText*                     mText;
   CText*                     mBoldText;
   glm::mat4                  mProjection;
   glm::vec4                  mTextColor;
   glm::vec4                  mLineColor;
   glm::vec4                  mBackgroundColor;
   bool                       mClipEnabled;

};


