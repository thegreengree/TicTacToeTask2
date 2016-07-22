// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TTTP.h"
#include "TTTPPlayerController.h"

ATTTPPlayerController::ATTTPPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
