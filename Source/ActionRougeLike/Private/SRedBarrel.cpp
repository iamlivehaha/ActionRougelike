// Fill out your copyright notice in the Description page of Project Settings.


#include "SRedBarrel.h"
#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/RadialForceComponent.h>
#include "DrawDebugHelpers.h"
#include <SMagicProjectile.h>

// Sets default values
ASRedBarrel::ASRedBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = staticMesh;
	staticMesh->SetSimulatePhysics(true);
	staticMesh->SetCollisionProfileName("PhysicActor");

	radialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	radialForceComp->SetupAttachment(staticMesh);
	radialForceComp->ImpulseStrength = 2000.0f;
	radialForceComp->Radius = 600.0f;
	/** If true, the impulse will ignore mass of objects and will always result in a fixed velocity change */
	radialForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	radialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASRedBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASRedBarrel::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();

	//动态代理(委托)绑定,在创建staticMesh对象后，绑定函数OnActorHit函数到其代理OnComponentHit事件上，OnActorHit函数执行具体的内容
	staticMesh->OnComponentHit.AddDynamic(this, &ASRedBarrel::OnActorHit);
}

void ASRedBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASMagicProjectile* Projectile = Cast<ASMagicProjectile>(OtherActor);
	if (Projectile)
	{
		radialForceComp->FireImpulse();

		UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

		// %s = string
		// %f = float
		// logs: "OtherActor: MyActor_1, at gametime: 124.4" 
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

		// Detailed info on logging in ue4
		// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
	}
}

// Called every frame
void ASRedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

