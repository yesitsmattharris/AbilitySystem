// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	// Check if attribute that has been changed by GE is Health
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		UE_LOG(LogTemp, Warning, TEXT("I took damage. My health is now %f"), Health.GetCurrentValue());
	}
}
