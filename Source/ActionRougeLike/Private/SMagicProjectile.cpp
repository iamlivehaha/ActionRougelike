// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "DrawDebugHelpers.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = sphereComp;
	/*way1(recommended) : set the profile in project setting and directly use it*/
	sphereComp->SetCollisionProfileName("Projectile");
	/*way2 set the collision channel respectively*/
	//sphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//sphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//sphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//sphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);


	particleComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
	particleComp->SetupAttachment(RootComponent);

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
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//#ifndef _DEBUG
	////it will be fast to use blueprint to draw debug info
	//DrawDebugSphere(GetWorld(), SweepResult.Location,100,12,FColor::Red,false,4.0f);
	//#endif

	if (OtherActor && OtherActor != GetInstigator())
	{
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");

		//// Parry Ability (GameplayTag Example)
		//USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		//if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		//{
		//	MoveComp->Velocity = -MoveComp->Velocity;

		//	SetInstigator(Cast<APawn>(OtherActor));
		//	return;
		//}

		//// Apply Damage & Impulse
		//if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		//{
		//	// We only explode if the target can be damaged, it ignores anything it Overlaps that it cannot Damage (it requires an AttributeComponent on the target)
		//	Explode();

		//	if (ActionComp && BurningActionClass && HasAuthority())
		//	{
		//		ActionComp->AddAction(GetInstigator(), BurningActionClass);
		//	}
		//}
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

