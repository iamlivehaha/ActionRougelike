// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasicProjectile.h"
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ASBasicProjectile::ASBasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	/*way1(recommended) : set the profile in project setting and directly use it*/
	SphereComp->SetCollisionProfileName("Projectile");
	/*way2 set the collision channel respectively*/
	//sphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//sphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//sphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//sphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
	particleComp->SetupAttachment(RootComponent);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	movementComp->InitialSpeed = 1000.0f;
	/** If true, this projectile will have its rotation updated each frame to match the direction of its velocity. */
	movementComp->bRotationFollowsVelocity = true;
	//If true, the initial Velocity is interpreted as being in local space upon startup.
	//从本地而非世界坐标意味着初速度方向由生产位置的Transform决定，而非固定不变
	movementComp->bInitialVelocityInLocalSpace = true;
	movementComp->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ASBasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	

}

void ASBasicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


void ASBasicProjectile::Explode_Implementation()
{

}

void ASBasicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentHit.AddDynamic(this, &ASBasicProjectile::OnActorHit);
}

// Called every frame
void ASBasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


