// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include <Math/UnrealMathUtility.h>

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
	HealthMax = 100;
}




bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributeComponent::IsHealthFull() const
{
	return Health >= HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(float delta)
{
	Health = FMath::Clamp(Health+delta, 0.f, HealthMax);
	OnHealthChange.Broadcast(nullptr, this, Health, delta);

	return true;
}

