// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ASPickUpActor::ASPickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;
	bIsActive = true;
	
}

// Called when the game starts or when spawned
void ASPickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPickUpActor::HideAndCooldown()
{
	SetPickUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickUpActor::ShowPickUp, RespawnTime);

}

void ASPickUpActor::SetPickUpState(bool state)
{
	MeshComp->SetVisibility(state,true);
	SetActorEnableCollision(state);
	bIsActive = state;
}

void ASPickUpActor::ShowPickUp()
{
	SetPickUpState(true);
}

// Called every frame
void ASPickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickUpActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

