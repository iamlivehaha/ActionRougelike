// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamePlayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASItemChest : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float targetPitch;

	//BlueprintNativeEvent Provide a body named [FunctionName]_Implementation instead of [FunctionName];
	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* LidMesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
