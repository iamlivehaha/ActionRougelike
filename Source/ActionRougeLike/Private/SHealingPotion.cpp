// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealingPotion.h"
#include <SAttributeComponent.h>
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ASHealingPotion::ASHealingPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RespawnTime = 10.f;
	bIsActive = true;
	HealingAmount = 50;
}

// Called when the game starts or when spawned
void ASHealingPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASHealingPotion::Respawn_TimeEclipse()
{
	bIsActive = true;
	MeshComp->SetVisibility(true);
	SetActorEnableCollision(true);
}

// Called every frame
void ASHealingPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASHealingPotion::Healing(APawn* InstigatorPawn)
{
	if (bIsActive)
	{
		//USAttributeComponent::StaticClass()
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		
		if (AttributeComp&& !AttributeComp->IsHealthFull())
		{
			AttributeComp->ApplyHealthChange(HealingAmount);

			HideAndCooldown();

			UE_LOG(LogTemp, Warning, TEXT("Healing for Player%s with %d HP!"), *GetNameSafe(InstigatorPawn),HealingAmount);
		}
	}

}

void ASHealingPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		Healing(InstigatorPawn);
	}
}

