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
	FVector ViewLocation;
	bGetPlayerLookingPoint(ViewLocation);

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
}

bool AGATargetActorGroundSelect::bGetPlayerLookingPoint(OUT FVector& OutViewPoint)
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
	bool bTryTrace = GetWorld()->LineTraceSingleByChannel(OutHitResult, OutViewPoint, OutViewPoint + OutViewRotation.Vector()*10000.0f, ECC_Visibility, QueryParams);
	if (bTryTrace)
	{
		OutViewPoint = OutHitResult.ImpactPoint;
	}
	else
	{
		OutViewPoint = FVector();
	}

	return bTryTrace;
}
