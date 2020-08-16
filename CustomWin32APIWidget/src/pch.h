#pragma once


/**
* Windows headers
*/
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

/**
* Common controls header
* Uses manifest file to link with version 6 of ComCtl32.dll which is the first version to support window styles (make it look more modern)
*/
#include <CommCtrl.h>
#pragma comment(lib, "ComCtl32.lib")

/**
* Header for visual themes
* Supported since Windows XP
*/
#include <Uxtheme.h>
#pragma comment(lib, "UxTheme.lib")

/**
* Windows SDK provides headers vssym32.h and vsstyle.h
* which provide some symbolic identifiers to the parts and states (enumerations)
* and also for properties (defined as preprocessor macros)
*/
#include <vssym32.h>
#include <vsstyle.h>


/**
* Common data structures
*/
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

/**
* Libraries
*/

