// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageExecutionCalculation.h"
#include "AttributeSetBase.h"

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour)
	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armour, Target, true);
	}
};

static DamageStatics& GetDamageStatics()
{
	static DamageStatics DamageStaticsVar;
	return DamageStaticsVar;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	HealthProperty = FindFieldChecked <UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health));
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true);

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AttackDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmourDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	// Examples of what other information can be obtained
	//UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	//UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	//const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float OutAttackDamageMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AttackDamageDef, FAggregatorEvaluateParameters(), OutAttackDamageMagnitude);
	float OutArmourMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmourDef, FAggregatorEvaluateParameters(), OutArmourMagnitude);

	// Calculation
	float FinalDamage = FMath::Clamp(OutAttackDamageMagnitude - OutArmourMagnitude, 0.0f, OutAttackDamageMagnitude - OutArmourMagnitude);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
