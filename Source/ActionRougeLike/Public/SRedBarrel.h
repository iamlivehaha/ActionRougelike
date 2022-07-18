// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRedBarrel.generated.h"
class UStaticMesh;
class URadialForceComponent;

UCLASS()
class ACTIONROUGELIKE_API ASRedBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRedBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* staticMesh;
	UPROPERTY(BlueprintReadWrite)
	URadialForceComponent* radialForceComp;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		float ExplodeDamage;

	//Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay
	virtual void PostInitializeComponents() override;

	// Must be marked with ufunction in order to 'bind' the event
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
