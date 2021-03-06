//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "stdafx.h"
#include "OwmScite.h"
#include "Parameters.h"
//#include "FileDialog.h"
#include "ScitillaComponent/ScintillaEditView.h"
#include <shlobj.h>

#include "keys.h"

struct WinMenuKeyDefinition {	//more or less matches accelerator table definition, easy copy/paste
	//const TCHAR * name;	//name retrieved from menu?
	int vKey;
	int functionId;
	bool isCtrl;
	bool isAlt;
	bool isShift;
	TCHAR * specialName;		//Used when no real menu name exists (in case of toggle for example)
};


struct ScintillaKeyDefinition {
	const TCHAR * name;
	int functionId;
	bool isCtrl;
	bool isAlt;
	bool isShift;
	int vKey;
	int redirFunctionId;	//this gets set  when a function is being redirected through Notepad++ if Scintilla doesnt do it properly :)
};

WinMenuKeyDefinition winKeyDefs[] = {	//array of accelerator keys for all std menu items, values can be 0 for vKey, which means its unused
/*	{VK_N,		IDM_FILE_NEW,			 			true,  false, false, NULL},
	{VK_O,		IDM_FILE_OPEN,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_RELOAD,					false, false, false, NULL},
	{VK_S,		IDM_FILE_SAVE,						true,  false, false, NULL},
	{VK_S,		IDM_FILE_SAVEAS,					true,  true,  false, NULL},
	{VK_NULL,	IDM_FILE_SAVECOPYAS,				false, false, false, NULL},
	{VK_S,		IDM_FILE_SAVEALL,					true,  false, true,  NULL},
	{VK_W,	 	IDM_FILE_CLOSE,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_CLOSEALL,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_CLOSEALL_BUT_CURRENT,		false, false, false, NULL},
	{VK_NULL,	IDM_FILE_DELETE,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_RENAME,					false, false, false, NULL},
	{VK_NULL,	IDM_FILE_LOADSESSION,				false, false, false, NULL},
	{VK_NULL,	IDM_FILE_SAVESESSION,				false, false, false, NULL},
	{VK_P,		IDM_FILE_PRINT,						true,  false, false, NULL},
	{VK_NULL,	IDM_FILE_PRINTNOW,					false, false, false, NULL},
	{VK_F4,		IDM_FILE_EXIT,						false, true,  false, NULL},
*/
//	{VK_NULL,	IDM_EDIT_UNDO,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_REDO,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_CUT,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_COPY,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_PASTE,		 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_DELETE,	 				false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_SELECTALL,					false, false, false, NULL},

/*	{VK_NULL,	IDM_EDIT_SETREADONLY,				false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_CLEARREADONLY,				false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_FULLPATHTOCLIP,			false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_FILENAMETOCLIP,			false, false, false, NULL},
	{VK_NULL,	IDM_EDIT_CURRENTDIRTOCLIP,			false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_INS_TAB,					false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_RMV_TAB,					false, false, false, NULL},
//	{VK_NULL,	IDM_EDIT_DUP_LINE,					false, false, false, NULL},
	{VK_I,		IDM_EDIT_SPLIT_LINES,		 		true,  false, false, NULL},
	{VK_J,		IDM_EDIT_JOIN_LINES,		 		true,  false, false, NULL},
	{VK_UP,		IDM_EDIT_LINE_UP,					true,  false, true,  NULL},
	{VK_DOWN,	IDM_EDIT_LINE_DOWN,					true,  false, true,  NULL},
	{VK_NULL,	IDM_EDIT_TRIMTRAILING,		 		false, false, false, NULL},
	{VK_C,		IDM_EDIT_COLUMNMODE,				false, true,  false, NULL},
	{VK_U, 		IDM_EDIT_UPPERCASE,					true,  false, true,  NULL},
	{VK_U, 		IDM_EDIT_LOWERCASE,					true,  false, false, NULL},
	{VK_Q,		IDM_EDIT_BLOCK_COMMENT,				true,  false, false, NULL},
	{VK_K,		IDM_EDIT_BLOCK_COMMENT_SET,			true,  false, false, NULL},
	{VK_K,		IDM_EDIT_BLOCK_UNCOMMENT,			true,  false, true,  NULL},
	{VK_Q,		IDM_EDIT_STREAM_COMMENT, 			true,  false, true,  NULL},
	{VK_SPACE,	IDM_EDIT_AUTOCOMPLETE,				true,  false, false, NULL},
	{VK_RETURN,	IDM_EDIT_AUTOCOMPLETE_CURRENTFILE,	true,  false, false, NULL},
	{VK_SPACE,	IDM_EDIT_FUNCCALLTIP,				true,  false, true,  NULL},
	{VK_R,		IDM_EDIT_RTL,						true,  true,  false, NULL},
	{VK_L,		IDM_EDIT_LTR,						true,  true,  false, NULL},

	{VK_F,		IDM_SEARCH_FIND,					true,  false, false, NULL},
	{VK_F,		IDM_SEARCH_FINDINFILES,				true,  false, true,  NULL},
	{VK_F3,		IDM_SEARCH_FINDNEXT,				false, false, false, NULL},
	{VK_F3,		IDM_SEARCH_FINDPREV,				false, false, true,  NULL},
	{VK_F3,		IDM_SEARCH_VOLATILE_FINDNEXT,		true,  false, false, NULL},
	{VK_F3,		IDM_SEARCH_VOLATILE_FINDPREV,		true,  false, true,  NULL},
	{VK_F4,		NPPM_INTERNAL_SEARCH_GOTONEXTFOUND,	false, false, false, TEXT("Goto next found result")},
	{VK_F4,		NPPM_INTERNAL_SEARCH_GOTOPREVFOUND,	false, false, true,  TEXT("Goto previous found result")},
	{VK_M,		IDM_SEARCH_MARKALL,					true,  false, false, NULL},
	{VK_M,		IDM_SEARCH_UNMARKALL,				true,  false, true,  NULL},
	{VK_H,		IDM_SEARCH_REPLACE,					true,  false, false, NULL},
	{VK_I,		IDM_SEARCH_FINDINCREMENT,			true,  true,  false, NULL},
	{VK_G,		IDM_SEARCH_GOTOLINE,		 		true,  false, false, NULL},
	{VK_B,		IDM_SEARCH_GOTOMATCHINGBRACE,		true,  false, false, NULL},
	{VK_F2,		IDM_SEARCH_TOGGLE_BOOKMARK,			true,  false, false, NULL},
	{VK_F2,		IDM_SEARCH_NEXT_BOOKMARK, 			false, false, false, NULL},
	{VK_F2,		IDM_SEARCH_PREV_BOOKMARK, 			false, false, true,  NULL},
	{VK_NULL,	IDM_SEARCH_CLEAR_BOOKMARKS, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_CUTMARKEDLINES, 			false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_COPYMARKEDLINES, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_PASTEMARKEDLINES, 		false, false, false, NULL},
	{VK_NULL,	IDM_SEARCH_DELETEMARKEDLINES, 		false, false, false, NULL},
	{VK_F7,		NPPM_INTERNAL_FOCUS_ON_FOUND_RESULTS,false, false, false, TEXT("Switch to found results window")},

	{VK_F11,	IDM_VIEW_FULLSCREENTOGGLE,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_ALWAYSONTOP,				false, false, false, NULL},
	{VK_F12,	IDM_VIEW_POSTIT,					false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_TAB_SPACE,					false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_EOL,						false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_ALL_CHARACTERS,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_INDENT_GUIDE,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_WRAP,						false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_WRAP_SYMBOL,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_USER_DLG,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMIN,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMOUT,					false, false, false, NULL},
	//{VK_NULL,	IDM_VIEW_ZOOMRESTORE,				false, false, false, NULL},
	{VK_0,		IDM_VIEW_TOGGLE_FOLDALL, 			false, true,  false, NULL},
	{VK_F,		IDM_VIEW_FOLD_CURRENT,				true,  true,  false, NULL},
	{VK_1,		IDM_VIEW_FOLD_1, 					false, true,  false, NULL},
	{VK_2,		IDM_VIEW_FOLD_2, 					false, true,  false, NULL},
	{VK_3,		IDM_VIEW_FOLD_3, 					false, true,  false, NULL},
	{VK_4,		IDM_VIEW_FOLD_4, 					false, true,  false, NULL},
	{VK_5,		IDM_VIEW_FOLD_5, 					false, true,  false, NULL},
	{VK_6,		IDM_VIEW_FOLD_6, 					false, true,  false, NULL},
	{VK_7,		IDM_VIEW_FOLD_7, 					false, true,  false, NULL},
	{VK_8,		IDM_VIEW_FOLD_8, 					false, true,  false, NULL},
	{VK_F,		IDM_VIEW_UNFOLD_CURRENT,			true,  true,  true,  NULL},
	{VK_1,		IDM_VIEW_UNFOLD_1,					false, true,  true,  NULL},
	{VK_2,		IDM_VIEW_UNFOLD_2,					false, true,  true,  NULL},
	{VK_3,		IDM_VIEW_UNFOLD_3,					false, true,  true,  NULL},
	{VK_4,		IDM_VIEW_UNFOLD_4,					false, true,  true,  NULL},
	{VK_5,		IDM_VIEW_UNFOLD_5,					false, true,  true,  NULL},
	{VK_6,		IDM_VIEW_UNFOLD_6,					false, true,  true,  NULL},
	{VK_7,		IDM_VIEW_UNFOLD_7,					false, true,  true,  NULL},
	{VK_8,		IDM_VIEW_UNFOLD_8,					false, true,  true,  NULL},
	{VK_0,		IDM_VIEW_TOGGLE_UNFOLDALL,			false, true,  true,  NULL},
	{VK_H,		IDM_VIEW_HIDELINES,					false, true,  false, NULL},
	{VK_NULL,	IDM_VIEW_GOTO_ANOTHER_VIEW,			false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_CLONE_TO_ANOTHER_VIEW,		false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_SYNSCROLLV,				false, false, false, NULL},
	{VK_NULL,	IDM_VIEW_SYNSCROLLH,				false, false, false, NULL},
	{VK_F8,		IDM_VIEW_SWITCHTO_OTHER_VIEW,		false, false, false, NULL},

	{VK_NULL, 	IDM_FORMAT_TODOS,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_TOUNIX,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_TOMAC,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_ANSI,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UTF_8,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_AS_UTF_8,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UCS_2BE,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_UCS_2LE,					false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_ANSI,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_AS_UTF_8,			false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UTF_8,				false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UCS_2BE,			false, false, false, NULL},
	{VK_NULL, 	IDM_FORMAT_CONV2_UCS_2LE,			false, false, false, NULL},

	{VK_NULL, 	IDM_SETTING_PREFERECE,				false, false, false, NULL},
	{VK_NULL, 	IDM_LANGSTYLE_CONFIG_DLG,			false, false, false, NULL},
	{VK_NULL, 	IDM_SETTING_SHORTCUT_MAPPER,		false, false, false, NULL},

	{VK_R,		IDC_EDIT_TOGGLEMACRORECORDING,		true,  false, true,  TEXT("Toggle macro record")},
	{VK_P,		IDM_MACRO_PLAYBACKRECORDEDMACRO, 	true,  false, true,  NULL},
	{VK_NULL,	IDM_MACRO_SAVECURRENTMACRO, 		false, false, false, NULL},
	{VK_NULL,	IDM_MACRO_RUNMULTIMACRODLG, 		false, false, false, NULL},

	{VK_F5,		IDM_EXECUTE,						false, false, false, NULL},

	{VK_NULL,	IDM_HOMESWEETHOME, 					false, false, false, NULL},
	{VK_NULL,	IDM_PROJECTPAGE, 					false, false, false, NULL},
	{VK_NULL,	IDM_ONLINEHELP, 					false, false, false, NULL},
	{VK_NULL,	IDM_FORUM, 							false, false, false, NULL},
	{VK_NULL,	IDM_PLUGINSHOME, 					false, false, false, NULL},
	{VK_F1,		IDM_ABOUT, 							false, false, false, NULL},
*/	{VK_F1,		ID_WORD_HELP, 						false, false, true, NULL},
/*
	{VK_TAB,	IDC_PREV_DOC,						true,  false, true, TEXT("Switch to previous document")},
	{VK_TAB,	IDC_NEXT_DOC,						true,  false, false, TEXT("Switch to next document")},
*/
};


ScintillaKeyDefinition scintKeyDefs[] = {	//array of accelerator keys for all possible scintilla functions, values can be 0 for vKey, which means its unused
	{TEXT("SCI_CUT"),						SCI_CUT,					true,  false, false, VK_X, 		ID_EDIT_CUT},
	{TEXT(""),							SCI_CUT,					false, false, true,  VK_DELETE, 0},
	{TEXT("SCI_COPY"),					SCI_COPY,					true,  false, false, VK_C, 		ID_EDIT_COPY},
	{TEXT(""),							SCI_COPY,					true,  false, false, VK_INSERT, 0},
	{TEXT("SCI_PASTE"),					SCI_PASTE,					true,  false, false, VK_V, 		ID_EDIT_PASTE},
	{TEXT(""),							SCI_PASTE,					false, false, true,  VK_INSERT, 0},
	{TEXT("SCI_SELECTALL"),				SCI_SELECTALL,				true,  false, false, VK_A, 		ID_EDIT_SELECT_ALL},
	{TEXT("SCI_CLEAR"),					SCI_CLEAR,					false, false, false, VK_DELETE, ID_EDIT_DELETE},
	{TEXT("SCI_CLEARALL"),				SCI_CLEARALL,				false, false, false, 0,			0},
	{TEXT("SCI_UNDO"),					SCI_UNDO,					true,  false, false, VK_Z, 		ID_EDIT_UNDO},
	{TEXT(""),							SCI_UNDO,					false, true,  false, VK_BACK, 	0},
	{TEXT("SCI_REDO"),					SCI_REDO,					true,  false, false, VK_Y, 		ID_EDIT_REDO},
	{TEXT("SCI_NEWLINE"),					SCI_NEWLINE,				false, false, false, VK_RETURN, 0},
	{TEXT(""),							SCI_NEWLINE,				false, false, true,  VK_RETURN, 0},
	//{TEXT("SCI_TAB"),						SCI_TAB,					false, false, false, VK_TAB,	ID_EDIT_INS_TAB},
	//{TEXT("SCI_BACKTAB"),					SCI_BACKTAB,				false, false, true,  VK_TAB,	ID_EDIT_RMV_TAB},
	{TEXT("SCI_FORMFEED"),				SCI_FORMFEED,				false, false, false, 0,			0},
	//{TEXT("SCI_ZOOMIN"),					SCI_ZOOMIN,					true,  false, false, VK_ADD, 	ID_VIEW_ZOOMIN},
	//{TEXT("SCI_ZOOMOUT"),					SCI_ZOOMOUT,				true,  false, false, VK_SUBTRACT,ID_VIEW_ZOOMOUT},
	//{TEXT("SCI_SETZOOM"),					SCI_SETZOOM,				true,  false, false, VK_DIVIDE,	ID_VIEW_ZOOMRESTORE},
	//{TEXT("SCI_SELECTIONDUPLICATE"),		SCI_SELECTIONDUPLICATE,		true,  false, false, VK_D, 		ID_EDIT_DUP_LINE},
	{TEXT("SCI_LINESJOIN"),				SCI_LINESJOIN,				false, false, false, 0,			0},
	{TEXT("SCI_SCROLLCARET"),				SCI_SCROLLCARET,			false, false, false, 0,			0},
	{TEXT("SCI_EDITTOGGLEOVERTYPE"),		SCI_EDITTOGGLEOVERTYPE,		false, false, false, VK_INSERT, 0},
	{TEXT("SCI_MOVECARETINSIDEVIEW"),		SCI_MOVECARETINSIDEVIEW,	false, false, false, 0,			0},
	{TEXT("SCI_LINEDOWN"),				SCI_LINEDOWN,				false, false, false, VK_DOWN,	0},
	{TEXT("SCI_LINEDOWNEXTEND"),			SCI_LINEDOWNEXTEND,			false, false, true,  VK_DOWN,	0},
	{TEXT("SCI_LINEDOWNRECTEXTEND"),		SCI_LINEDOWNRECTEXTEND,		false, true,  true,  VK_DOWN,	0},
	{TEXT("SCI_LINESCROLLDOWN"),			SCI_LINESCROLLDOWN,			true,  false, false, VK_DOWN,	0},
	{TEXT("SCI_LINEUP"),					SCI_LINEUP,					false, false, false, VK_UP,		0},
	{TEXT("SCI_LINEUPEXTEND"),			SCI_LINEUPEXTEND,			false, false, true,  VK_UP,		0},
	{TEXT("SCI_LINEUPRECTEXTEND"),		SCI_LINEUPRECTEXTEND,		false, true,  true,  VK_UP,		0},
	{TEXT("SCI_LINESCROLLUP"),			SCI_LINESCROLLUP,			true,  false, false, VK_UP,		0},
	{TEXT("SCI_PARADOWN"),				SCI_PARADOWN,				true,  false, false, VK_OEM_6,	0},
	{TEXT("SCI_PARADOWNEXTEND"),			SCI_PARADOWNEXTEND,			true,  false, true,  VK_OEM_6,	0},
	{TEXT("SCI_PARAUP"),					SCI_PARAUP,					true,  false, false, VK_OEM_4,	0},
	{TEXT("SCI_PARAUPEXTEND"),			SCI_PARAUPEXTEND,			true,  false, true,  VK_OEM_4,	0},
	{TEXT("SCI_CHARLEFT"),				SCI_CHARLEFT,				false, false, false, VK_LEFT,	0},
	{TEXT("SCI_CHARLEFTEXTEND"),			SCI_CHARLEFTEXTEND,			false, false, true,  VK_LEFT,	0},
	{TEXT("SCI_CHARLEFTRECTEXTEND"),		SCI_CHARLEFTRECTEXTEND,		false, true,  true,  VK_LEFT,	0},
	{TEXT("SCI_CHARRIGHT"),				SCI_CHARRIGHT,				false, false, false, VK_RIGHT,	0},
	{TEXT("SCI_CHARRIGHTEXTEND"),			SCI_CHARRIGHTEXTEND,		false, false, true,  VK_RIGHT,	0},
	{TEXT("SCI_CHARRIGHTRECTEXTEND"),		SCI_CHARRIGHTRECTEXTEND,	false, true,  true,  VK_RIGHT,	0},
	{TEXT("SCI_WORDLEFT"),				SCI_WORDLEFT,				true,  false, false, VK_LEFT,	0},
	{TEXT("SCI_WORDLEFTEXTEND"),			SCI_WORDLEFTEXTEND,			true,  false, true,  VK_LEFT,	0},
	{TEXT("SCI_WORDRIGHT"),				SCI_WORDRIGHT,				true,  false, false, VK_RIGHT,	0},
	{TEXT("SCI_WORDRIGHTEXTEND"),			SCI_WORDRIGHTEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_WORDLEFTEND"),				SCI_WORDLEFTEND,			false, false, false, 0,			0},
	{TEXT("SCI_WORDLEFTENDEXTEND"),		SCI_WORDLEFTENDEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_WORDRIGHTEND"),			SCI_WORDRIGHTEND,			false, false, false, 0,			0},
	{TEXT("SCI_WORDRIGHTENDEXTEND"),		SCI_WORDRIGHTENDEXTEND,		true,  false, true,  VK_RIGHT,	0},
	{TEXT("SCI_WORDPARTLEFT"),			SCI_WORDPARTLEFT,			true,  false, false, VK_OEM_2,	0},
	{TEXT("SCI_WORDPARTLEFTEXTEND"),		SCI_WORDPARTLEFTEXTEND,		true,  false, true,  VK_OEM_2,	0},
	{TEXT("SCI_WORDPARTRIGHT"),			SCI_WORDPARTRIGHT,			true,  false, false, VK_OEM_5,	0},
	{TEXT("SCI_WORDPARTRIGHTEXTEND"),		SCI_WORDPARTRIGHTEXTEND,	true,  false, true,  VK_OEM_5,	0},
	{TEXT("SCI_HOME"),					SCI_HOME,					false, false, false, 0,			0},
	{TEXT("SCI_HOMEEXTEND"),				SCI_HOMEEXTEND,				false, false, false, 0,			0},
	{TEXT("SCI_HOMERECTEXTEND"),			SCI_HOMERECTEXTEND,			false, false, false, 0,			0},
	{TEXT("SCI_HOMEDISPLAY"),				SCI_HOMEDISPLAY,			false, true,  false, VK_HOME, 	0},
	{TEXT("SCI_HOMEDISPLAYEXTEND"),		SCI_HOMEDISPLAYEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_HOMEWRAP"),				SCI_HOMEWRAP,				false, false, false, 0,			0},
	{TEXT("SCI_HOMEWRAPEXTEND"),			SCI_HOMEWRAPEXTEND,			false, false, false, 0,			0},
	{TEXT("SCI_VCHOME"),					SCI_VCHOME,					false, false, false, 0,			0},
	{TEXT("SCI_VCHOMEEXTEND"),			SCI_VCHOMEEXTEND,			false, false, true,  VK_HOME, 	0},
	{TEXT("SCI_VCHOMERECTEXTEND"),		SCI_VCHOMERECTEXTEND,		false, true,  true,  VK_HOME,	0},
	{TEXT("SCI_VCHOMEWRAP"),				SCI_VCHOMEWRAP,				false, false, false, VK_HOME,	0},
	{TEXT("SCI_VCHOMEWRAPEXTEND"),		SCI_VCHOMEWRAPEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_LINEEND"),					SCI_LINEEND,				false, false, false, 0,			0},
	{TEXT("SCI_LINEENDEXTEND"),			SCI_LINEENDEXTEND,			false, false, true,  VK_END,	0},
	{TEXT("SCI_LINEENDRECTEXTEND"),		SCI_LINEENDRECTEXTEND,		false, true,  true,  VK_END,	0},
	{TEXT("SCI_LINEENDDISPLAY"),			SCI_LINEENDDISPLAY,			false, true,  false, VK_END, 	0},
	{TEXT("SCI_LINEENDDISPLAYEXTEND"), 	SCI_LINEENDDISPLAYEXTEND,	false, false, false, 0,			0},
	{TEXT("SCI_LINEENDWRAP"),				SCI_LINEENDWRAP,			false, false, false, VK_END,	0},
	{TEXT("SCI_LINEENDWRAPEXTEND"),		SCI_LINEENDWRAPEXTEND,		false, false, false, 0,			0},
	{TEXT("SCI_DOCUMENTSTART"),			SCI_DOCUMENTSTART,			true,  false, false, VK_HOME, 	0},
	{TEXT("SCI_DOCUMENTSTARTEXTEND"),		SCI_DOCUMENTSTARTEXTEND,	true,  false, true,  VK_HOME, 	0},
	{TEXT("SCI_DOCUMENTEND"),				SCI_DOCUMENTEND,			true,  false, false, VK_END, 	0},
	{TEXT("SCI_DOCUMENTENDEXTEND"),		SCI_DOCUMENTENDEXTEND,		true,  false, true,  VK_END, 	0},
	{TEXT("SCI_PAGEUP"),					SCI_PAGEUP,					false, false, false, VK_PRIOR,	0},
	{TEXT("SCI_PAGEUPEXTEND"),			SCI_PAGEUPEXTEND,			false, false, true,  VK_PRIOR,	0},
	{TEXT("SCI_PAGEUPRECTEXTEND"),		SCI_PAGEUPRECTEXTEND,		false, true,  true,  VK_PRIOR,	0},
	{TEXT("SCI_PAGEDOWN"),				SCI_PAGEDOWN,				false, false, false, VK_NEXT, 	0},
	{TEXT("SCI_PAGEDOWNEXTEND"),			SCI_PAGEDOWNEXTEND,			false, false, true,  VK_NEXT, 	0},
	{TEXT("SCI_PAGEDOWNRECTEXTEND"),		SCI_PAGEDOWNRECTEXTEND,		false, true,  true,  VK_NEXT,	0},
	{TEXT("SCI_STUTTEREDPAGEUP"),			SCI_STUTTEREDPAGEUP,		false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEUPEXTEND"),	SCI_STUTTEREDPAGEUPEXTEND,	false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEDOWN"),		SCI_STUTTEREDPAGEDOWN,		false, false, false, 0,			0},
	{TEXT("SCI_STUTTEREDPAGEDOWNEXTEND"), SCI_STUTTEREDPAGEDOWNEXTEND,false, false, false, 0,			0},
	{TEXT("SCI_DELETEBACK"),				SCI_DELETEBACK,				false, false, false, VK_BACK,	0},
	{TEXT(""),							SCI_DELETEBACK,				false, false, true,  VK_BACK,	0},
	{TEXT("SCI_DELETEBACKNOTLINE"),		SCI_DELETEBACKNOTLINE,		false, false, false, 0,			0},
	{TEXT("SCI_DELWORDLEFT"),				SCI_DELWORDLEFT,			true,  false, false, VK_BACK,	0},
	{TEXT("SCI_DELWORDRIGHT"),			SCI_DELWORDRIGHT,			true,  false, false, VK_DELETE, 0},
	{TEXT("SCI_DELLINELEFT"),				SCI_DELLINELEFT,			true,  false, true,  VK_BACK,	0},
	{TEXT("SCI_DELLINERIGHT"),			SCI_DELLINERIGHT,			true,  false, true,  VK_DELETE,	0},
	{TEXT("SCI_LINEDELETE"),				SCI_LINEDELETE,				true,  false, true,  VK_L, 		0},
	{TEXT("SCI_LINECUT"),					SCI_LINECUT,				true,  false, false, VK_L, 		0},
	{TEXT("SCI_LINECOPY"),				SCI_LINECOPY,				true,  false, true,  VK_T, 		0},
	{TEXT("SCI_LINETRANSPOSE"),			SCI_LINETRANSPOSE,			true,  false, false, VK_T, 		0},
	{TEXT("SCI_LINEDUPLICATE"),			SCI_LINEDUPLICATE,			false, false, false, 0,			0},
	{TEXT("SCI_CANCEL"),					SCI_CANCEL,					false, false, false, VK_ESCAPE, 0}
	//{TEXT("SCI_EMPTYUNDOBUFFER"),		SCI_EMPTYUNDOBUFFER,		false, false, false, 0,			0},
	//{TEXT("SCI_TOGGLECARETSTICKY"),		SCI_TOGGLECARETSTICKY,		false, false, false, 0,			0},
	//{TEXT("SCI_CALLTIPCANCEL"),			SCI_CALLTIPCANCEL,			false, false, false, 0,			0},
	//{TEXT("SCI_SETSAVEPOINT"),			SCI_SETSAVEPOINT,			false, false, false, 0,			0},
	//{TEXT("SCI_CLEARDOCUMENTSTYLE"),	SCI_CLEARDOCUMENTSTYLE,		false, false, false, 0,			0},
	//
	//
	//{TEXT("SCI_CHOOSECARETX"),			SCI_CHOOSECARETX,			false, false, false, 0,			0},
	//{TEXT("SCI_AUTOCCOMPLETE"),			SCI_AUTOCCOMPLETE,			false, false, false, 0,			0},
	//{TEXT("SCI_AUTOCCANCEL"),			SCI_AUTOCCANCEL,			false, false, false, 0,			0},
	//{TEXT("SCI_CLEARREGISTEREDIMAGES"), SCI_CLEARREGISTEREDIMAGES,	false, false, false, 0,			0},
	//{TEXT("SCI_HOMEDISPLAYEXTEND"),		SCI_HOMEDISPLAYEXTEND,		false, true,  true,  VK_HOME,	0},
	//{TEXT("SCI_LINEENDDISPLAYEXTEND"),	SCI_LINEENDDISPLAYEXTEND,	false, true,  true,  VK_END,	0},
	//
	//{TEXT("SCI_DELWORDRIGHTEND"),		SCI_DELWORDRIGHTEND,		false, false, false, 0,			0},
	//{TEXT("SCI_LOWERCASE"),				SCI_LOWERCASE,				false, false, false, 0,			0},
	//{TEXT("SCI_UPPERCASE"),				SCI_UPPERCASE,				false, false, false, 0,			0},
	//{TEXT("SCI_LOWERCASE"),				SCI_LOWERCASE,				true,  false, false, VK_U, 		0},
	//{TEXT("SCI_UPPERCASE"),				SCI_UPPERCASE,				true,  false, true,  VK_U, 		0},
	//
	//{TEXT("SCI_FORMFEED"),				SCI_FORMFEED,				true,  false, false, VK_L, 		0},
	//{TEXT("SCI_CLEARALLCMDKEYS"),		SCI_CLEARALLCMDKEYS,		false, false, false, 0,			0},
	//{TEXT("SCI_STARTRECORD"),			SCI_STARTRECORD,			false, false, false, 0,			0},
	//{TEXT("SCI_STOPRECORD"),			SCI_STOPRECORD,				false, false, false, 0,			0},
	//{TEXT("SCI_SEARCHANCHOR"),			SCI_SEARCHANCHOR,			false, false, false, 0,			0},
	//{TEXT("SCI_TARGETFROMSELECTION"),	SCI_TARGETFROMSELECTION,	false, false, false, 0,			0},
	//{TEXT("SCI_STYLERESETDEFAULT"),		SCI_STYLERESETDEFAULT,		false, false, false, 0,			0},
	//{TEXT("SCI_STYLECLEARALL"),			SCI_STYLECLEARALL,			false, false, false, 0,			0},
	//
};
#ifdef UNICODE
#include "localizationString.h"

LocalizationSwicher::LocalizationSwicher() 
{
	TCHAR userPath[MAX_PATH];

	// Make localConf.xml path
	TCHAR localConfPath[MAX_PATH];
	TCHAR nppPath[MAX_PATH];
	//::GetModuleFileName(NULL, nppPath, MAX_PATH);
	//PathRemoveFileSpec(nppPath);

	CString strOwmPath = theApp.GetModulePath();
	lstrcpy(_nppPath, strOwmPath);

	lstrcpy(localConfPath, nppPath);
	PathAppend(localConfPath, localConfFile);

	// Test if localConf.xml exist
	bool isLocal = (PathFileExists(localConfPath) == TRUE);

	if (isLocal)
	{
		lstrcpy(userPath, nppPath);
	}
	else
	{
		ITEMIDLIST *pidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		SHGetPathFromIDList(pidl, userPath);
		if(theApp.m_pIPlatUI)
		{
			PathAppend(userPath, theApp.m_pIPlatUI->GetAppName());
		}else
		{
			PathAppend(userPath, TEXT("OwmScite"));
		}
	}

	TCHAR nativeLangPath[MAX_PATH];
	lstrcpy(nativeLangPath, userPath);
	PathAppend(nativeLangPath, TEXT("nativeLang.xml"));

	_nativeLangPath = nativeLangPath;
}

wstring LocalizationSwicher::getLangFromXmlFileName(wchar_t *fn) const
{
	size_t nbItem = sizeof(localizationDefs)/sizeof(LocalizationSwicher::LocalizationDefinition);
	for (size_t i = 0 ; i < nbItem ; i++)
	{
		if (wcsicmp(fn, localizationDefs[i]._xmlFileName) == 0)
			return localizationDefs[i]._langName;
	}
	return TEXT("");
}

wstring LocalizationSwicher::getXmlFilePathFromLangName(wchar_t *langName) const
{
	for (size_t i = 0 ; i < _localizationList.size() ; i++)
	{
		if (wcsicmp(langName, _localizationList[i].first.c_str()) == 0)
			return _localizationList[i].second;
	}
	return TEXT("");
}

bool LocalizationSwicher::addLanguageFromXml(wstring xmlFullPath)
{
	wchar_t * fn = ::PathFindFileNameW(xmlFullPath.c_str());
	wstring foundLang = getLangFromXmlFileName(fn);
	if (foundLang != TEXT(""))
	{
		_localizationList.push_back(pair<wstring, wstring>(foundLang ,xmlFullPath));
		return true;
	}
	return false;
}

bool LocalizationSwicher::switchToLang(wchar_t *lang2switch) const
{
	wstring langPath = getXmlFilePathFromLangName(lang2switch);
	if (langPath == TEXT(""))
		return false;

	return ::CopyFileW(langPath.c_str(), _nativeLangPath.c_str(), FALSE) != FALSE;
}

#endif

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);


NppParameters * NppParameters::_pSelf = new NppParameters;
//int FileDialog::_dialogFileBoxId = (NppParameters::getInstance())->getWinVersion() < WV_W2K?edt1:cmb13;

NppParameters::NppParameters() : _pXmlDoc(NULL),_pXmlUserDoc(NULL), _pXmlUserStylerDoc(NULL),\
								_pXmlUserLangDoc(NULL), /*_pXmlNativeLangDoc(NULL), */_pXmlNativeLangDocA(NULL),\
								_nbLang(0), _nbFile(0), _nbMaxFile(10), _pXmlToolIconsDoc(NULL),\
								_pXmlShortcutDoc(NULL), _pXmlContextMenuDoc(NULL),\
								_nbUserLang(0), _nbExternalLang(0), _hUser32(NULL), _hUXTheme(NULL),\
								_transparentFuncAddr(NULL), _enableThemeDialogTextureFuncAddr(NULL),\
								_isTaskListRBUTTONUP_Active(false), _fileSaveDlgFilterIndex(-1), _isFindReplacing(false)
{
	_findHistory._nbFindHistoryPath = 0;
	_findHistory._nbFindHistoryFilter = 0;
	_findHistory._nbFindHistoryFind = 0;
	_findHistory._nbFindHistoryReplace = 0;
}

NppParameters::~NppParameters() 
{
	for (int i = 0 ; i < _nbLang ; i++)
		delete _langList[i];
	for (int i = 0 ; i < _nbFile ; i++)
		delete _LRFileList[i];
	for (int i = 0 ; i < _nbUserLang ; i++)
		delete _userLangArray[i];
	if (_hUser32)
		FreeLibrary(_hUser32);
	if (_hUXTheme)
		FreeLibrary(_hUXTheme);

	::RemoveFontResource(LINEDRAW_FONT);
}
void cutString(const TCHAR *str2cut, vector<generic_string> & patternVect)
{
	TCHAR str2scan[MAX_PATH];
	lstrcpy(str2scan, str2cut);
	size_t len = lstrlen(str2scan);
	bool isProcessing = false;
	TCHAR *pBegin = NULL;
	for (size_t i = 0 ; i <= len ; i++)
	{
		switch(str2scan[i])
		{
			case ' ':
			case '\0':
			{
				if (isProcessing)
				{
					str2scan[i] = '\0';
					patternVect.push_back(pBegin);
					isProcessing = false;
				}
				break;
			}

			default :
				if (!isProcessing)
				{
					isProcessing = true;
					pBegin = str2scan+i;
				}
		}
	}
}

bool NppParameters::reloadLang()
{
	TCHAR userPath[MAX_PATH];

	// Make localConf.xml path
	TCHAR localConfPath[MAX_PATH];
	lstrcpy(localConfPath, _nppPath);
	PathAppend(localConfPath, localConfFile);

	// Test if localConf.xml exist
	bool isLocal = (PathFileExists(localConfPath) == TRUE);

	if (isLocal)
	{
		lstrcpy(userPath, _nppPath);
	}
	else
	{
		ITEMIDLIST *pidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		SHGetPathFromIDList(pidl, userPath);
		if(theApp.m_pIPlatUI)
		{
			PathAppend(userPath, theApp.m_pIPlatUI->GetAppName());
		}else
		{
			PathAppend(userPath, TEXT("OwmScite"));
		}
	}

	TCHAR nativeLangPath[MAX_PATH];
	lstrcpy(nativeLangPath, userPath);
	PathAppend(nativeLangPath, TEXT("nativeLang.xml"));

	if (!PathFileExists(nativeLangPath))
	{
		return false;
	}

	if (_pXmlNativeLangDocA)
		delete _pXmlNativeLangDocA;

	_pXmlNativeLangDocA = new TiXmlDocumentA();

	bool loadOkay = _pXmlNativeLangDocA->LoadUnicodeFilePath(nativeLangPath);
	if (!loadOkay)
	{
		delete _pXmlNativeLangDocA;
		_pXmlNativeLangDocA = NULL;
		return false;
	}
	return loadOkay;
}

bool NppParameters::load()
{
	// Prepare for default path
	CString strOwmPath = theApp.GetModulePath();
	lstrcpy(_nppPath, strOwmPath);

	//Initialize current directory to startup directory
	::GetCurrentDirectory(MAX_PATH, _currentDirectory);

	_appdataNppDir[0] = '\0';
	::AddFontResource(LINEDRAW_FONT);

	//Load initial accelerator key definitions
	//initMenuKeys();
	//initScintillaKeys();

	L_END = L_EXTERNAL;
	bool isAllLaoded = true;
	for (int i = 0 ; i < NB_LANG ; _langList[i] = NULL, i++);
	
	TCHAR userPath[MAX_PATH];

	// Make localConf.xml path
	TCHAR localConfPath[MAX_PATH];
	lstrcpy(localConfPath, _nppPath);
	PathAppend(localConfPath, localConfFile);

	// Test if localConf.xml exist
	bool isLocal = (PathFileExists(localConfPath) == TRUE);

	if (isLocal)
	{
		lstrcpy(userPath, _nppPath);
	}
	else
	{
		ITEMIDLIST *pidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		SHGetPathFromIDList(pidl, userPath);

		if(theApp.m_pIPlatUI)
		{
			PathAppend(userPath, theApp.m_pIPlatUI->GetAppName());
		}else
		{
			PathAppend(userPath, TEXT("OwmScite"));
		}

		lstrcpy(_appdataNppDir, userPath);

		if (!PathFileExists(userPath))
		{
			::CreateDirectory(userPath, NULL);
		}
	}

	//-------------------------------------//
	// Transparent function for w2k and xp //
	//-------------------------------------//
	_hUser32 = ::GetModuleHandle(TEXT("User32"));
	if (_hUser32)
		_transparentFuncAddr = (WNDPROC)::GetProcAddress(_hUser32, "SetLayeredWindowAttributes");
	
	//---------------------------------------------//
	// Dlg theme texture function for xp and vista //
	//---------------------------------------------//
	_hUXTheme = ::LoadLibrary(TEXT("uxtheme.dll"));
	if (_hUXTheme)
		_enableThemeDialogTextureFuncAddr = (WNDPROC)::GetProcAddress(_hUXTheme, "EnableThemeDialogTexture");

	//---------------------------------------//
	// langs.xml : for every user statically //
	//---------------------------------------//
	TCHAR langs_xml_path[MAX_PATH];
	lstrcpy(langs_xml_path, _nppPath);
	
	PathAppend(langs_xml_path, TEXT("langs.xml"));
	if (!PathFileExists(langs_xml_path))
	{
		TCHAR srcLangsPath[MAX_PATH];
		lstrcpy(srcLangsPath, _nppPath);
		PathAppend(srcLangsPath, TEXT("langs.model.xml"));

		::CopyFile(srcLangsPath, langs_xml_path, TRUE);
	}

	_pXmlDoc = new TiXmlDocument(langs_xml_path);
	bool loadOkay = _pXmlDoc->LoadFile();
	if (!loadOkay)
	{
		::MessageBox(NULL, TEXT("Load langs.xml failed!"), TEXT("Configurator"),MB_OK);
		delete _pXmlDoc;
		_pXmlDoc = NULL;
		isAllLaoded = false;
	}
	else
		getLangKeywordsFromXmlTree();

	//---------------------------//
	// config.xml : for per user //
	//---------------------------//
	TCHAR configPath[MAX_PATH];
	lstrcpy(configPath, userPath);
	PathAppend(configPath, TEXT("config.xml"));
	
	TCHAR srcConfigPath[MAX_PATH];
	lstrcpy(srcConfigPath, _nppPath);
	PathAppend(srcConfigPath, TEXT("config.model.xml"));

	if (!::PathFileExists(configPath))
		::CopyFile(srcConfigPath, configPath, FALSE);

label_load_configxml:
	_pXmlUserDoc = new TiXmlDocument(configPath);
	loadOkay = _pXmlUserDoc->LoadFile();
	if (!loadOkay)
	{
		int res = ::MessageBox(NULL, TEXT("Load config.xml failed!\rDo you want to recover your config.xml?"), TEXT("Configurator"),MB_YESNO);
		if (res ==IDYES)
		{
			::CopyFile(srcConfigPath, configPath, FALSE);

			loadOkay = _pXmlUserDoc->LoadFile();
			if (!loadOkay)
			{
				::MessageBox(NULL, TEXT("Recover config.xml failed!"), TEXT("Configurator"),MB_OK);
				delete _pXmlUserDoc;
				_pXmlUserDoc = NULL;
				isAllLaoded = false;
			}
			else
				getUserParametersFromXmlTree();
		}
		else
		{
			delete _pXmlUserDoc;
			_pXmlUserDoc = NULL;
			isAllLaoded = false;
		}
	}
	else
	{
		TiXmlNode *root = _pXmlUserDoc->FirstChild(TEXT("OwmScite"));
		const TCHAR *fileVersion = (root->ToElement())->Attribute(TEXT("version"));
		CString strVersion = fileVersion ? fileVersion : "";
		if (theApp.m_pIPlatUI->GetVciVersion("org.owm.edit") != strVersion)
		{
			delete _pXmlUserDoc;

			::CopyFile(srcConfigPath, configPath, FALSE);

			goto label_load_configxml;
		}
		getUserParametersFromXmlTree();
	}

	//----------------------------//
	// stylers.xml : for per user //
	//----------------------------//
	TCHAR stylerPath[MAX_PATH];
	lstrcpy(stylerPath, userPath);
	PathAppend(stylerPath, TEXT("stylers.xml"));

	if (!PathFileExists(stylerPath))
	{
		TCHAR srcStylersPath[MAX_PATH];
		lstrcpy(srcStylersPath, _nppPath);
		PathAppend(srcStylersPath, TEXT("stylers.model.xml"));

		::CopyFile(srcStylersPath, stylerPath, TRUE);
	}

label_load_stylerxml:
	_pXmlUserStylerDoc = new TiXmlDocument(stylerPath);
	loadOkay = _pXmlUserStylerDoc->LoadFile();
	if (!loadOkay)
	{
		::MessageBox(NULL, TEXT("Load stylers.xml failed!"), TEXT("Configurator"),MB_OK);
		delete _pXmlUserStylerDoc;
		_pXmlUserStylerDoc = NULL;
		isAllLaoded = false;
	}
	else
	{
		TiXmlNode *root = _pXmlUserStylerDoc->FirstChild(TEXT("OwmScite"));
		const TCHAR *fileVersion = (root->ToElement())->Attribute(TEXT("version"));
		CString strVersion = fileVersion ? fileVersion : "";
		if (theApp.m_pIPlatUI->GetVciVersion("org.owm.edit") != strVersion)
		{
			delete _pXmlUserStylerDoc;

			TCHAR srcStylersPath[MAX_PATH];
			lstrcpy(srcStylersPath, _nppPath);
			PathAppend(srcStylersPath, TEXT("stylers.model.xml"));

			::CopyFile(srcStylersPath, stylerPath, FALSE);

			goto label_load_stylerxml;
		}
		getUserStylersFromXmlTree();
	}

	//-----------------------------------//
	// userDefineLang.xml : for per user //
	//-----------------------------------//
	lstrcpy(_userDefineLangPath, userPath);
	PathAppend(_userDefineLangPath, TEXT("userDefineLang.xml"));

	_pXmlUserLangDoc = new TiXmlDocument(_userDefineLangPath);
	loadOkay = _pXmlUserLangDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlUserLangDoc;
		_pXmlUserLangDoc = NULL;
		isAllLaoded = false;
	}
	else
		getUserDefineLangsFromXmlTree();
	
	//----------------------------------------------//
	// nativeLang.xml : for per user                //
	// In case of absence of user's nativeLang.xml, //
	// We'll look in the Notepad++ Dir.             //
	//----------------------------------------------//
	TCHAR nativeLangPath[MAX_PATH];
	lstrcpy(nativeLangPath, userPath);
	PathAppend(nativeLangPath, TEXT("nativeLang.xml"));
	
	if (!PathFileExists(nativeLangPath))
	{
		lstrcpy(nativeLangPath, _nppPath);
		PathAppend(nativeLangPath, TEXT("nativeLang.xml"));
	}

	_pXmlNativeLangDocA = new TiXmlDocumentA();

	loadOkay = _pXmlNativeLangDocA->LoadUnicodeFilePath(nativeLangPath);
	if (!loadOkay)
	{
		delete _pXmlNativeLangDocA;
		_pXmlNativeLangDocA = NULL;
		isAllLaoded = false;
	}

/*
	//----------------------------------------------//
	// english.xml : for every user                 //
	// Always in the Notepad++ Dir.                 //
	//----------------------------------------------//
	TCHAR englishPath[MAX_PATH];
	lstrcpy(englishPath, _nppPath);
	PathAppend(englishPath, TEXT("localization\\english.xml"));

	if (PathFileExists(englishPath))
	{
		_pXmlEnglishDocA = new TiXmlDocumentA();

		loadOkay = _pXmlEnglishDocA->LoadUnicodeFilePath(englishPath);
		if (!loadOkay)
		{
			delete _pXmlEnglishDocA;
			_pXmlEnglishDocA = NULL;
			isAllLaoded = false;
		}
	}

	//------------------------------//
	// shortcuts.xml : for per user //
	//------------------------------//
	lstrcpy(_shortcutsPath, userPath);
	PathAppend(_shortcutsPath, TEXT("shortcuts.xml"));

	if (!PathFileExists(_shortcutsPath))
	{
		TCHAR srcShortcutsPath[MAX_PATH];
		lstrcpy(srcShortcutsPath, _nppPath);
		PathAppend(srcShortcutsPath, TEXT("shortcuts.xml"));

		::CopyFile(srcShortcutsPath, _shortcutsPath, TRUE);
	}

	_pXmlShortcutDoc = new TiXmlDocument(_shortcutsPath);
	loadOkay = _pXmlShortcutDoc->LoadFile();
	if (!loadOkay)
	{
		delete _pXmlShortcutDoc;
		_pXmlShortcutDoc = NULL;
		isAllLaoded = false;
	}
	else
	{
		getShortcutsFromXmlTree();
		getMacrosFromXmlTree();
		getUserCmdsFromXmlTree();

		// fill out _scintillaModifiedKeys : 
		// those user defined Scintilla key will be used remap Scintilla Key Array
		getScintKeysFromXmlTree();
	}
*/
	return isAllLaoded;
}

void NppParameters::destroyInstance()
{
	if (_pXmlDoc != NULL)
	{
		delete _pXmlDoc;
	}

	if (_pXmlUserDoc != NULL)
	{
		_pXmlUserDoc->SaveFile();
		delete _pXmlUserDoc;
	}
	if (_pXmlUserStylerDoc)
		delete _pXmlUserStylerDoc;

	if (_pXmlUserLangDoc)
		delete _pXmlUserLangDoc;

	if (_pXmlNativeLangDocA)
		delete _pXmlNativeLangDocA;

	if (_pXmlToolIconsDoc)
		delete _pXmlToolIconsDoc;

	if (_pXmlShortcutDoc)
		delete _pXmlShortcutDoc;

	if (_pXmlContextMenuDoc)
		delete _pXmlContextMenuDoc;

	delete _pSelf;
}

void NppParameters::setFontList(HWND hWnd)
{
	//---------------//
	// Sys font list //
	//---------------//

	LOGFONT lf;
	_fontlist.clear();
	_fontlist.push_back(TEXT(""));

	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0]='\0';
	lf.lfPitchAndFamily = 0;
	HDC hDC = ::GetDC(hWnd);

	::EnumFontFamiliesEx(hDC, 
						&lf, 
						(FONTENUMPROC) EnumFontFamExProc, 
						(LPARAM) &_fontlist, 0);
}

void NppParameters::getLangKeywordsFromXmlTree()
{
	TiXmlNode *root = _pXmlDoc->FirstChild(TEXT("OwmScite"));
		if (!root) return;
	feedKeyWordsParameters(root);
}
 
void NppParameters::getExternalLexerFromXmlTree(TiXmlDocument *doc)
{
	TiXmlNode *root = doc->FirstChild(TEXT("OwmScite"));
		if (!root) return;
	feedKeyWordsParameters(root);
	feedStylerArray(root);
}

int NppParameters::addExternalLangToEnd(ExternalLangContainer * externalLang)
{
	_externalLangArray[_nbExternalLang] = externalLang;
	_nbExternalLang++;
	L_END++;
	return _nbExternalLang-1;
}

bool NppParameters::getUserStylersFromXmlTree()
{
	TiXmlNode *root = _pXmlUserStylerDoc->FirstChild(TEXT("OwmScite"));
		if (!root) return false;
	return feedStylerArray(root);
}

bool NppParameters::getUserParametersFromXmlTree()
{
	if (!_pXmlUserDoc)
		return false;

	TiXmlNode *root = _pXmlUserDoc->FirstChild(TEXT("OwmScite"));
	if (!root) return false;

	// GUI
	feedGUIParameters(root);

	//History
	feedFileListParameters(root);

	// Raser tout
	TiXmlNode *node = root->FirstChildElement(TEXT("History"));
	root->RemoveChild(node);

	// Repartir de zero
	TiXmlElement HistoryNode(TEXT("History"));

	root->InsertEndChild(HistoryNode);

	//Find history
	feedFindHistoryParameters(root);

	return true;
}

bool NppParameters::getUserDefineLangsFromXmlTree()
{
	if (!_pXmlUserLangDoc)
		return false;
	
	TiXmlNode *root = _pXmlUserLangDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedUserLang(root);
	return true;
}

bool NppParameters::getShortcutsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;
	
	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedShortcut(root);
	return true;
}

bool NppParameters::getMacrosFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;
	
	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedMacros(root);
	return true;
}

bool NppParameters::getUserCmdsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;
	
	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedUserCmds(root);
	return true;
}


bool NppParameters::getPluginCmdsFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;
	
	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedPluginCustomizedCmds(root);
	return true;
}


bool NppParameters::getScintKeysFromXmlTree()
{
	if (!_pXmlShortcutDoc)
		return false;
	
	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root) 
		return false;

	feedScintKeys(root);
	return true;
}

void NppParameters::initMenuKeys() 
{
	int nrCommands = sizeof(winKeyDefs)/sizeof(WinMenuKeyDefinition);
	WinMenuKeyDefinition wkd;
	for(int i = 0; i < nrCommands; i++) 
	{
		wkd = winKeyDefs[i];
		Shortcut sc( (wkd.specialName?wkd.specialName:TEXT("")), wkd.isCtrl, wkd.isAlt, wkd.isShift, wkd.vKey);
		_shortcuts.push_back( CommandShortcut(sc, wkd.functionId) );
	}
}

void NppParameters::initScintillaKeys() {

	int nrCommands = sizeof(scintKeyDefs)/sizeof(ScintillaKeyDefinition);

	//Warning! Matching function have to be consecutive
	ScintillaKeyDefinition skd;
	size_t prevIndex = -1;
	int prevID = -1;
	for(int i = 0; i < nrCommands; i++) {
		skd = scintKeyDefs[i];
		if (skd.functionId == prevID) {
			KeyCombo kc;
			kc._isCtrl = skd.isCtrl;
			kc._isAlt = skd.isAlt;
			kc._isShift = skd.isShift;
			kc._key = skd.vKey;
			_scintillaKeyCommands[prevIndex].addKeyCombo(kc);
		} else {
			_scintillaKeyCommands.push_back(ScintillaKeyMap(Shortcut(skd.name, skd.isCtrl, skd.isAlt, skd.isShift, skd.vKey), skd.functionId, skd.redirFunctionId));
			prevIndex++;
		}
		prevID = skd.functionId;
	}
}

void NppParameters::setWorkingDir(const TCHAR * newPath)
{
	if (newPath && newPath[0]) 
	{
		lstrcpyn(_currentDirectory, newPath, MAX_PATH);	//dont use sizeof
	}
	else
	{
		if (PathFileExists(_nppGUI._defaultDirExp))
		{
			lstrcpyn(_currentDirectory, _nppGUI._defaultDirExp, MAX_PATH);
		}
		else
		{
			lstrcpyn(_currentDirectory, _nppPath, MAX_PATH);
		}
	}
}

void NppParameters::feedFileListParameters(TiXmlNode *node)
{
	_nbMaxFile = 10;

	TiXmlNode *historyRoot = node->FirstChildElement(TEXT("History"));
	if (!historyRoot) return;

	(historyRoot->ToElement())->Attribute(TEXT("nbMaxFile"), &_nbMaxFile);
	if ((_nbMaxFile < 0) || (_nbMaxFile > 30))
		return;

	for (TiXmlNode *childNode = historyRoot->FirstChildElement(TEXT("File"));
		childNode && (_nbFile < NB_MAX_LRF_FILE);
		childNode = childNode->NextSibling(TEXT("File")) )
	{
		const TCHAR *filePath = (childNode->ToElement())->Attribute(TEXT("filename"));
		if (filePath)
		{
			_LRFileList[_nbFile] = new generic_string(filePath);
			_nbFile++;
		}
	}
}

void NppParameters::feedFindHistoryParameters(TiXmlNode *node)
{
	TiXmlNode *findHistoryRoot = node->FirstChildElement(TEXT("FindHistory"));
	if (!findHistoryRoot) return;

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryPath"), &_findHistory._nbMaxFindHistoryPath);
	if ((_findHistory._nbMaxFindHistoryPath > 0) && (_findHistory._nbMaxFindHistoryPath <= NB_MAX_FINDHISTORY_PATH))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Path"));
			childNode && (_findHistory._nbFindHistoryPath < NB_MAX_FINDHISTORY_PATH);
			childNode = childNode->NextSibling(TEXT("Path")) )
		{
			const TCHAR *filePath = (childNode->ToElement())->Attribute(TEXT("name"));
			if (filePath)
			{
				_findHistory._pFindHistoryPath[_findHistory._nbFindHistoryPath++] = new generic_string(filePath);
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryFilter"), &_findHistory._nbMaxFindHistoryFilter);
	if ((_findHistory._nbMaxFindHistoryFilter > 0) && (_findHistory._nbMaxFindHistoryFilter <= NB_MAX_FINDHISTORY_FILTER))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Filter"));
			childNode && (_findHistory._nbFindHistoryFilter < NB_MAX_FINDHISTORY_FILTER);
			childNode = childNode->NextSibling(TEXT("Filter")))
		{
			const TCHAR *fileFilter = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileFilter)
			{
				_findHistory._pFindHistoryFilter[_findHistory._nbFindHistoryFilter++] = new generic_string(fileFilter);
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryFind"), &_findHistory._nbMaxFindHistoryFind);
	if ((_findHistory._nbMaxFindHistoryFind > 0) && (_findHistory._nbMaxFindHistoryFind <= NB_MAX_FINDHISTORY_FIND))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Find"));
			childNode && (_findHistory._nbFindHistoryFind < NB_MAX_FINDHISTORY_FIND);
			childNode = childNode->NextSibling(TEXT("Find")))
		{
			const TCHAR *fileFind = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileFind)
			{
				_findHistory._pFindHistoryFind[_findHistory._nbFindHistoryFind++] = new generic_string(fileFind);
			}
		}
	}

	(findHistoryRoot->ToElement())->Attribute(TEXT("nbMaxFindHistoryReplace"), &_findHistory._nbMaxFindHistoryReplace);
	if ((_findHistory._nbMaxFindHistoryReplace > 0) && (_findHistory._nbMaxFindHistoryReplace <= NB_MAX_FINDHISTORY_REPLACE))
	{
		for (TiXmlNode *childNode = findHistoryRoot->FirstChildElement(TEXT("Replace"));
			childNode && (_findHistory._nbFindHistoryReplace < NB_MAX_FINDHISTORY_REPLACE);
			childNode = childNode->NextSibling(TEXT("Replace")))
		{
			const TCHAR *fileReplace = (childNode->ToElement())->Attribute(TEXT("name"));
			if (fileReplace)
			{
				_findHistory._pFindHistoryReplace[_findHistory._nbFindHistoryReplace++] = new generic_string(fileReplace);
			}
		}
	}

	const TCHAR *boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("matchWord"));
	if (boolStr)
		_findHistory._isMatchWord = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("matchCase"));
	if (boolStr)
		_findHistory._isMatchCase = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("wrap"));
	if (boolStr)
		_findHistory._isWrap = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("directionDown"));
	if (boolStr)
		_findHistory._isDirectionDown = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifRecuisive"));
	if (boolStr)
		_findHistory._isFifRecuisive = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("fifInHiddenFolder"));
	if (boolStr)
		_findHistory._isFifInHiddenFolder = !lstrcmp(TEXT("yes"), boolStr);

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("dlgAlwaysVisible"));
	if (boolStr)
		_findHistory._isDlgAlwaysVisible = !lstrcmp(TEXT("yes"), boolStr);

	int mode = 0;
	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("searchMode"), &mode);
	if (boolStr)
		_findHistory._searchMode = (FindHistory::searchMode)mode;

	boolStr = (findHistoryRoot->ToElement())->Attribute(TEXT("transparencyMode"), &mode);
	if (boolStr)
		_findHistory._transparencyMode = (FindHistory::transparencyMode)mode;

	(findHistoryRoot->ToElement())->Attribute(TEXT("transparency"), &_findHistory._transparency);
	if (_findHistory._transparency <= 0 || _findHistory._transparency > 200)
		_findHistory._transparency = 150;
}

void NppParameters::feedShortcut(TiXmlNode *node)
{
	TiXmlNode *shortcutsRoot = node->FirstChildElement(TEXT("InternalCommands"));
	if (!shortcutsRoot) return;

	for (TiXmlNode *childNode = shortcutsRoot->FirstChildElement(TEXT("Shortcut"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Shortcut")) )
	{
		int id;
		const TCHAR *idStr = (childNode->ToElement())->Attribute(TEXT("id"), &id);
		if (idStr)
		{
			//find the commandid that matches this Shortcut sc and alter it, push back its index in the modified list, if not present
			int len = (int)_shortcuts.size();
			for(int i = 0; i < len; i++) 
			{
				if (_shortcuts[i].getID() == id) 
				{	//found our match
					getShortcuts(childNode, _shortcuts[i]);
					addUserModifiedIndex(i);
				}
			}
		}
	}
}

void NppParameters::feedMacros(TiXmlNode *node)
{
	TiXmlNode *macrosRoot = node->FirstChildElement(TEXT("Macros"));
	if (!macrosRoot) return;

	for (TiXmlNode *childNode = macrosRoot->FirstChildElement(TEXT("Macro"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Macro")) )
	{
		Shortcut sc;
		if (getShortcuts(childNode, sc))// && sc.isValid())
		{
			Macro macro;
			getActions(childNode, macro);
			int cmdID = ID_MACRO + _macros.size();
			MacroShortcut ms(sc, macro, cmdID);
			//if (ms.isValid())
			_macros.push_back(ms);
		}
	}
}


void NppParameters::getActions(TiXmlNode *node, Macro & macro)
{
	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("Action"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Action")) )
	{
		int type;
		const TCHAR *typeStr = (childNode->ToElement())->Attribute(TEXT("type"), &type);
		if ((!typeStr) || (type > 2))
			continue;

		int msg = 0;
		const TCHAR *msgStr = (childNode->ToElement())->Attribute(TEXT("message"), &msg);

		int wParam = 0;
		const TCHAR *wParamStr = (childNode->ToElement())->Attribute(TEXT("wParam"), &wParam);

		int lParam = 0;
		const TCHAR *lParamStr = (childNode->ToElement())->Attribute(TEXT("lParam"), &lParam);

		const TCHAR *sParam = (childNode->ToElement())->Attribute(TEXT("sParam"));
		if (!sParam)
			sParam = TEXT("");
		recordedMacroStep step(type, msg, wParam, lParam, sParam);
		if (step.isValid())
			macro.push_back(step);

	}
}

void NppParameters::feedUserCmds(TiXmlNode *node)
{
	TiXmlNode *userCmdsRoot = node->FirstChildElement(TEXT("UserDefinedCommands"));
	if (!userCmdsRoot) return;

	for (TiXmlNode *childNode = userCmdsRoot->FirstChildElement(TEXT("Command"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Command")) )
	{
		Shortcut sc;
		if (getShortcuts(childNode, sc))// && sc.isValid())
		{
			TiXmlNode *aNode = childNode->FirstChild();
			if (aNode)
			{
				const TCHAR *cmdStr = aNode->Value();
				if (cmdStr)
				{
					int cmdID = ID_USER_CMD + _userCommands.size();
					UserCommand uc(sc, cmdStr, cmdID);
					//if (uc.isValid())
					_userCommands.push_back(uc);
				}
			}
		}
	}
}

void NppParameters::feedPluginCustomizedCmds(TiXmlNode *node)
{
	TiXmlNode *pluginCustomizedCmdsRoot = node->FirstChildElement(TEXT("PluginCommands"));
	if (!pluginCustomizedCmdsRoot) return;

	for (TiXmlNode *childNode = pluginCustomizedCmdsRoot->FirstChildElement(TEXT("PluginCommand"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("PluginCommand")) )
	{
		const TCHAR *moduleName = (childNode->ToElement())->Attribute(TEXT("moduleName"));
		if (!moduleName)
			continue;

		int internalID = -1;
		const TCHAR *internalIDStr = (childNode->ToElement())->Attribute(TEXT("internalID"), &internalID);

		if (!internalIDStr)
			continue;

		//Find the corresponding plugincommand and alter it, put the index in the list
		int len = (int)_pluginCommands.size();
		for(int i = 0; i < len; i++) 
		{
			PluginCmdShortcut & pscOrig = _pluginCommands[i];
			if (!generic_strnicmp(pscOrig.getModuleName(), moduleName, lstrlen(moduleName)) && pscOrig.getInternalID() == internalID) 
			{
				//Found matching command
				getShortcuts(childNode, _pluginCommands[i]);
				addPluginModifiedIndex(i);
				break;
			}
		}
	}
}

void NppParameters::feedScintKeys(TiXmlNode *node)
{
	TiXmlNode *scintKeysRoot = node->FirstChildElement(TEXT("ScintillaKeys"));
	if (!scintKeysRoot) return;

	for (TiXmlNode *childNode = scintKeysRoot->FirstChildElement(TEXT("ScintKey"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("ScintKey")) )
	{
		int scintKey;
		const TCHAR *keyStr = (childNode->ToElement())->Attribute(TEXT("ScintID"), &scintKey);
		if (!keyStr)
			continue;

		int menuID;
		keyStr = (childNode->ToElement())->Attribute(TEXT("menuCmdID"), &menuID);
		if (!keyStr)
			continue;
		
		//Find the corresponding scintillacommand and alter it, put the index in the list
		size_t len = _scintillaKeyCommands.size();
		for(size_t i = 0; i < len; i++) 
		{
			ScintillaKeyMap & skmOrig = _scintillaKeyCommands[i];
			if (skmOrig.getScintillaKeyID() == scintKey &&skmOrig.getMenuCmdID() == menuID)
			{
				//Found matching command
				_scintillaKeyCommands[i].clearDups();
				getShortcuts(childNode, _scintillaKeyCommands[i]);
				_scintillaKeyCommands[i].setKeyComboByIndex(0, _scintillaKeyCommands[i].getKeyCombo());
				addScintillaModifiedIndex(i);
				KeyCombo kc;
				for (TiXmlNode *nextNode = childNode->FirstChildElement(TEXT("NextKey"));
					nextNode ;
					nextNode = childNode->NextSibling(TEXT("NextKey")) )
				{
					const TCHAR *str = (nextNode->ToElement())->Attribute(TEXT("Ctrl"));
					if (!str)
						continue;
					kc._isCtrl = !lstrcmp(TEXT("yes"), str);

					str = (nextNode->ToElement())->Attribute(TEXT("Alt"));
					if (!str)
						continue;
					kc._isAlt = !lstrcmp(TEXT("yes"), str);

					str = (nextNode->ToElement())->Attribute(TEXT("Shift"));
					if (!str)
						continue;
					kc._isShift = !lstrcmp(TEXT("yes"), str);

					int key;
					str = (nextNode->ToElement())->Attribute(TEXT("Key"), &key);
					if (!str)
						continue;
					kc._key = key;
					_scintillaKeyCommands[i].addKeyCombo(kc);
				}
				break;
			}
		}
	}
}

bool NppParameters::getShortcuts(TiXmlNode *node, Shortcut & sc)
{
	if (!node) return false;

	const TCHAR *name = (node->ToElement())->Attribute(TEXT("name"));
	if (!name)
		name = TEXT("");

	bool isCtrl = false;
	const TCHAR *isCtrlStr = (node->ToElement())->Attribute(TEXT("Ctrl"));
	if (isCtrlStr)
		isCtrl = !lstrcmp(TEXT("yes"), isCtrlStr);

	bool isAlt = false;
	const TCHAR *isAltStr = (node->ToElement())->Attribute(TEXT("Alt"));
	if (isAltStr)
		isAlt = !lstrcmp(TEXT("yes"), isAltStr);

	bool isShift = false;
	const TCHAR *isShiftStr = (node->ToElement())->Attribute(TEXT("Shift"));
	if (isShiftStr)
		isShift = !lstrcmp(TEXT("yes"), isShiftStr);

	int key;
	const TCHAR *keyStr = (node->ToElement())->Attribute(TEXT("Key"), &key);
	if (!keyStr)
		return false;

	sc = Shortcut(name, isCtrl, isAlt, isShift, key);
	return true;
}


const int loadFailed = 100;
const int missingName = 101;
void NppParameters::feedUserLang(TiXmlNode *node)
{
	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("UserLang"));
		childNode && (_nbUserLang < NB_MAX_USER_LANG);
		childNode = childNode->NextSibling(TEXT("UserLang")) )
	{
		const TCHAR *name = (childNode->ToElement())->Attribute(TEXT("name"));
		const TCHAR *ext = (childNode->ToElement())->Attribute(TEXT("ext"));
		try {
			if (!name || !name[0] || !ext) throw int(missingName);

			_userLangArray[_nbUserLang] = new UserLangContainer(name, ext);
			_nbUserLang++;

			TiXmlNode *settingsRoot = childNode->FirstChildElement(TEXT("Settings"));
			if (!settingsRoot) throw int(loadFailed);
			feedUserSettings(settingsRoot);

			TiXmlNode *keywordListsRoot = childNode->FirstChildElement(TEXT("KeywordLists"));
			if (!keywordListsRoot) throw int(loadFailed);
			feedUserKeywordList(keywordListsRoot);

			TiXmlNode *stylesRoot = childNode->FirstChildElement(TEXT("Styles"));
			if (!stylesRoot) throw int(loadFailed);
			feedUserStyles(stylesRoot);

		} catch (int e) {
			if (e == loadFailed)
				delete _userLangArray[--_nbUserLang];
		}
	}
}
/*
void NppParameters::writeUserDefinedLang()
{
	if (!_pXmlUserLangDoc)
	{
		//do the treatment
		_pXmlUserLangDoc = new TiXmlDocument(_userDefineLangPath);
	}

	//before remove the branch, we allocate and copy the TCHAR * which will be destroyed
	stylerStrOp(DUP);

	TiXmlNode *root = _pXmlUserLangDoc->FirstChild(TEXT("OwmScite"));
	if (root) 
	{
		_pXmlUserLangDoc->RemoveChild(root);
	}
	
	_pXmlUserLangDoc->InsertEndChild(TiXmlElement(TEXT("OwmScite")));

	root = _pXmlUserLangDoc->FirstChild(TEXT("OwmScite"));

	for (int i = 0 ; i < _nbUserLang ; i++)
	{
		insertUserLang2Tree(root, _userLangArray[i]);
	}
	_pXmlUserLangDoc->SaveFile();
	stylerStrOp(FREE);
}

void NppParameters::insertCmd(TiXmlNode *shortcutsRoot, const CommandShortcut & cmd)
{
	const KeyCombo & key = cmd.getKeyCombo();
	TiXmlNode *sc = shortcutsRoot->InsertEndChild(TiXmlElement(TEXT("Shortcut")));
	sc->ToElement()->SetAttribute(TEXT("id"), cmd.getID());
	sc->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	sc->ToElement()->SetAttribute(TEXT("Key"), key._key);
}

void NppParameters::insertMacro(TiXmlNode *macrosRoot, const MacroShortcut & macro)
{
	const KeyCombo & key = macro.getKeyCombo();
	TiXmlNode *macroRoot = macrosRoot->InsertEndChild(TiXmlElement(TEXT("Macro")));
	macroRoot->ToElement()->SetAttribute(TEXT("name"), macro.getMenuName());
	macroRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	macroRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);
	for (size_t i = 0 ; i < macro._macro.size() ; i++)
	{
		TiXmlNode *actionNode = macroRoot->InsertEndChild(TiXmlElement(TEXT("Action")));
		const recordedMacroStep & action = macro._macro[i];
		actionNode->ToElement()->SetAttribute(TEXT("type"), action.MacroType);
		actionNode->ToElement()->SetAttribute(TEXT("message"), action.message);
		actionNode->ToElement()->SetAttribute(TEXT("wParam"), action.wParameter);
		actionNode->ToElement()->SetAttribute(TEXT("lParam"), action.lParameter);
		actionNode->ToElement()->SetAttribute(TEXT("sParam"), action.sParameter.c_str());
	}
}

void NppParameters::insertUserCmd(TiXmlNode *userCmdRoot, const UserCommand & userCmd)
{
	const KeyCombo & key = userCmd.getKeyCombo();
	TiXmlNode *cmdRoot = userCmdRoot->InsertEndChild(TiXmlElement(TEXT("Command")));
	cmdRoot->ToElement()->SetAttribute(TEXT("name"), userCmd.getMenuName());
	cmdRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	cmdRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);
	cmdRoot->InsertEndChild(TiXmlText(userCmd._cmd.c_str()));
}

void NppParameters::insertPluginCmd(TiXmlNode *pluginCmdRoot, const PluginCmdShortcut & pluginCmd)
{
	const KeyCombo & key = pluginCmd.getKeyCombo();
	TiXmlNode *pluginCmdNode = pluginCmdRoot->InsertEndChild(TiXmlElement(TEXT("PluginCommand")));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("moduleName"), pluginCmd.getModuleName());
	pluginCmdNode->ToElement()->SetAttribute(TEXT("internalID"), pluginCmd.getInternalID());
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	pluginCmdNode->ToElement()->SetAttribute(TEXT("Key"), key._key);
}

void NppParameters::insertScintKey(TiXmlNode *scintKeyRoot, const ScintillaKeyMap & scintKeyMap)
{
	TiXmlNode *keyRoot = scintKeyRoot->InsertEndChild(TiXmlElement(TEXT("ScintKey")));
	keyRoot->ToElement()->SetAttribute(TEXT("ScintID"), scintKeyMap.getScintillaKeyID());
	keyRoot->ToElement()->SetAttribute(TEXT("menuCmdID"), scintKeyMap.getMenuCmdID());

	//Add main shortcut
	KeyCombo key = scintKeyMap.getKeyComboByIndex(0);
	keyRoot->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
	keyRoot->ToElement()->SetAttribute(TEXT("Key"), key._key);

	//Add additional shortcuts
	size_t size = scintKeyMap.getSize();
	if (size > 1) {
		TiXmlNode * keyNext;
		for(size_t i = 1; i < size; i++) {
			keyNext = keyRoot->InsertEndChild(TiXmlElement(TEXT("NextKey")));
			key = scintKeyMap.getKeyComboByIndex(i);
			keyNext->ToElement()->SetAttribute(TEXT("Ctrl"), key._isCtrl?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Alt"), key._isAlt?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Shift"), key._isShift?TEXT("yes"):TEXT("no"));
			keyNext->ToElement()->SetAttribute(TEXT("Key"), key._key);
		}
	}
}

void NppParameters::writeShortcuts()
{
	if (!_pXmlShortcutDoc)
	{
		//do the treatment
		_pXmlShortcutDoc = new TiXmlDocument(_shortcutsPath);
	}

	TiXmlNode *root = _pXmlShortcutDoc->FirstChild(TEXT("OwmScite"));
	if (!root)
	{
		root = _pXmlShortcutDoc->InsertEndChild(TiXmlElement(TEXT("OwmScite")));
	}

	TiXmlNode *cmdRoot = root->FirstChild(TEXT("InternalCommands"));
	if (cmdRoot)
		root->RemoveChild(cmdRoot);

	cmdRoot = root->InsertEndChild(TiXmlElement(TEXT("InternalCommands")));
	for (size_t i = 0 ; i < _customizedShortcuts.size() ; i++)
	{
		int index = _customizedShortcuts[i];
		CommandShortcut csc = _shortcuts[index];
		insertCmd(cmdRoot, csc);
	}

	TiXmlNode *macrosRoot = root->FirstChild(TEXT("Macros"));
	if (macrosRoot)
		root->RemoveChild(macrosRoot);

	macrosRoot = root->InsertEndChild(TiXmlElement(TEXT("Macros")));

	for (size_t i = 0 ; i < _macros.size() ; i++)
	{
		insertMacro(macrosRoot, _macros[i]);
	}

	TiXmlNode *userCmdRoot = root->FirstChild(TEXT("UserDefinedCommands"));
	if (userCmdRoot)
		root->RemoveChild(userCmdRoot);
	
	userCmdRoot = root->InsertEndChild(TiXmlElement(TEXT("UserDefinedCommands")));
	
	for (size_t i = 0 ; i < _userCommands.size() ; i++)
	{
		insertUserCmd(userCmdRoot, _userCommands[i]);
	}

	TiXmlNode *pluginCmdRoot = root->FirstChild(TEXT("PluginCommands"));
	if (pluginCmdRoot)
		root->RemoveChild(pluginCmdRoot);

	pluginCmdRoot = root->InsertEndChild(TiXmlElement(TEXT("PluginCommands")));
	for (size_t i = 0 ; i < _pluginCustomizedCmds.size() ; i++)
	{
		insertPluginCmd(pluginCmdRoot, _pluginCommands[_pluginCustomizedCmds[i]]);
	}

	TiXmlNode *scitillaKeyRoot = root->FirstChild(TEXT("ScintillaKeys"));
	if (scitillaKeyRoot)
		root->RemoveChild(scitillaKeyRoot);

	scitillaKeyRoot = root->InsertEndChild(TiXmlElement(TEXT("ScintillaKeys")));
	for (size_t i = 0 ; i < _scintillaModifiedKeyIndices.size() ; i++)
	{
		insertScintKey(scitillaKeyRoot, _scintillaKeyCommands[_scintillaModifiedKeyIndices[i]]);
	}
	_pXmlShortcutDoc->SaveFile();
}
*/
int NppParameters::addUserLangToEnd(const UserLangContainer & userLang, const TCHAR *newName)
{
	if (isExistingUserLangName(newName))
		return -1;
	_userLangArray[_nbUserLang] = new UserLangContainer();
	*(_userLangArray[_nbUserLang]) = userLang;
	_userLangArray[_nbUserLang]->_name = newName;
	_nbUserLang++;
	return _nbUserLang-1;
}

void NppParameters::removeUserLang(int index)
{
	if (index >= _nbUserLang )
		return;
	delete _userLangArray[index];
	for (int i = index ; i < (_nbUserLang - 1) ; i++)
		_userLangArray[i] = _userLangArray[i+1];
	_nbUserLang--;
}

int NppParameters::getIndexFromKeywordListName(const TCHAR *name)
{
	if (!name) return -1;
	if (!lstrcmp(name, TEXT("Folder+")))	return 1;
	else if (!lstrcmp(name, TEXT("Folder-")))	return 2;
	else if (!lstrcmp(name, TEXT("Operators")))return 3;
	else if (!lstrcmp(name, TEXT("Comment")))	return 4;
	else if (!lstrcmp(name, TEXT("Words1")))	return 5;
	else if (!lstrcmp(name, TEXT("Words2")))	return 6;
	else if (!lstrcmp(name, TEXT("Words3")))	return 7;
	else if (!lstrcmp(name, TEXT("Words4")))	return 8;
	else if (!lstrcmp(name, TEXT("Delimiters")))	return 0;
	else return -1;
}
void NppParameters::feedUserSettings(TiXmlNode *settingsRoot)
{
	const TCHAR *boolStr;
	TiXmlNode *globalSettingNode = settingsRoot->FirstChildElement(TEXT("Global"));
	if (globalSettingNode)
	{
		boolStr = (globalSettingNode->ToElement())->Attribute(TEXT("caseIgnored"));
		if (boolStr)
			_userLangArray[_nbUserLang - 1]->_isCaseIgnored = !lstrcmp(TEXT("yes"), boolStr);
	}
	TiXmlNode *treatAsSymbolNode = settingsRoot->FirstChildElement(TEXT("TreatAsSymbol"));
	if (treatAsSymbolNode)
	{
		boolStr = (treatAsSymbolNode->ToElement())->Attribute(TEXT("comment"));
		if (boolStr)
			_userLangArray[_nbUserLang - 1]->_isCommentSymbol = !lstrcmp(TEXT("yes"), boolStr);

		boolStr = (treatAsSymbolNode->ToElement())->Attribute(TEXT("commentLine"));
		if (boolStr)
			_userLangArray[_nbUserLang - 1]->_isCommentLineSymbol = !lstrcmp(TEXT("yes"), boolStr);
	}
	TiXmlNode *prefixNode = settingsRoot->FirstChildElement(TEXT("Prefix"));
	if (prefixNode)
	{
		TCHAR names[nbPrefixListAllowed][7] = {TEXT("words1"), TEXT("words2"), TEXT("words3"), TEXT("words4")};
		for (int i = 0 ; i < nbPrefixListAllowed ; i++)
		{
			boolStr = (prefixNode->ToElement())->Attribute(names[i]);
			if (boolStr)
				_userLangArray[_nbUserLang - 1]->_isPrefix[i] = !lstrcmp(TEXT("yes"), boolStr);
		}
	}
}

void NppParameters::feedUserKeywordList(TiXmlNode *node)
{
	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("Keywords"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("Keywords")))
	{
		const TCHAR *keywordsName = (childNode->ToElement())->Attribute(TEXT("name"));
		int i = getIndexFromKeywordListName(keywordsName);
		if (i != -1)
		{
			TiXmlNode *valueNode = childNode->FirstChild();
			if (valueNode)
			{
				const TCHAR *kwl = (valueNode)?valueNode->Value():(lstrcmp(keywordsName, TEXT("Delimiters"))?TEXT(""):TEXT("000000"));
				lstrcpy(_userLangArray[_nbUserLang - 1]->_keywordLists[i], kwl);
			}
		}
	}
}

void NppParameters::feedUserStyles(TiXmlNode *node)
{
	for (TiXmlNode *childNode = node->FirstChildElement(TEXT("WordsStyle"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("WordsStyle")))
	{
		int id;
		const TCHAR *styleIDStr = (childNode->ToElement())->Attribute(TEXT("styleID"), &id);
		if (styleIDStr)
		{
			_userLangArray[_nbUserLang - 1]->_styleArray.addStyler(id, childNode);
		}
	}
}

bool NppParameters::feedStylerArray(TiXmlNode *node)
{
    TiXmlNode *styleRoot = node->FirstChildElement(TEXT("LexerStyles"));
    if (!styleRoot) return false;

    // For each lexer
    for (TiXmlNode *childNode = styleRoot->FirstChildElement(TEXT("LexerType"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("LexerType")) )
    {
     	if (!_lexerStylerArray.hasEnoughSpace()) return false;

	    TiXmlElement *element = childNode->ToElement();
	    const TCHAR *lexerName = element->Attribute(TEXT("name"));
		const TCHAR *lexerDesc = element->Attribute(TEXT("desc"));
		const TCHAR *lexerUserExt = element->Attribute(TEXT("ext"));
		const TCHAR *lexerExcluded = element->Attribute(TEXT("excluded"));
	    if (lexerName)
	    {
		    _lexerStylerArray.addLexerStyler(lexerName, lexerDesc, lexerUserExt, childNode);
			if (lexerExcluded != NULL && !lstrcmp(lexerExcluded, TEXT("yes")))
			{
				int index = getExternalLangIndexFromName(lexerName);
				if (index != -1)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)(index + L_EXTERNAL)));
			}
	    }
    }

    // The global styles for all lexers
    TiXmlNode *globalStyleRoot = node->FirstChildElement(TEXT("GlobalStyles"));
    if (!globalStyleRoot) return false;

    for (TiXmlNode* childNode = globalStyleRoot->FirstChildElement(TEXT("WidgetStyle"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("WidgetStyle")) )
    {
     	if (!_widgetStyleArray.hasEnoughSpace()) return false;

	    TiXmlElement *element = childNode->ToElement();
	    const TCHAR *styleIDStr = element->Attribute(TEXT("styleID"));

        int styleID = -1;
		if ((styleID = decStrVal(styleIDStr)) != -1)
		{
		    _widgetStyleArray.addStyler(styleID, childNode);
        }
    }
	return true;
}

void LexerStylerArray::addLexerStyler(const TCHAR *lexerName, const TCHAR *lexerDesc, const TCHAR *lexerUserExt , TiXmlNode *lexerNode)
{
    LexerStyler & ls = _lexerStylerArray[_nbLexerStyler++];
    ls.setLexerName(lexerName);
	if (lexerDesc)
		ls.setLexerDesc(lexerDesc);

	if (lexerUserExt)
		ls.setLexerUserExt(lexerUserExt);
    
    for (TiXmlNode *childNode = lexerNode->FirstChildElement(TEXT("WordsStyle"));
		 childNode ;
		 childNode = childNode->NextSibling(TEXT("WordsStyle")) )
    {
	        
		if (!ls.hasEnoughSpace()) return;

		TiXmlElement *element = childNode->ToElement();
		const TCHAR *styleIDStr = element->Attribute(TEXT("styleID"));
		
		if (styleIDStr)
		{
			int styleID = -1;
			if ((styleID = decStrVal(styleIDStr)) != -1)
			{
				ls.addStyler(styleID, childNode);
			}
		}
    }
}

void StyleArray::addStyler(int styleID, TiXmlNode *styleNode)
{
	_styleArray[_nbStyler]._styleID = styleID;
	
	if (styleNode)
	{
		TiXmlElement *element = styleNode->ToElement();
		
		// Pour _fgColor, _bgColor :
		// RGB() | (result & 0xFF000000) c'est pour le cas de -1 (0xFFFFFFFF)
		// retourné par hexStrVal(str)
		const TCHAR *str = element->Attribute(TEXT("name"));
		if (str)
		{
			_styleArray[_nbStyler]._styleDesc = str;
		}

		str = element->Attribute(TEXT("fgColor"));
		if (str)
		{
			unsigned long result = hexStrVal(str);
			_styleArray[_nbStyler]._fgColor = (RGB((result >> 16) & 0xFF, (result >> 8) & 0xFF, result & 0xFF)) | (result & 0xFF000000);
	            
		}
		
		str = element->Attribute(TEXT("bgColor"));
		if (str)
		{
			unsigned long result = hexStrVal(str);
			_styleArray[_nbStyler]._bgColor = (RGB((result >> 16) & 0xFF, (result >> 8) & 0xFF, result & 0xFF)) | (result & 0xFF000000);
		}

		str = element->Attribute(TEXT("colorStyle"));
		if (str)
		{
			_styleArray[_nbStyler]._colorStyle = decStrVal(str);
		}
		
		str = element->Attribute(TEXT("fontName"));
		_styleArray[_nbStyler]._fontName = str;
		
		str = element->Attribute(TEXT("fontStyle"));
		if (str)
		{
			_styleArray[_nbStyler]._fontStyle = decStrVal(str);
		}
		
		str = element->Attribute(TEXT("fontSize"));
		if (str)
		{
			_styleArray[_nbStyler]._fontSize = decStrVal(str);
		}

		str = element->Attribute(TEXT("keywordClass"));
		if (str)
		{
			_styleArray[_nbStyler]._keywordClass = getKwClassFromName(str);
		}

		TiXmlNode *v = styleNode->FirstChild();
		if (v)
		{
			_styleArray[_nbStyler]._keywords = new generic_string(v->Value());
		}
	}
	_nbStyler++;
}

TiXmlNode * NppParameters::getChildElementByAttribut(TiXmlNode *pere, const TCHAR *childName,\
			const TCHAR *attributName, const TCHAR *attributVal) const 
{
	for (TiXmlNode *childNode = pere->FirstChildElement(childName);
		childNode ;
		childNode = childNode->NextSibling(childName))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *val = element->Attribute(attributName);
		if (val)
		{
			if (!lstrcmp(val, attributVal))
				return childNode;
		}
	}
	return NULL;
}

// 2 restes : L_H, L_USER
LangType NppParameters::getLangIDFromStr(const TCHAR *langName)
{
	int lang = (int)L_TXT;
	for(; lang < L_EXTERNAL; lang++) {
		const TCHAR * name = ScintillaEditView::langNames[lang].lexerName;
		if (!lstrcmp(name, langName)) {	//found lang?
			return (LangType)lang;
		}
	}

	//Cannot find language, check if its an external one

	LangType l = (LangType)lang;
	if (l == L_EXTERNAL) {	//try find external lexer
		int id = _pSelf->getExternalLangIndexFromName(langName);
		if (id != -1) return (LangType)(id + L_EXTERNAL);
	}

	return L_TXT;
}

void NppParameters::feedKeyWordsParameters(TiXmlNode *node)
{
	
	TiXmlNode *langRoot = node->FirstChildElement(TEXT("Languages"));
	if (!langRoot) return;

	for (TiXmlNode *langNode = langRoot->FirstChildElement(TEXT("Language"));
		langNode ;
		langNode = langNode->NextSibling(TEXT("Language")) )
	{
		if (_nbLang < NB_LANG)
		{
			TiXmlElement *element = langNode->ToElement();
			const TCHAR *name = element->Attribute(TEXT("name"));
			if (name)
			{
				_langList[_nbLang] = new Lang(getLangIDFromStr(name), name);
				_langList[_nbLang]->setDefaultExtList(element->Attribute(TEXT("ext")));
				_langList[_nbLang]->setCommentLineSymbol(element->Attribute(TEXT("commentLine")));
				_langList[_nbLang]->setCommentStart(element->Attribute(TEXT("commentStart")));
				_langList[_nbLang]->setCommentEnd(element->Attribute(TEXT("commentEnd")));

				for (TiXmlNode *kwNode = langNode->FirstChildElement(TEXT("Keywords"));
					kwNode ;
					kwNode = kwNode->NextSibling(TEXT("Keywords")) )
				{
					const TCHAR *indexName = (kwNode->ToElement())->Attribute(TEXT("name"));
					TiXmlNode *kwVal = kwNode->FirstChild();
					const TCHAR *keyWords = TEXT("");
					if ((indexName) && (kwVal))
						keyWords = kwVal->Value();

					int i = getKwClassFromName(indexName);

					if (i >= 0 && i <= KEYWORDSET_MAX)
						_langList[_nbLang]->setWords(keyWords, i);
				}
				_nbLang++;
			}
		}
	}
}

extern "C" {
typedef DWORD (WINAPI * EESFUNC) (LPCTSTR, LPTSTR, DWORD);
}

void NppParameters::feedGUIParameters(TiXmlNode *node)
{
	TiXmlNode *GUIRoot = node->FirstChildElement(TEXT("GUIConfigs"));
	if (!GUIRoot) return;

	for (TiXmlNode *childNode = GUIRoot->FirstChildElement(TEXT("GUIConfig"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("GUIConfig")) )
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
		if (!nm) continue;

		const TCHAR *val;

		if (!lstrcmp(nm, TEXT("Auto-detection")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._fileAutoDetection = cdEnabled;
					else if (!lstrcmp(val, TEXT("auto")))
            			_nppGUI._fileAutoDetection = cdAutoUpdate;
					else if (!lstrcmp(val, TEXT("Update2End")))
            			_nppGUI._fileAutoDetection = cdGo2end;
					else if (!lstrcmp(val, TEXT("autoUpdate2End")))
            			_nppGUI._fileAutoDetection = cdAutoUpdateGo2end;
         			else //(!lstrcmp(val, TEXT("no")))
            			_nppGUI._fileAutoDetection = cdDisabled;

					_nppGUI._fileAutoDetectionOriginalValue = _nppGUI._fileAutoDetection;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("MaitainIndent")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._maitainIndent = true;
					else
						_nppGUI._maitainIndent = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("SmartHighLight")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("yes")))
						_nppGUI._enableSmartHilite = true;
					else
						_nppGUI._enableSmartHilite = false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("TagsMatchHighLight")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._enableTagsMatchHilite = !lstrcmp(val, TEXT("yes"));
					const TCHAR *tahl = element->Attribute(TEXT("TagAttrHighLight"));
					if (tahl) 
						_nppGUI._enableTagAttrsHilite = !lstrcmp(tahl, TEXT("yes"));

					tahl = element->Attribute(TEXT("HighLightNonHtmlZone"));
					if (tahl) 
						_nppGUI._enableHiliteNonHTMLZone = !lstrcmp(tahl, TEXT("yes"));
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("TaskList")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._doTaskList = (!lstrcmp(val, TEXT("yes")))?true:false;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("URL")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("1")))
						_nppGUI._styleURL = 1;
					else if (!lstrcmp(val, TEXT("2")))
						_nppGUI._styleURL = 2;
					else
						_nppGUI._styleURL = 0;
				}
			}
		}

		else if (!lstrcmp(nm, TEXT("ScintillaViewsSplitter")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					if (!lstrcmp(val, TEXT("vertical")))
						_nppGUI._splitterPos = POS_VERTICAL;
					else if (!lstrcmp(val, TEXT("horizontal")))
						_nppGUI._splitterPos = POS_HORIZOTAL;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("UserDefineDlg")))
		{
			bool isFailed = false;
			int oldValue = _nppGUI._userDefineDlgStatus;

			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val) 
				{
					if (!lstrcmp(val, TEXT("hide")))
						_nppGUI._userDefineDlgStatus = 0;
					else if (!lstrcmp(val, TEXT("show")))
						_nppGUI._userDefineDlgStatus = UDD_SHOW;
					else 
						isFailed = true;
				}
			}
			val = element->Attribute(TEXT("position"));
			if (val) 
			{
				if (!lstrcmp(val, TEXT("docked")))
					_nppGUI._userDefineDlgStatus |= UDD_DOCKED;
				else if (!lstrcmp(val, TEXT("undocked")))
					_nppGUI._userDefineDlgStatus |= 0;
				else 
					isFailed = true;
			}
			if (isFailed)
				_nppGUI._userDefineDlgStatus = oldValue;
		}
		else if (!lstrcmp(nm, TEXT("TabSetting")))
		{
			int i;
			val = element->Attribute(TEXT("size"), &i);
			if (val)
				_nppGUI._tabSize = i;

			if ((_nppGUI._tabSize == -1) || (_nppGUI._tabSize == 0))
				_nppGUI._tabSize = 8;

			val = element->Attribute(TEXT("replaceBySpace"));
			if (val)
				_nppGUI._tabReplacedBySpace = (!lstrcmp(val, TEXT("yes")));
		}

		else if (!lstrcmp(nm, TEXT("Caret")))
		{
			int i;
			val = element->Attribute(TEXT("width"), &i);
			if (val)
				_nppGUI._caretWidth = i;

			val = element->Attribute(TEXT("blinkRate"), &i);
			if (val)
				_nppGUI._caretBlinkRate = i;
		}

		else if (!lstrcmp(nm, TEXT("NewDocDefaultSettings")))
		{
			int i;
			if (element->Attribute(TEXT("format"), &i))
				_nppGUI._newDocDefaultSettings._format = (formatType)i;

			if (element->Attribute(TEXT("encoding"), &i))
				_nppGUI._newDocDefaultSettings._encoding = (UniMode)i;

			if (element->Attribute(TEXT("lang"), &i))
				_nppGUI._newDocDefaultSettings._lang = (LangType)i;

			if (val = element->Attribute(TEXT("openAnsiAsUTF8")))
				_nppGUI._newDocDefaultSettings._openAnsiAsUtf8 = (lstrcmp(val, TEXT("yes")) == 0);

		}
		else if (!lstrcmp(nm, TEXT("langsExcluded")))
		{
			int g0 = 0; // up to  8
			int g1 = 0; // up to 16
			int g2 = 0; // up to 24
			int g3 = 0; // up to 32
			int g4 = 0; // up to 40
			int g5 = 0; // up to 48
			int g6 = 0; // up to 56
			int g7 = 0; // up to 64
			const int nbMax = 64;

			int i;
			if (element->Attribute(TEXT("gr0"), &i))
				if (i <= 255)
					g0 = i;
			if (element->Attribute(TEXT("gr1"), &i))
				if (i <= 255)
					g1 = i;
			if (element->Attribute(TEXT("gr2"), &i))
				if (i <= 255)
					g2 = i;
			if (element->Attribute(TEXT("gr3"), &i))
				if (i <= 255)
					g3 = i;
			if (element->Attribute(TEXT("gr4"), &i))
				if (i <= 255)
					g4 = i;
			if (element->Attribute(TEXT("gr5"), &i))
				if (i <= 255)
					g5 = i;
			if (element->Attribute(TEXT("gr6"), &i))
				if (i <= 255)
					g6 = i;
			if (element->Attribute(TEXT("gr7"), &i))
				if (i <= 255)
					g7 = i;

			bool langArray[nbMax];
			for (i = 0 ; i < nbMax ; i++) langArray[i] = false;
			
			UCHAR mask = 1;
			for (i = 0 ; i < 8 ; i++) 
			{
				if (mask & g0)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 8 ; i < 16 ; i++) 
			{
				if (mask & g1)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 16 ; i < 24 ; i++) 
			{
				if (mask & g2)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 24 ; i < 32 ; i++) 
			{
				if (mask & g3)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 32 ; i < 40 ; i++) 
			{
				if (mask & g4)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 40 ; i < 48 ; i++) 
			{
				if (mask & g5)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 48 ; i < 56 ; i++) 
			{
				if (mask & g6)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}

			mask = 1;
			for (i = 56 ; i < 64 ; i++) 
			{
				if (mask & g7)
					_nppGUI._excludedLangList.push_back(LangMenuItem((LangType)i));
				mask <<= 1;
			}
		}

		else if (!lstrcmp(nm, TEXT("Print")))
		{
			val = element->Attribute(TEXT("lineNumber"));
			if (val)
				_nppGUI._printSettings._printLineNumber = (!lstrcmp(val, TEXT("yes")));

			int i;
			if (element->Attribute(TEXT("printOption"), &i))
				_nppGUI._printSettings._printOption = i;

			val = element->Attribute(TEXT("headerLeft"));
			if (val)
				_nppGUI._printSettings._headerLeft = val;

			val = element->Attribute(TEXT("headerMiddle"));
			if (val)
				_nppGUI._printSettings._headerMiddle = val;

			val = element->Attribute(TEXT("headerRight"));
			if (val)
				_nppGUI._printSettings._headerRight = val;


			val = element->Attribute(TEXT("footerLeft"));
			if (val)
				_nppGUI._printSettings._footerLeft = val;

			val = element->Attribute(TEXT("footerMiddle"));
			if (val)
				_nppGUI._printSettings._footerMiddle = val;

			val = element->Attribute(TEXT("footerRight"));
			if (val)
				_nppGUI._printSettings._footerRight = val;


			val = element->Attribute(TEXT("headerFontName"));
			if (val)
				_nppGUI._printSettings._headerFontName = val;

			val = element->Attribute(TEXT("footerFontName"));
			if (val)
				_nppGUI._printSettings._footerFontName = val;

			if (element->Attribute(TEXT("headerFontStyle"), &i))
				_nppGUI._printSettings._headerFontStyle = i;

			if (element->Attribute(TEXT("footerFontStyle"), &i))
				_nppGUI._printSettings._footerFontStyle = i;

			if (element->Attribute(TEXT("headerFontSize"), &i))
				_nppGUI._printSettings._headerFontSize = i;

			if (element->Attribute(TEXT("footerFontSize"), &i))
				_nppGUI._printSettings._footerFontSize = i;


			if (element->Attribute(TEXT("margeLeft"), &i))
				_nppGUI._printSettings._marge.left = i;

			if (element->Attribute(TEXT("margeTop"), &i))
				_nppGUI._printSettings._marge.top = i;

			if (element->Attribute(TEXT("margeRight"), &i))
				_nppGUI._printSettings._marge.right = i;

			if (element->Attribute(TEXT("margeBottom"), &i))
				_nppGUI._printSettings._marge.bottom = i;
		}

		else if (!lstrcmp(nm, TEXT("ScintillaPrimaryView")))
		{
			feedScintillaParam(SCIV_PRIMARY, element);
		}
		else if (!lstrcmp(nm, TEXT("ScintillaSecondaryView")))
		{
			feedScintillaParam(SCIV_SECOND, element);
		}
		else if (!lstrcmp(nm, TEXT("Backup")))
		{
			int i;
			if (element->Attribute(TEXT("action"), &i))
				_nppGUI._backup = (BackupFeature)i;

			const TCHAR *bDir = element->Attribute(TEXT("useCustumDir"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._useDir = true;
			}
			const TCHAR *pDir = element->Attribute(TEXT("dir"));
			if (pDir)
				lstrcpy(_nppGUI._backupDir, pDir);
		}
		
		else if (!lstrcmp(nm, TEXT("globalOverride")))
		{
			const TCHAR *bDir = element->Attribute(TEXT("fg"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableFg = true;
			}

			bDir = element->Attribute(TEXT("bg"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableBg = true;
			}

			bDir = element->Attribute(TEXT("font"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableFont = true;
			}

			bDir = element->Attribute(TEXT("fontSize"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableFontSize = true;
			}

			bDir = element->Attribute(TEXT("bold"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableBold = true;
			}

			bDir = element->Attribute(TEXT("italic"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableItalic = true;
			}

			bDir = element->Attribute(TEXT("underline"));
			if (bDir && !lstrcmp(bDir, TEXT("yes"))) 
			{
				_nppGUI._globalOverride.enableUnderLine = true;
			}
		}
		else if (!lstrcmp(nm, TEXT("auto-completion")))
		{
			int i;
			if (element->Attribute(TEXT("autoCAction"), &i))
				_nppGUI._autocStatus = (NppGUI::AutocStatus)i;

			if (element->Attribute(TEXT("triggerFromNbChar"), &i))
				_nppGUI._autocFromLen = i;

			const TCHAR * funcParams = element->Attribute(TEXT("funcParams"));
			if (funcParams && !lstrcmp(funcParams, TEXT("yes"))) 
			{
				_nppGUI._funcParams = true;
			}
		}
		else if (!lstrcmp(nm, TEXT("noUpdate")))
		{
			TiXmlNode *n = childNode->FirstChild();
			if (n)
			{
				val = n->Value();
				if (val)
				{
					_nppGUI._neverUpdate = (!lstrcmp(val, TEXT("yes")))?true:false;
				}
			}
		}
		else if (!lstrcmp(nm, TEXT("openSaveDir")))
		{
			const TCHAR * value = element->Attribute(TEXT("value"));
			if (value && value[0])
			{
				if (lstrcmp(value, TEXT("1")) == 0)
					_nppGUI._openSaveDir = dir_last;
				else if (lstrcmp(value, TEXT("2")) == 0)
					_nppGUI._openSaveDir = dir_userDef;
				else
					_nppGUI._openSaveDir = dir_followCurrent;
			}
			const TCHAR * path = element->Attribute(TEXT("defaultDirPath"));
			if (path && path[0])
			{
				lstrcpyn(_nppGUI._defaultDir, path, MAX_PATH);
				::ExpandEnvironmentStrings(_nppGUI._defaultDir, _nppGUI._defaultDirExp, 500);
			}
 		}
	}
}

void NppParameters::feedScintillaParam(bool whichOne, TiXmlNode *node)
{
    TiXmlElement *element = node->ToElement();

    // Line Number Margin
    const TCHAR *nm = element->Attribute(TEXT("lineNumberMargin"));
	if (nm) 
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._lineNumberMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._lineNumberMarginShow = false;
	}

	// Bookmark Margin
	nm = element->Attribute(TEXT("bookMarkMargin"));
	if (nm) 
	{

		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._bookMarkMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._bookMarkMarginShow = false;
	}

	// doc change state Margin
	nm = element->Attribute(TEXT("docChangeStateMargin"));
	if (nm) 
	{

		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._docChangeStateMarginShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._docChangeStateMarginShow = false;
	}

    // Indent GuideLine 
    nm = element->Attribute(TEXT("indentGuideLine"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._indentGuideLineShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._indentGuideLineShow= false;
	}

    // Folder Mark Style
    nm = element->Attribute(TEXT("folderMarkStyle"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("box")))
			_svp[whichOne]._folderStyle = FOLDER_STYLE_BOX;
		else if (!lstrcmp(nm, TEXT("circle")))
			_svp[whichOne]._folderStyle = FOLDER_STYLE_CIRCLE;
		else if (!lstrcmp(nm, TEXT("arrow")))
			_svp[whichOne]._folderStyle = FOLDER_STYLE_ARROW;
		else if (!lstrcmp(nm, TEXT("simple")))
			_svp[whichOne]._folderStyle = FOLDER_STYLE_SIMPLE;
	}

    // Current Line Highlighting State
    nm = element->Attribute(TEXT("currentLineHilitingShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._currentLineHilitingShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._currentLineHilitingShow = false;
	}

    // Current wrap symbol visibility State
    nm = element->Attribute(TEXT("wrapSymbolShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._wrapSymbolShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._wrapSymbolShow = false;
	}

	// Do Wrap
    nm = element->Attribute(TEXT("Wrap"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("yes")))
			_svp[whichOne]._doWrap = true;
		else if (!lstrcmp(nm, TEXT("no")))
			_svp[whichOne]._doWrap = false;
	}

	// Do Edge
    nm = element->Attribute(TEXT("edge"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("background")))
			_svp[whichOne]._edgeMode = EDGE_BACKGROUND;
		else if (!lstrcmp(nm, TEXT("line")))
			_svp[whichOne]._edgeMode = EDGE_LINE;
		else
			_svp[whichOne]._edgeMode = EDGE_NONE;
	}
	
	int val;
	nm = element->Attribute(TEXT("edgeNbColumn"), &val);
	if (nm)
	{
		_svp[whichOne]._edgeNbColumn = val;
	}

	nm = element->Attribute(TEXT("zoom"), &val);
	if (nm)
	{
		_svp[whichOne]._zoom = val;
	}

	// White Space visibility State
    nm = element->Attribute(TEXT("whiteSpaceShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._whiteSpaceShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._whiteSpaceShow = false;
	}

	// EOL visibility State
    nm = element->Attribute(TEXT("eolShow"));
	if (nm)
	{
		if (!lstrcmp(nm, TEXT("show")))
			_svp[whichOne]._eolShow = true;
		else if (!lstrcmp(nm, TEXT("hide")))
			_svp[whichOne]._eolShow = false;
	}
}

bool NppParameters::writeScintillaParams(const ScintillaViewParams & svp, bool whichOne) 
{
	if (!_pXmlUserDoc) return false;

	const TCHAR *pViewName = (whichOne == SCIV_PRIMARY)?TEXT("ScintillaPrimaryView"):TEXT("ScintillaSecondaryView");
	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("OwmScite"));
	if (!nppRoot) return false;

	TiXmlNode *configsRoot = nppRoot->FirstChildElement(TEXT("GUIConfigs"));
	if (!configsRoot) return false;

	TiXmlNode *scintNode = getChildElementByAttribut(configsRoot, TEXT("GUIConfig"), TEXT("name"), pViewName);
	if (!scintNode) return false;

	(scintNode->ToElement())->SetAttribute(TEXT("lineNumberMargin"), svp._lineNumberMarginShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("bookMarkMargin"), svp._bookMarkMarginShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("docChangeStateMargin"), svp._docChangeStateMarginShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("indentGuideLine"), svp._indentGuideLineShow?TEXT("show"):TEXT("hide"));
	const TCHAR *pFolderStyleStr = (svp._folderStyle == FOLDER_STYLE_SIMPLE)?TEXT("simple"):
									(svp._folderStyle == FOLDER_STYLE_ARROW)?TEXT("arrow"):
										(svp._folderStyle == FOLDER_STYLE_CIRCLE)?TEXT("circle"):TEXT("box");
	(scintNode->ToElement())->SetAttribute(TEXT("folderMarkStyle"), pFolderStyleStr);
	(scintNode->ToElement())->SetAttribute(TEXT("currentLineHilitingShow"), svp._currentLineHilitingShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("wrapSymbolShow"), svp._wrapSymbolShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("Wrap"), svp._doWrap?TEXT("yes"):TEXT("no"));
	TCHAR *edgeStr = NULL;
	if (svp._edgeMode == EDGE_NONE)
		edgeStr = TEXT("no");
	else if (svp._edgeMode == EDGE_LINE)
		edgeStr = TEXT("line");
	else
		edgeStr = TEXT("background");
	(scintNode->ToElement())->SetAttribute(TEXT("edge"), edgeStr);
	(scintNode->ToElement())->SetAttribute(TEXT("edgeNbColumn"), svp._edgeNbColumn);
	(scintNode->ToElement())->SetAttribute(TEXT("zoom"), svp._zoom);
	(scintNode->ToElement())->SetAttribute(TEXT("whiteSpaceShow"), svp._whiteSpaceShow?TEXT("show"):TEXT("hide"));
	(scintNode->ToElement())->SetAttribute(TEXT("eolShow"), svp._eolShow?TEXT("show"):TEXT("hide"));

	return true;
}

bool NppParameters::writeGUIParams()
{
	if (!_pXmlUserDoc) return false;

	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("OwmScite"));
	if (!nppRoot) return false;

	TiXmlNode *GUIRoot = nppRoot->FirstChildElement(TEXT("GUIConfigs"));
	if (!GUIRoot) return false;

	bool autoDetectionExist = false;
	bool rememberLastSessionExist = false;
	bool newDocDefaultSettingsExist = false;
	bool langsExcludedLstExist = false;
	bool printSettingExist = false;
	bool doTaskListExist = false;
	bool maitainIndentExist = false;
	bool backExist = false;
	bool URLExist = false;
	bool globalOverrideExist = false;
	bool autocExist = false;
	bool noUpdateExist = false;
	bool smartHighLightExist = false;
	bool tagsMatchHighLightExist = false;
	bool caretExist = false;
	bool openSaveDirExist = false;

	TiXmlNode *dockingParamNode = NULL;

	for (TiXmlNode *childNode = GUIRoot->FirstChildElement(TEXT("GUIConfig"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("GUIConfig")))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
		if (!nm) continue;

		if (!lstrcmp(nm, TEXT("ScintillaViewsSplitter")))
		{
			const TCHAR *pStr = _nppGUI._splitterPos == POS_VERTICAL?TEXT("vertical"):TEXT("horizontal");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("UserDefineDlg")))
		{
			const TCHAR *pStr = _nppGUI._userDefineDlgStatus & UDD_SHOW?TEXT("show"):TEXT("hide");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
			
			pStr = (_nppGUI._userDefineDlgStatus & UDD_DOCKED)?TEXT("docked"):TEXT("undocked");
			element->SetAttribute(TEXT("position"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("TabSetting")))
		{
			const TCHAR *pStr = _nppGUI._tabReplacedBySpace?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("replaceBySpace"), pStr);
			element->SetAttribute(TEXT("size"), _nppGUI._tabSize);
		}
		else if (!lstrcmp(nm, TEXT("Caret")))
		{
			caretExist = true;
			element->SetAttribute(TEXT("width"), _nppGUI._caretWidth);
			element->SetAttribute(TEXT("blinkRate"), _nppGUI._caretBlinkRate);
		}
		else if (!lstrcmp(nm, TEXT("Auto-detection")))
		{
			autoDetectionExist = true;
			const TCHAR *pStr = TEXT("no");
			switch (_nppGUI._fileAutoDetection)
			{
				case cdEnabled:
					pStr = TEXT("yes");
					break;
				case cdAutoUpdate:
					pStr = TEXT("auto");
					break;
				case cdGo2end:
					pStr = TEXT("Update2End");
					break;
				case cdAutoUpdateGo2end:
					pStr = TEXT("autoUpdate2End");
					break;
			}
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}

		else if (!lstrcmp(nm, TEXT("MaitainIndent")))
		{
			maitainIndentExist = true;
			const TCHAR *pStr = _nppGUI._maitainIndent?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("SmartHighLight")))
		{
			smartHighLightExist = true;
			const TCHAR *pStr = _nppGUI._enableSmartHilite?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}

		else if (!lstrcmp(nm, TEXT("TagsMatchHighLight")))
		{
			tagsMatchHighLightExist = true;
			const TCHAR *pStr = _nppGUI._enableTagsMatchHilite?TEXT("yes"):TEXT("no");
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));

			(childNode->ToElement())->SetAttribute(TEXT("TagAttrHighLight"), _nppGUI._enableTagAttrsHilite?TEXT("yes"):TEXT("no"));
			(childNode->ToElement())->SetAttribute(TEXT("HighLightNonHtmlZone"), _nppGUI._enableHiliteNonHTMLZone?TEXT("yes"):TEXT("no"));
		}

		else if (!lstrcmp(nm, TEXT("TaskList")))
		{
			doTaskListExist = true;
			const TCHAR *pStr = _nppGUI._doTaskList?TEXT("yes"):TEXT("no");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("NewDocDefaultSettings")))
		{
			element->SetAttribute(TEXT("format"), _nppGUI._newDocDefaultSettings._format);
			element->SetAttribute(TEXT("encoding"), _nppGUI._newDocDefaultSettings._encoding);
			element->SetAttribute(TEXT("lang"), _nppGUI._newDocDefaultSettings._lang);
			element->SetAttribute(TEXT("openAnsiAsUTF8"), _nppGUI._newDocDefaultSettings._openAnsiAsUtf8?TEXT("yes"):TEXT("no"));
			newDocDefaultSettingsExist = true;
		}
		else if (!lstrcmp(nm, TEXT("langsExcluded")))
		{
			writeExcludedLangList(element);
			langsExcludedLstExist = true;
		}
		else if (!lstrcmp(nm, TEXT("Print")))
		{
			writePrintSetting(element);
			printSettingExist = true;
		}
		else if (!lstrcmp(nm, TEXT("Backup")))
		{
			element->SetAttribute(TEXT("action"), _nppGUI._backup);
			element->SetAttribute(TEXT("useCustumDir"), _nppGUI._useDir?TEXT("yes"):TEXT("no"));
			element->SetAttribute(TEXT("dir"), _nppGUI._backupDir);
			backExist = true;
		}
		else if (!lstrcmp(nm, TEXT("URL")))
		{
			URLExist = true;
			const TCHAR *pStr = TEXT("0");
			if (_nppGUI._styleURL == 1)
				pStr = TEXT("1");
			else if (_nppGUI._styleURL == 2)
				pStr = TEXT("2");

			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("globalOverride")))
		{
			globalOverrideExist = true;
			const TCHAR *pStr = _nppGUI._globalOverride.enableFg?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("fg"), pStr);

			pStr = (_nppGUI._globalOverride.enableBg)?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("bg"), pStr);

			pStr = _nppGUI._globalOverride.enableFont?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("font"), pStr);

			pStr = _nppGUI._globalOverride.enableFontSize?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("fontSize"), pStr);

			pStr = _nppGUI._globalOverride.enableBold?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("bold"), pStr);

			pStr = _nppGUI._globalOverride.enableItalic?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("italic"), pStr);

			pStr = _nppGUI._globalOverride.enableUnderLine?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("underline"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("auto-completion")))
		{
			autocExist = true;
			element->SetAttribute(TEXT("autoCAction"), _nppGUI._autocStatus);
			element->SetAttribute(TEXT("triggerFromNbChar"), _nppGUI._autocFromLen);
			const TCHAR * pStr = _nppGUI._funcParams?TEXT("yes"):TEXT("no");
			element->SetAttribute(TEXT("funcParams"), pStr);
		}
		else if (!lstrcmp(nm, TEXT("noUpdate")))
		{
			noUpdateExist = true;
			const TCHAR *pStr = _nppGUI._neverUpdate?TEXT("yes"):TEXT("no");
			
			TiXmlNode *n = childNode->FirstChild();
			if (n)
				n->SetValue(pStr);
			else
				childNode->InsertEndChild(TiXmlText(pStr));
		}
		else if (!lstrcmp(nm, TEXT("openSaveDir")))
		{
			openSaveDirExist = true;
			element->SetAttribute(TEXT("value"), _nppGUI._openSaveDir);
			element->SetAttribute(TEXT("defaultDirPath"), _nppGUI._defaultDir);
		}
	}

	if (!noUpdateExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("noUpdate"), _nppGUI._neverUpdate);
	}

	if (!autoDetectionExist)
	{
		const TCHAR *pStr = TEXT("no");
		switch (_nppGUI._fileAutoDetection)
		{
			case cdEnabled:
				pStr = TEXT("yes");
				break;
			case cdAutoUpdate:
				pStr = TEXT("auto");
				break;
			case cdGo2end:
				pStr = TEXT("Update2End");
				break;
			case cdAutoUpdateGo2end:
				pStr = TEXT("autoUpdate2End");
				break;
		}
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Auto-detection"));
		GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	}
	
	if (!maitainIndentExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("MaitainIndent"), _nppGUI._maitainIndent);
	}

	if (!smartHighLightExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("SmartHighLight"), _nppGUI._enableSmartHilite);
	}
	if (!tagsMatchHighLightExist)
	{
		TiXmlElement * ele = insertGUIConfigBoolNode(GUIRoot, TEXT("TagsMatchHighLight"), _nppGUI._enableTagsMatchHilite);
		ele->SetAttribute(TEXT("TagAttrHighLight"), _nppGUI._enableTagAttrsHilite?TEXT("yes"):TEXT("no"));
		ele->SetAttribute(TEXT("HighLightNonHtmlZone"), _nppGUI._enableHiliteNonHTMLZone?TEXT("yes"):TEXT("no"));
	}

	if (!newDocDefaultSettingsExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("NewDocDefaultSettings"));
		GUIConfigElement->SetAttribute(TEXT("format"), _nppGUI._newDocDefaultSettings._format);
		GUIConfigElement->SetAttribute(TEXT("encoding"), _nppGUI._newDocDefaultSettings._encoding);
		GUIConfigElement->SetAttribute(TEXT("lang"), _nppGUI._newDocDefaultSettings._lang);
		GUIConfigElement->SetAttribute(TEXT("openAnsiAsUTF8"), _nppGUI._newDocDefaultSettings._openAnsiAsUtf8?TEXT("yes"):TEXT("no"));
	}

	if (!langsExcludedLstExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("langsExcluded"));
		writeExcludedLangList(GUIConfigElement);
	}

	if (!printSettingExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Print"));
		writePrintSetting(GUIConfigElement);
	}
	if (!backExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Backup"));
		GUIConfigElement->SetAttribute(TEXT("action"), _nppGUI._backup);
		GUIConfigElement->SetAttribute(TEXT("useCustumDir"), _nppGUI._useDir?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("dir"), _nppGUI._backupDir);
	}

	if (!doTaskListExist)
	{
		insertGUIConfigBoolNode(GUIRoot, TEXT("TaskList"), _nppGUI._doTaskList);
	}

	if (!URLExist)
	{
		const TCHAR *pStr = TEXT("0");
		if (_nppGUI._styleURL == 1)
			pStr = TEXT("1");
		else if (_nppGUI._styleURL == 2)
			pStr = TEXT("2");

		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("URL"));
		GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	}
	
	if (!globalOverrideExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("globalOverride"));
		GUIConfigElement->SetAttribute(TEXT("fg"), _nppGUI._globalOverride.enableFg?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("bg"), _nppGUI._globalOverride.enableBg?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("font"), _nppGUI._globalOverride.enableFont?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("fontSize"), _nppGUI._globalOverride.enableFontSize?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("bold"), _nppGUI._globalOverride.enableBold?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("italic"), _nppGUI._globalOverride.enableItalic?TEXT("yes"):TEXT("no"));
		GUIConfigElement->SetAttribute(TEXT("underline"), _nppGUI._globalOverride.enableUnderLine?TEXT("yes"):TEXT("no"));
	}
	
	if (!autocExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("auto-completion"));
		GUIConfigElement->SetAttribute(TEXT("autoCAction"), _nppGUI._autocStatus);
		GUIConfigElement->SetAttribute(TEXT("triggerFromNbChar"), _nppGUI._autocFromLen);
		const TCHAR * pStr = _nppGUI._funcParams?TEXT("yes"):TEXT("no");
		GUIConfigElement->SetAttribute(TEXT("funcParams"), pStr);
		autocExist = true;
	}

	if (dockingParamNode)
	{
		// Rase tout
		GUIRoot->RemoveChild(dockingParamNode);
	}

	if (!caretExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("Caret"));
		GUIConfigElement->SetAttribute(TEXT("width"), _nppGUI._caretWidth);
		GUIConfigElement->SetAttribute(TEXT("blinkRate"), _nppGUI._caretBlinkRate);
	}

	if (!openSaveDirExist)
	{
		TiXmlElement *GUIConfigElement = (GUIRoot->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
		GUIConfigElement->SetAttribute(TEXT("name"), TEXT("openSaveDir"));
		GUIConfigElement->SetAttribute(TEXT("value"), _nppGUI._openSaveDir);
		GUIConfigElement->SetAttribute(TEXT("defaultDirPath"), _nppGUI._defaultDir);
	}

	return true;
}

bool NppParameters::writeFindHistory()
{
	if (!_pXmlUserDoc) return false;

	TiXmlNode *nppRoot = _pXmlUserDoc->FirstChild(TEXT("OwmScite"));
	if (!nppRoot) return false;

	TiXmlNode *findHistoryRoot = nppRoot->FirstChildElement(TEXT("FindHistory"));
	if (!findHistoryRoot)
	{
		TiXmlElement element(TEXT("FindHistory"));
		findHistoryRoot = nppRoot->InsertEndChild(element);
	}

	int i;

	findHistoryRoot->Clear();

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryPath"),    _findHistory._nbMaxFindHistoryPath);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryFilter"),  _findHistory._nbMaxFindHistoryFilter);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryFind"),    _findHistory._nbMaxFindHistoryFind);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("nbMaxFindHistoryReplace"), _findHistory._nbMaxFindHistoryReplace);

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("matchWord"),				_findHistory._isMatchWord?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("matchCase"),				_findHistory._isMatchCase?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("wrap"),					_findHistory._isWrap?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("directionDown"),			_findHistory._isDirectionDown?TEXT("yes"):TEXT("no"));

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifRecuisive"),			_findHistory._isFifRecuisive?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("fifInHiddenFolder"),		_findHistory._isFifInHiddenFolder?TEXT("yes"):TEXT("no"));
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("dlgAlwaysVisible"), _findHistory._isDlgAlwaysVisible?TEXT("yes"):TEXT("no"));

	(findHistoryRoot->ToElement())->SetAttribute(TEXT("searchMode"), _findHistory._searchMode);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("transparencyMode"), _findHistory._transparencyMode);
	(findHistoryRoot->ToElement())->SetAttribute(TEXT("transparency"), _findHistory._transparency);

	TiXmlElement hist_element(TEXT(""));

	hist_element.SetValue(TEXT("Path"));
	for (i = 0; i < _findHistory._nbFindHistoryPath; i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._pFindHistoryPath[i]->c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Filter"));
	for (i = 0; i < _findHistory._nbFindHistoryFilter; i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._pFindHistoryFilter[i]->c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Find"));
	for (i = 0; i < _findHistory._nbFindHistoryFind; i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._pFindHistoryFind[i]->c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	hist_element.SetValue(TEXT("Replace"));
	for (i = 0; i < _findHistory._nbFindHistoryReplace; i++)
	{
		(hist_element.ToElement())->SetAttribute(TEXT("name"), _findHistory._pFindHistoryReplace[i]->c_str());
		findHistoryRoot->InsertEndChild(hist_element);
	}

	return true;
}

void NppParameters::writePrintSetting(TiXmlElement *element)
{
	const TCHAR *pStr = _nppGUI._printSettings._printLineNumber?TEXT("yes"):TEXT("no");
	element->SetAttribute(TEXT("lineNumber"), pStr);

	element->SetAttribute(TEXT("printOption"), _nppGUI._printSettings._printOption);

	element->SetAttribute(TEXT("headerLeft"), _nppGUI._printSettings._headerLeft.c_str());
	element->SetAttribute(TEXT("headerMiddle"), _nppGUI._printSettings._headerMiddle.c_str());
	element->SetAttribute(TEXT("headerRight"), _nppGUI._printSettings._headerRight.c_str());
	element->SetAttribute(TEXT("footerLeft"), _nppGUI._printSettings._footerLeft.c_str());
	element->SetAttribute(TEXT("footerMiddle"), _nppGUI._printSettings._footerMiddle.c_str());
	element->SetAttribute(TEXT("footerRight"), _nppGUI._printSettings._footerRight.c_str());

	element->SetAttribute(TEXT("headerFontName"), _nppGUI._printSettings._headerFontName.c_str());
	element->SetAttribute(TEXT("headerFontStyle"), _nppGUI._printSettings._headerFontStyle);
	element->SetAttribute(TEXT("headerFontSize"), _nppGUI._printSettings._headerFontSize);
	element->SetAttribute(TEXT("footerFontName"), _nppGUI._printSettings._footerFontName.c_str());
	element->SetAttribute(TEXT("footerFontStyle"), _nppGUI._printSettings._footerFontStyle);
	element->SetAttribute(TEXT("footerFontSize"), _nppGUI._printSettings._footerFontSize);

	element->SetAttribute(TEXT("margeLeft"), _nppGUI._printSettings._marge.left);
	element->SetAttribute(TEXT("margeRight"), _nppGUI._printSettings._marge.right);
	element->SetAttribute(TEXT("margeTop"), _nppGUI._printSettings._marge.top);
	element->SetAttribute(TEXT("margeBottom"), _nppGUI._printSettings._marge.bottom);
}

void NppParameters::writeExcludedLangList(TiXmlElement *element)
{
	int g0 = 0; // up to  8
	int g1 = 0; // up to 16
	int g2 = 0; // up to 24
	int g3 = 0; // up to 32
	int g4 = 0; // up to 40
	int g5 = 0; // up to 48
	int g6 = 0; // up to 56
	int g7 = 0; // up to 64

	for (size_t i = 0 ; i < _nppGUI._excludedLangList.size() ; i++)
	{
		LangType langType = _nppGUI._excludedLangList[i]._langType;
		if (langType >= L_EXTERNAL && langType < L_END)
			continue;

		int nGrp = langType / 8;
		int nMask = 1 << langType % 8;


		switch (nGrp)
		{
			case 0 :
				g0 |= nMask;
				break;
			case 1 :
				g1 |= nMask;
				break;
			case 2 :
				g2 |= nMask;
				break;
			case 3 :
				g3 |= nMask;
				break;
			case 4 :
				g4 |= nMask;
				break;
			case 5 :
				g5 |= nMask;
				break;
			case 6 :
				g6 |= nMask;
				break;
			case 7 :
				g7 |= nMask;
				break;
		}
	}

	element->SetAttribute(TEXT("gr0"), g0);
	element->SetAttribute(TEXT("gr1"), g1);
	element->SetAttribute(TEXT("gr2"), g2);
	element->SetAttribute(TEXT("gr3"), g3);
	element->SetAttribute(TEXT("gr4"), g4);
	element->SetAttribute(TEXT("gr5"), g5);
	element->SetAttribute(TEXT("gr6"), g6);
	element->SetAttribute(TEXT("gr7"), g7);
}

TiXmlElement * NppParameters::insertGUIConfigBoolNode(TiXmlNode *r2w, const TCHAR *name, bool bVal)
{
	const TCHAR *pStr = bVal?TEXT("yes"):TEXT("no");
	TiXmlElement *GUIConfigElement = (r2w->InsertEndChild(TiXmlElement(TEXT("GUIConfig"))))->ToElement();
	GUIConfigElement->SetAttribute(TEXT("name"), name);
	GUIConfigElement->InsertEndChild(TiXmlText(pStr));
	return GUIConfigElement;
}

int RGB2int(COLORREF color) {
    return (((((DWORD)color) & 0x0000FF) << 16) | ((((DWORD)color) & 0x00FF00)) | ((((DWORD)color) & 0xFF0000) >> 16));
}

void NppParameters::writeStyles(LexerStylerArray & lexersStylers, StyleArray & globalStylers)
{
	TiXmlNode *lexersRoot = (_pXmlUserStylerDoc->FirstChild(TEXT("OwmScite")))->FirstChildElement(TEXT("LexerStyles"));
	for (TiXmlNode *childNode = lexersRoot->FirstChildElement(TEXT("LexerType"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("LexerType")))
	{
		TiXmlElement *element = childNode->ToElement();
		const TCHAR *nm = element->Attribute(TEXT("name"));
		
		LexerStyler *pLs = _lexerStylerArray.getLexerStylerByName(nm);
        LexerStyler *pLs2 = lexersStylers.getLexerStylerByName(nm);

		if (pLs) 
		{
			const TCHAR *extStr = pLs->getLexerUserExt();
			element->SetAttribute(TEXT("ext"), extStr);
			for (TiXmlNode *grChildNode = childNode->FirstChildElement(TEXT("WordsStyle"));
					grChildNode ;
					grChildNode = grChildNode->NextSibling(TEXT("WordsStyle")))
			{
				TiXmlElement *grElement = grChildNode->ToElement();
                const TCHAR *styleName = grElement->Attribute(TEXT("name"));

                int i = pLs->getStylerIndexByName(styleName);
                if (i != -1)
				{
                    Style & style = pLs->getStyler(i);
                    Style & style2Sync = pLs2->getStyler(i);

                    writeStyle2Element(style, style2Sync, grElement);
                }
			}
		}
	}
	
	for(size_t x = 0; x < _pXmlExternalLexerDoc.size(); x++)
	{
		TiXmlNode *lexersRoot = ( _pXmlExternalLexerDoc[x]->FirstChild(TEXT("OwmScite")))->FirstChildElement(TEXT("LexerStyles"));
		for (TiXmlNode *childNode = lexersRoot->FirstChildElement(TEXT("LexerType"));
			childNode ;
			childNode = childNode->NextSibling(TEXT("LexerType")))
		{
			TiXmlElement *element = childNode->ToElement();
			const TCHAR *nm = element->Attribute(TEXT("name"));
			
			LexerStyler *pLs = _lexerStylerArray.getLexerStylerByName(nm);
			LexerStyler *pLs2 = lexersStylers.getLexerStylerByName(nm);

			if (pLs) 
			{
				const TCHAR *extStr = pLs->getLexerUserExt();
				element->SetAttribute(TEXT("ext"), extStr);

				for (TiXmlNode *grChildNode = childNode->FirstChildElement(TEXT("WordsStyle"));
						grChildNode ;
						grChildNode = grChildNode->NextSibling(TEXT("WordsStyle")))
				{
					TiXmlElement *grElement = grChildNode->ToElement();
					const TCHAR *styleName = grElement->Attribute(TEXT("name"));

					int i = pLs->getStylerIndexByName(styleName);
					if (i != -1)
					{
						Style & style = pLs->getStyler(i);
						Style & style2Sync = pLs2->getStyler(i);

						writeStyle2Element(style, style2Sync, grElement);
					}
				}
			}
		}
		_pXmlExternalLexerDoc[x]->SaveFile();
	}

	TiXmlNode *globalStylesRoot = (_pXmlUserStylerDoc->FirstChild(TEXT("OwmScite")))->FirstChildElement(TEXT("GlobalStyles"));

    for (TiXmlNode* childNode = globalStylesRoot->FirstChildElement(TEXT("WidgetStyle"));
		childNode ;
		childNode = childNode->NextSibling(TEXT("WidgetStyle")))
    {
        TiXmlElement *pElement = childNode->ToElement();
        const TCHAR *styleName = pElement->Attribute(TEXT("name"));
        int i = _widgetStyleArray.getStylerIndexByName(styleName);

        if (i != -1)
		{
            Style & style = _widgetStyleArray.getStyler(i);
            Style & style2Sync = globalStylers.getStyler(i);

            writeStyle2Element(style, style2Sync, pElement);
        }
    }

	_pXmlUserStylerDoc->SaveFile();
}

void NppParameters::writeStyle2Element(Style & style2Wite, Style & style2Sync, TiXmlElement *element)
{
    const TCHAR *styleName = element->Attribute(TEXT("name"));

    if (HIBYTE(HIWORD(style2Wite._fgColor)) != 0xFF)
    {
        int rgbVal = RGB2int(style2Wite._fgColor);
	    TCHAR fgStr[7];
	    wsprintf(fgStr, TEXT("%.6X"), rgbVal);
	    element->SetAttribute(TEXT("fgColor"), fgStr);
    }

    if (HIBYTE(HIWORD(style2Wite._bgColor)) != 0xFF)
    {
        int rgbVal = RGB2int(style2Wite._bgColor);
	    TCHAR bgStr[7];
	    wsprintf(bgStr, TEXT("%.6X"), rgbVal);
	    element->SetAttribute(TEXT("bgColor"), bgStr);
    }

	if (style2Wite._colorStyle != COLORSTYLE_ALL)
    {
	    element->SetAttribute(TEXT("colorStyle"), style2Wite._colorStyle);
    }

    if (style2Wite._fontName)
    {
        const TCHAR *oldFontName = element->Attribute(TEXT("fontName"));
        if (lstrcmp(oldFontName, style2Wite._fontName))
        {
		    element->SetAttribute(TEXT("fontName"), style2Wite._fontName);
            style2Sync._fontName = style2Wite._fontName = element->Attribute(TEXT("fontName"));
        }
    }

    if (style2Wite._fontSize != -1)
    {
        if (!style2Wite._fontSize)
            element->SetAttribute(TEXT("fontSize"), TEXT(""));
        else
		    element->SetAttribute(TEXT("fontSize"), style2Wite._fontSize);
    }

    if (style2Wite._fontStyle != -1)
    {
	    element->SetAttribute(TEXT("fontStyle"), style2Wite._fontStyle);
    }

	
	if (style2Wite._keywords)
    {	
		TiXmlNode *teteDeNoeud = element->LastChild();

		if (teteDeNoeud)
			teteDeNoeud->SetValue(style2Wite._keywords->c_str());
		else 
			element->InsertEndChild(TiXmlText(style2Wite._keywords->c_str()));
    }
}
/*
void NppParameters::insertUserLang2Tree(TiXmlNode *node, UserLangContainer *userLang) 
{
	TiXmlElement *rootElement = (node->InsertEndChild(TiXmlElement(TEXT("UserLang"))))->ToElement();

	rootElement->SetAttribute(TEXT("name"), userLang->_name);
	rootElement->SetAttribute(TEXT("ext"), userLang->_ext);
	TiXmlElement *settingsElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("Settings"))))->ToElement();
	{
		TiXmlElement *globalElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("Global"))))->ToElement();
		globalElement->SetAttribute(TEXT("caseIgnored"), userLang->_isCaseIgnored?TEXT("yes"):TEXT("no"));

		TiXmlElement *treatAsSymbolElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("TreatAsSymbol"))))->ToElement();
		treatAsSymbolElement->SetAttribute(TEXT("comment"), userLang->_isCommentSymbol?TEXT("yes"):TEXT("no"));
		treatAsSymbolElement->SetAttribute(TEXT("commentLine"), userLang->_isCommentLineSymbol?TEXT("yes"):TEXT("no"));

		TiXmlElement *prefixElement = (settingsElement->InsertEndChild(TiXmlElement(TEXT("Prefix"))))->ToElement();
		TCHAR names[nbPrefixListAllowed][7] = {TEXT("words1"), TEXT("words2"), TEXT("words3"), TEXT("words4")};
		for (int i = 0 ; i < nbPrefixListAllowed ; i++)
			prefixElement->SetAttribute(names[i], userLang->_isPrefix[i]?TEXT("yes"):TEXT("no"));
	}

	TiXmlElement *kwlElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("KeywordLists"))))->ToElement();

	const int nbKWL = 9;
	TCHAR kwn[nbKWL][16] = {TEXT("Delimiters"), TEXT("Folder+"), TEXT("Folder-"), TEXT("Operators"), TEXT("Comment"), TEXT("Words1"), TEXT("Words2"), TEXT("Words3"), TEXT("Words4")};
	for (int i = 0 ; i < nbKWL ; i++)
	{
		TiXmlElement *kwElement = (kwlElement->InsertEndChild(TiXmlElement(TEXT("Keywords"))))->ToElement();
		kwElement->SetAttribute(TEXT("name"), kwn[i]);
		kwElement->InsertEndChild(TiXmlText(userLang->_keywordLists[i]));
	}

	TiXmlElement *styleRootElement = (rootElement->InsertEndChild(TiXmlElement(TEXT("Styles"))))->ToElement();

	for (i = 0 ; i < userLang->_styleArray.getNbStyler() ; i++)
	{
		TiXmlElement *styleElement = (styleRootElement->InsertEndChild(TiXmlElement(TEXT("WordsStyle"))))->ToElement();
		Style style2Wite = userLang->_styleArray.getStyler(i);

		styleElement->SetAttribute(TEXT("name"), style2Wite._styleDesc);

		styleElement->SetAttribute(TEXT("styleID"), style2Wite._styleID);

		//if (HIBYTE(HIWORD(style2Wite._fgColor)) != 0xFF)
		{
			int rgbVal = RGB2int(style2Wite._fgColor);
			TCHAR fgStr[7];
			wsprintf(fgStr, TEXT("%.6X"), rgbVal);
			styleElement->SetAttribute(TEXT("fgColor"), fgStr);
		}

		//if (HIBYTE(HIWORD(style2Wite._bgColor)) != 0xFF)
		{
			int rgbVal = RGB2int(style2Wite._bgColor);
			TCHAR bgStr[7];
			wsprintf(bgStr, TEXT("%.6X"), rgbVal);
			styleElement->SetAttribute(TEXT("bgColor"), bgStr);
		}

		if (style2Wite._colorStyle != COLORSTYLE_ALL)
		{
			styleElement->SetAttribute(TEXT("colorStyle"), style2Wite._colorStyle);
		}

		if (style2Wite._fontName)
		{
			styleElement->SetAttribute(TEXT("fontName"), style2Wite._fontName);
		}

		if (style2Wite._fontStyle == -1)
		{
			styleElement->SetAttribute(TEXT("fontStyle"), TEXT("0"));
		}
		else
		{
			styleElement->SetAttribute(TEXT("fontStyle"), style2Wite._fontStyle);
		}

		if (style2Wite._fontSize != -1)
		{
			if (!style2Wite._fontSize)
				styleElement->SetAttribute(TEXT("fontSize"), TEXT(""));
			else
				styleElement->SetAttribute(TEXT("fontSize"), style2Wite._fontSize);
		}
	}
}
*/
void NppParameters::stylerStrOp(bool op) 
{
	for (int i = 0 ; i < _nbUserLang ; i++)
	{
		int nbStyler = _userLangArray[i]->_styleArray.getNbStyler();
		for (int j = 0 ; j < nbStyler ; j++)
		{
			Style & style = _userLangArray[i]->_styleArray.getStyler(j);
			
			if (op == DUP)
			{
				TCHAR *str = new TCHAR[lstrlen(style._styleDesc) + 1];
				style._styleDesc = lstrcpy(str, style._styleDesc);
				if (style._fontName)
				{
					str = new TCHAR[lstrlen(style._fontName) + 1];
					style._fontName = lstrcpy(str, style._fontName);
				}
				else
				{
					str = new TCHAR[2];
					str[0] = str[1] = '\0';
					style._fontName = str;
				}
			}
			else
			{
				//delete [] style._styleDesc;
				//delete [] style._fontName;
			}
		}
	}
}

void NppParameters::addUserModifiedIndex(int index) 
{
	size_t len = _customizedShortcuts.size();
	bool found = false;
	for(size_t i = 0; i < len; i++) 
	{
		if (_customizedShortcuts[i] == index) 
		{
			found = true;
			break;
		}
	}
	if (!found) 
	{
		_customizedShortcuts.push_back(index);
	}
}

void NppParameters::addPluginModifiedIndex(int index) 
{
	size_t len = _pluginCustomizedCmds.size();
	bool found = false;
	for(size_t i = 0; i < len; i++) 
	{
		if (_pluginCustomizedCmds[i] == index) 
		{
			found = true;
			break;
		}
	}
	if (!found) 
	{
		_pluginCustomizedCmds.push_back(index);
	}
}

void NppParameters::addScintillaModifiedIndex(int index) 
{
	size_t len = _scintillaModifiedKeyIndices.size();
	bool found = false;
	for(size_t i = 0; i < len; i++) 
	{
		if (_scintillaModifiedKeyIndices[i] == index) 
		{
			found = true;
			break;
		}
	}
	if (!found) 
	{
		_scintillaModifiedKeyIndices.push_back(index);
	}
}
