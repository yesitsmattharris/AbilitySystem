// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f),
	MaxHealth(200.0f),
	Mana(100.0f),
	MaxMana(100.0f),
	Strength(250.0f),
	MaxStrength(250.0f)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	// Check if attribute that has been changed by GE is Health
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}

	// Check if attribute that has been changed by GE is Mana
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	// Check if attribute that has been changed by GE is Strength
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strength)))
	{
		Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.f, MaxStrength.GetCurrentValue()));
		Strength.SetBaseValue(FMath::Clamp(Strength.GetBaseValue(), 0.f, MaxStrength.GetCurrentValue()));
		OnStrengthChange.Broadcast(Strength.GetCurrentValue(), MaxStrength.GetCurrentValue());
	}
}
