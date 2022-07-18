// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHoleProjectile.h"
#include <PhysicsEngine/RadialForceComponent.h>
#include <SBasicProjectile.h>
#include <Components/SphereComponent.h>

ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComp;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
	//RadialForceComp->ImpulseStrength = 1000.0f;
	RadialForceComp->ForceStrength = -800000.0;
	RadialForceComp->Radius = 1000.0f;

	DestroyDelay = 5.f;

	SphereComp->SetSphereRadius(60);
}

void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDestroy, this, &ASBlackHoleProjectile::DestorySelf, DestroyDelay);
	AActor* instigator = GetInstigator();
	SphereComp->IgnoreActorWhenMoving(instigator, true);
}

void ASBlackHoleProjectile::Explode_Implementation()
{

}

void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
		{
			OtherActor->Destroy();
		}
	}
}

void ASBlackHoleProjectile::DestorySelf()
{
	Destroy();
}

void ASBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHoleProjectile::OnActorOverlap);
}

void ASBlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
