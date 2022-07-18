// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include <SPickUpActor.h>
#include "SHealingPotion.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASHealingPotion : public ASPickUpActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealingPotion();

protected:

	UPROPERTY(EditAnywhere, Category = "Healing")
		float HealingAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn_TimeEclipse();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Healing(APawn* InstigatorPawn);
	//BlueprintNativeEvent Provide a body named [FunctionName]_Implementation instead of [FunctionName];
	void Interact_Implementation(APawn* InstigatorPawn) override;
};
