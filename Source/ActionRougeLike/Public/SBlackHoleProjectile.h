// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasicProjectile.h"
#include "SBlackHoleProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROUGELIKE_API ASBlackHoleProjectile : public ASBasicProjectile
{
	GENERATED_BODY()
public:
	ASBlackHoleProjectile();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlackHole")
		float DestroyDelay;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDestroy;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestorySelf();

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
