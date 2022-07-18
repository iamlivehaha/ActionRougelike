// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SPickUpActor.generated.h"
class USphereComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class ACTIONROUGELIKE_API ASPickUpActor : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpActor();
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "PickUp")
		float RespawnTime;
	UPROPERTY(VisibleAnywhere, Category = "PickUp")
		bool bIsActive;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_RespawnTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HideAndCooldown();

	void SetPickUpState(bool state);

	void ShowPickUp();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//BlueprintNativeEvent Provide a body named [FunctionName]_Implementation instead of [FunctionName];
	void Interact_Implementation(APawn* InstigatorPawn);
};
