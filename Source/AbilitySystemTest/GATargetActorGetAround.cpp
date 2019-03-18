// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGetAround.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"

void AGATargetActorGetAround::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGetAround::ConfirmTargetingAndContinue()
{
	APawn* OwningPawn = MasterPC->GetPawn();
	if (!OwningPawn) {
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		return;
	}

	FVector ViewLocation = OwningPawn->GetActorLocation();

	TArray<FOverlapResult> OutOverlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	bool bTraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = bTraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		CollisionQueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	bool bTryOverlap = GetWorld()->OverlapMultiByObjectType(
		OutOverlaps,
		ViewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParams
	);

	if (bTryOverlap)
	{
		for (int32 i = 0; i < OutOverlaps.Num(); i++)
		{
			APawn* PawnOverlapped = Cast<APawn>(OutOverlaps[i].GetActor());
			if (PawnOverlapped && !OverlappedActors.Contains(PawnOverlapped))
			{
				OverlappedActors.Add(PawnOverlapped);
			}
		}
	}

	if (OverlappedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}