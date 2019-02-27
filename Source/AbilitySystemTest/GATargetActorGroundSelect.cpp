// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector OutViewPoint;
	FRotator OutViewRotation;
	MasterPC->GetPlayerViewPoint(OutViewPoint, OutViewRotation);

	FHitResult OutHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	FVector LookAtPoint = FVector();
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(OutHitResult, OutViewPoint, OutViewPoint + OutViewRotation.Vector()*10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		LookAtPoint = OutHitResult.ImpactPoint;
	}
}