// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicProjectile.h"
#include "SDasgProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASDasgProjectile : public ASBasicProjectile
{
	GENERATED_BODY()
	
public:
	ASDasgProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
		float DetonateDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* TeleportEffectComp;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
