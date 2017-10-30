/* ====================================================================
 * File: LayoutManagerValues.h
 * Created: 03/15/09
 * Modified: 05/03/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef LAYOUTMANAGERVALUES_H
#define LAYOUTMANAGERVALUES_H

// INCLUDE FILES
#include <avkon.hrh>

/////////////////////////////////////////////////////////////////////////////
//
// SUPPORTED UI RESOLUTIONS ARE:
//
// QVGA RESOLUTION PORTRAIT     240x320 ( Quarter VGA )
// QVGA RESOLUTION LANDSCAPE    320x240 ( Quarter VGA )
// qHD  RESOLUTION PORTRAIT     360x640 ( Quarter High Definition )
//
////////////////////////////////////////////////////////////////////////////

// Supported screen resolutions
enum TResolution
    {
    EQvgaPortrait,
    EQvgaLandscape,
    EQhdPortrait,
    // EQhdLandscape, <-- Tetris2 doesn't support landscape on qHD device
    ENotSupported
    };

// Supported screen orientation
enum TOrientation
    {
    EAutomatic,
    EPortrait,
    ELandscape
    };

// Component enumerations
enum TTe2Component
    {
    EBackground,
    ESoftkey1,
    ESoftkey2,
    EMainMenu,
    EScroller,
    EPrimaryMatrix,
    ESecondaryMatrix,
    EScorekeeper,
    ERankingTitle,
    ERankingList
    };

/////////////////////////////////////////////////////////////////////////////
//
// TETRIS2 COMPONENTS VALUES
//
//  * LAYOUT RECT
//  * TEXT FONT
//  * TEXT BASELINE
//
/////////////////////////////////////////////////////////////////////////////


// Screen
static const TInt KQvgaPortraitWidth = 240;
static const TInt KQvgaPortraitHeight = 320;

static const TInt KQvgaLandscapeWidth = 320;
static const TInt KQvgaLandscapeHeight = 240;

static const TInt KQhdPortraitWidth = 360;
static const TInt KQhdPortraitHeight = 640;

static const TInt KQhdLandscapeWidth = 640;
static const TInt KQhdLandscapeHeight = 360;

// Background
static const TInt KQvgaPrtBgLeft = 0;
static const TInt KQvgaPrtBgTop = 0;
static const TInt KQvgaPrtBgRight = 240;
static const TInt KQvgaPrtBgBottom = 320;
static const TInt KQvgaPrtBgWidth = 240;
static const TInt KQvgaPrtBgHeight = 320;

static const TInt KQvgaLscBgLeft = 0;
static const TInt KQvgaLscBgTop = 0;
static const TInt KQvgaLscBgRight = 320;
static const TInt KQvgaLscBgBottom = 240;
static const TInt KQvgaLscBgWidth = 320;
static const TInt KQvgaLscBgHeight = 240;

static const TInt KQhdPrtBgLeft = 0;
static const TInt KQhdPrtBgTop = 0;
static const TInt KQhdPrtBgRight = 360;
static const TInt KQhdPrtBgBottom = 640;
static const TInt KQhdPrtBgWidth = 360;
static const TInt KQhdPrtBgHeight = 640;


// Softkey1
static const TInt KQvgaPrtSk1Left = 0;
static const TInt KQvgaPrtSk1Top = 293;
static const TInt KQvgaPrtSk1Right = 100;
static const TInt KQvgaPrtSk1Bottom = 320;
static const TInt KQvgaPrtSk1Width = 100;
static const TInt KQvgaPrtSk1Height = 27;

static const TInt KQvgaLscSk1Left = 0;
static const TInt KQvgaLscSk1Top = 213;
static const TInt KQvgaLscSk1Right = 100;
static const TInt KQvgaLscSk1Bottom = 240;
static const TInt KQvgaLscSk1Width = 100;
static const TInt KQvgaLscSk1Height = 27;

static const TInt KQhdPrtSk1Left = 5;
static const TInt KQhdPrtSk1Top = 600;
static const TInt KQhdPrtSk1Right = 125;
static const TInt KQhdPrtSk1Bottom = 640;
static const TInt KQhdPrtSk1Width = 120;
static const TInt KQhdPrtSk1Height = 40;

// Softkey2
static const TInt KQvgaPrtSk2Left = 140;
static const TInt KQvgaPrtSk2Top = 293;
static const TInt KQvgaPrtSk2Right = 240;
static const TInt KQvgaPrtSk2Bottom = 320;
static const TInt KQvgaPrtSk2Width = 100;
static const TInt KQvgaPrtSk2Height = 27;

static const TInt KQvgaLscSk2Left = 220;
static const TInt KQvgaLscSk2Top = 213;
static const TInt KQvgaLscSk2Right = 320;
static const TInt KQvgaLscSk2Bottom = 240;
static const TInt KQvgaLscSk2Width = 100;
static const TInt KQvgaLscSk2Height = 27;

static const TInt KQhdPrtSk2Left = 240;
static const TInt KQhdPrtSk2Top = 595;
static const TInt KQhdPrtSk2Right = 360;
static const TInt KQhdPrtSk2Bottom = 635;
static const TInt KQhdPrtSk2Width = 120;
static const TInt KQhdPrtSk2Height = 40;

// MainMenu
static const TInt KQvgaPrtMMLeft = 60;
static const TInt KQvgaPrtMMTop = 100;
static const TInt KQvgaPrtMMRight = 180;
static const TInt KQvgaPrtMMBottom = 200;
static const TInt KQvgaPrtMMWidth = 120;
static const TInt KQvgaPrtMMHeight = 100;

static const TInt KQvgaLscMMLeft = 100;
static const TInt KQvgaLscMMTop = 70;
static const TInt KQvgaLscMMRight = 220;
static const TInt KQvgaLscMMBottom = 170;
static const TInt KQvgaLscMMWidth = 120;
static const TInt KQvgaLscMMHeight = 100;

static const TInt KQhdPrtMMLeft = 90;
static const TInt KQhdPrtMMTop = 210;
static const TInt KQhdPrtMMRight = 270;
static const TInt KQhdPrtMMBottom = 390;
static const TInt KQhdPrtMMWidth = 180;
static const TInt KQhdPrtMMHeight = 180;

// Scroller
static const TInt KQvgaPrtScrollerLeft = 60;
static const TInt KQvgaPrtScrollerTop = 80;
static const TInt KQvgaPrtScrollerRight = 180;
static const TInt KQvgaPrtScrollerBottom = 200;
static const TInt KQvgaPrtScrollerWidth = 120;
static const TInt KQvgaPrtScrollerHeight = 120;

static const TInt KQvgaLscScrollerLeft = 100;
static const TInt KQvgaLscScrollerTop = 70;
static const TInt KQvgaLscScrollerRight = 220;
static const TInt KQvgaLscScrollerBottom = 170;
static const TInt KQvgaLscScrollerWidth = 120;
static const TInt KQvgaLscScrollerHeight = 100;

static const TInt KQhdPrtScrollerLeft = 90;
static const TInt KQhdPrtScrollerTop = 240;
static const TInt KQhdPrtScrollerRight = 270;
static const TInt KQhdPrtScrollerBottom = 400;
static const TInt KQhdPrtScrollerWidth = 180;
static const TInt KQhdPrtScrollerHeight = 160;

// Primary Matrix
static const TInt KQvgaPrtPMLeft = 5;
static const TInt KQvgaPrtPMTop = 5;
static const TInt KQvgaPrtPMRight = 145;
static const TInt KQvgaPrtPMBottom = 285;
static const TInt KQvgaPrtPMWidth = 140;
static const TInt KQvgaPrtPMHeight = 280;

static const TInt KQvgaLscPMLeft = 20;
static const TInt KQvgaLscPMTop = 20;
static const TInt KQvgaLscPMRight = 120;
static const TInt KQvgaLscPMBottom = 220;
static const TInt KQvgaLscPMWidth = 100;
static const TInt KQvgaLscPMHeight = 200;

static const TInt KQhdPrtPMLeft = 5;
static const TInt KQhdPrtPMTop = 70;
static const TInt KQhdPrtPMRight = 265;
static const TInt KQhdPrtPMBottom = 550;
static const TInt KQhdPrtPMWidth = 240;
static const TInt KQhdPrtPMHeight = 500;

// Secondary Matrix
static const TInt KQvgaPrtSMLeft = 155;
static const TInt KQvgaPrtSMTop = 70;
static const TInt KQvgaPrtSMRight = 211;
static const TInt KQvgaPrtSMBottom = 126;
static const TInt KQvgaPrtSMWidth = 56;
static const TInt KQvgaPrtSMHeight = 56;

static const TInt KQvgaLscSMLeft = 150;
static const TInt KQvgaLscSMTop = 30;
static const TInt KQvgaLscSMRight = 190;
static const TInt KQvgaLscSMBottom = 70;
static const TInt KQvgaLscSMWidth = 40;
static const TInt KQvgaLscSMHeight = 40;

static const TInt KQhdPrtSMLeft = 270;
static const TInt KQhdPrtSMTop = 130;
static const TInt KQhdPrtSMRight = 350;
static const TInt KQhdPrtSMBottom = 210;
static const TInt KQhdPrtSMWidth = 80;
static const TInt KQhdPrtSMHeight = 80;

// Scorekeeper
static const TInt KQvgaPrtScorekeeperLeft = 155;
static const TInt KQvgaPrtScorekeeperTop = 136;
static const TInt KQvgaPrtScorekeeperRight = 235;
static const TInt KQvgaPrtScorekeeperBottom = 236;
static const TInt KQvgaPrtScorekeeperWidth = 80;
static const TInt KQvgaPrtScorekeeperHeight = 100;

static const TInt KQvgaLscScorekeeperLeft = 160;
static const TInt KQvgaLscScorekeeperTop = 80;
static const TInt KQvgaLscScorekeeperRight = 240;
static const TInt KQvgaLscScorekeeperBottom = 180;
static const TInt KQvgaLscScorekeeperWidth = 80;
static const TInt KQvgaLscScorekeeperHeight = 100;

static const TInt KQhdPrtScorekeeperLeft = 250;
static const TInt KQhdPrtScorekeeperTop = 240;
static const TInt KQhdPrtScorekeeperRight = 355;
static const TInt KQhdPrtScorekeeperBottom = 380;
static const TInt KQhdPrtScorekeeperWidth = 105;
static const TInt KQhdPrtScorekeeperHeight = 140;

// Ranking title
static const TInt KQvgaPrtRankingTitleLeft = 40;
static const TInt KQvgaPrtRankingTitleTop = 80;
static const TInt KQvgaPrtRankingTitleRight = 220;
static const TInt KQvgaPrtRankingTitleBottom = 100;
static const TInt KQvgaPrtRankingTitleWidth = 180;
static const TInt KQvgaPrtRankingTitleHeight = 20;

static const TInt KQvgaLscRankingTitleLeft = 80;
static const TInt KQvgaLscRankingTitleTop = 50;
static const TInt KQvgaLscRankingTitleRight = 260;
static const TInt KQvgaLscRankingTitleBottom = 70;
static const TInt KQvgaLscRankingTitleWidth = 180;
static const TInt KQvgaLscRankingTitleHeight = 20;

static const TInt KQhdPrtRankingTitleLeft = 90;
static const TInt KQhdPrtRankingTitleTop = 180;
static const TInt KQhdPrtRankingTitleRight = 270;
static const TInt KQhdPrtRankingTitleBottom = 200;
static const TInt KQhdPrtRankingTitleWidth = 180;
static const TInt KQhdPrtRankingTitleHeight = 20;

// Ranking list
static const TInt KQvgaPrtRankingListLeft = 40;
static const TInt KQvgaPrtRankingListTop = 110;
static const TInt KQvgaPrtRankingListRight = 220;
static const TInt KQvgaPrtRankingListBottom = 210;
static const TInt KQvgaPrtRankingListWidth = 180;
static const TInt KQvgaPrtRankingListHeight = 100;

static const TInt KQvgaLscRankingListLeft = 80;
static const TInt KQvgaLscRankingListTop = 80;
static const TInt KQvgaLscRankingListRight = 260;
static const TInt KQvgaLscRankingListBottom = 180;
static const TInt KQvgaLscRankingListWidth = 160;
static const TInt KQvgaLscRankingListHeight = 100;

static const TInt KQhdPrtRankingListLeft = 90;
static const TInt KQhdPrtRankingListTop = 210;
static const TInt KQhdPrtRankingListRight = 310;
static const TInt KQhdPrtRankingListBottom = 300;
static const TInt KQhdPrtRankingListWidth = 220;
static const TInt KQhdPrtRankingListHeight = 90;

#endif // LAYOUTMANAGERVALUES_H

// End of File
