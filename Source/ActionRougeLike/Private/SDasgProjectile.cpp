// Fill out your copyright notice in the Description page of Project Settings.


#include "SDasgProjectile.h"
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <SBasicProjectile.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

ASDasgProjectile::ASDasgProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->SetSphereRadius(20.0f);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TeleportDelay = 0.2f;
	DetonateDelay = 0.3f;

	movementComp->InitialSpeed = 5000.f;

	EffectComp->Deactivate();

	TeleportEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("TeleportEffectComp");
	TeleportEffectComp->SetupAttachment(RootComponent);
	TeleportEffectComp->Deactivate();
}

void ASDasgProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(),GetActorRotation());

	movementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	TeleportEffectComp->Activate(true);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDasgProjectile::TeleportInstigator, DetonateDelay);
}

void ASDasgProjectile::TeleportInstigator()
{

	AActor* InstigatorToTeleport = GetInstigator();
	if (ensure(InstigatorToTeleport))
	{
		InstigatorToTeleport->TeleportTo(GetActorLocation(), InstigatorToTeleport->GetActorRotation());

		// Play shake on the player we teleported
		APawn* InstigatorPawn = Cast<APawn>(InstigatorToTeleport);
		APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
		if (PC && PC->IsLocalController())
		{
			//PC->ClientStartCameraShake();
		}
	}
	Destroy();
}

void ASDasgProjectile::BeginPlay()
{
	Super::BeginPlay();

	EffectComp->Activate(true);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDasgProjectile::Explode_Implementation, TeleportDelay);
}

void ASDasgProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASDasgProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
