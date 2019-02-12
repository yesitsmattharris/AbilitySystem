// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemTestHUD.generated.h"

UCLASS()
class AAbilitySystemTestHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAbilitySystemTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

