// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMTEST_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Health;

	UAttributeSetBase();

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};
