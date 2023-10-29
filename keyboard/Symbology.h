//-----------------------------------------------------------------------------
//                            UNCLASSIFIED
//-----------------------------------------------------------------------------
//                    DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
//  This software and the accompanying documentation are provided to the U.S.
//  Government with unlimited rights as provided in DFARS § 252.227-7014.  The
//  contractor, Veraxx Engineering Corporation, retains ownership, the
//  copyrights, and all other rights.
//
//  © Veraxx Engineering Corporation, 2022.  All rights reserved.
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
//! Title:      Executive Datatype Header
//! Class:      C++ Header
//! Filename:   Symbology.h
//! Author:     Mike Gilroy
//! Purpose:    Prototype declarations of MACCE Entry Point C functions
//
//-----------------------------------------------------------------------------

#ifndef _SYMBOLOGY__H_
#define _SYMBOLOGY__H_

typedef struct
{
   // Symbology Display Mode
   unsigned char display_symbology;

   // TSS Grey Scale
   unsigned char grey_scale[2];

   // TSS Focus Labels
   int focus_shade;
   float focus_sym_pos;

   // TSS Reticle
   int reticle_shade;
   unsigned char los_reticle;
   float los_x_pos;
   float los_y_pos;
   int yardstick_mode; // -1, no display, 0 cannot be computed, 1 = exceeds 999m,  2 estimated, 3 laser
   float yardstick_dist;

   // TSS Offset Reticle
   unsigned char offset_track;
   float offset_x;
   float offset_y;

   // TSS Symbology control
   float symbology_brightness;
   unsigned char symbology_declutter;

   // TSS Track symbols
   int track_symbol[3];
   int track_symbol_rate[3];
   float track_x_pos[3];
   float track_y_pos[3];
   float track_x_size_centroid[3];
   float track_y_size_centroid[3];

   // TSS Control

   float azimuth;
   float elevation;
   int fov; //  0=wide,1=med,2=narrow,3=n2
   unsigned char fov_zoom;
   float zoom[2];
   unsigned char video_type;
   unsigned char boresight_bsr;
   unsigned char boresight_status_tv;
   unsigned char boresight_status_flir;
   unsigned char boresight_status_flir_vnfov;
   unsigned char transfer_align;
   unsigned char FOM;

   // TSS Status Msg
   int status_msg; //  0 no msg, 1 FLIR CALIBRATION IN PROGRESS
                   //  2 FLIR CALIBRATION COMPLETE
                   //  3 FLIR CALIBRATION ABORTED
                   // TSS Cooldown status
   int cooldown_status;

} T_FromHostToSymbologyAH;

typedef struct
{

   double mt_los_lat;
   double mt_los_lon;
   unsigned int gray_scale_on;
   unsigned int reticle_shade;
   unsigned int graticule_shade;
   unsigned int fov_bracket_shade;
   unsigned int FOV;
   unsigned int heartbeat;
   float azimuth;
   float elevation;
   float mt_box_x_pos;
   float mt_box_x_size;
   float mt_box_y_pos;
   float mt_box_y_size;
   float mt_los_alt;
   float heading;
   int menu_count;
   int menu_font[5];
   int menu_font_size[5];
   int menu_line_count[5];
   int menu_list[5];
   int menu_shade;
   int menu_text_length[5];
   int menu_top_x[5];
   int menu_top_y[5];
   int menu_reverse_start[5];
   int menu_reverse_end[5];
   int menu_underline_start[5];
   int menu_underline_end[5];
   int mt_box_shade;
   int mt_box_status;
   int top_bar_length;
   int top_reverse_start;
   int top_reverse_end;
   int bottom_bar_length;
   char menu_text[80][20][5];
   char top_bar_text[100];
   char bottom_bar_text[100];
} T_FromHostToSymbologyUH;

#endif
