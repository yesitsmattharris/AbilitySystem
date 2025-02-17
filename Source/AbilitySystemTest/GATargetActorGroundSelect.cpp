// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
	PrimaryActorTick.bCanEverTick = true;
	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");

	SetRootComponent(RootComp);
	Decal->SetupAttachment(RootComp);

	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	Decal->DecalSize = FVector(Radius);
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

	/** Note: This is cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}

	if (OverlappedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation));
	}
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector OutLookPoint;
	bGetPlayerLookingPoint(OutLookPoint);
	Decal->SetWorldLocation(OutLookPoint);
}

bool AGATargetActorGroundSelect::bGetPlayerLookingPoint(FVector& ViewPoint)
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
		ViewPoint = OutHitResult.ImpactPoint;
	}
	else
	{
		ViewPoint = FVector();
	}

	return bTryTrace;
}
