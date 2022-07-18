// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include <SAttributeComponent.h>

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("staticMesh");
	RootComponent = staticMesh;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	//Trigger when health changed (damage/healing)
	AttributeComp->OnHealthChange.AddDynamic(this,&ASTargetDummy::OnHealthChange);
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTargetDummy::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* Attricomp, float NewHealth, float Delta)
{
	if (Delta<0.f)
	{
		staticMesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

