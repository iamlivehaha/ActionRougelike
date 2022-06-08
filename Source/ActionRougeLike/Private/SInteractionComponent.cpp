// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include <SGamePlayInterface.h>
#include <DrawDebugHelpers.h>

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FVector Start;
	FVector End;

	FVector EyeLocation;
	FRotator EyeRotation;

	//GetOwner():Get the owner of this Actor, used primarily for network replication.
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	Start = EyeLocation;
	End = EyeLocation + EyeRotation.Vector()*600;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	//line trace way to detect collision
	//FHitResult Hit;
	//bool bBlocking = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

	//sphere sweep way to detect collision
	TArray<FHitResult> Hits;
	FCollisionShape shape;
	float Radius = 30.f;
	shape.SetSphere(Radius);
	bool bBlocking = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, shape);

	FColor lineColor = bBlocking ? FColor::Cyan : FColor::Red;

	for (auto Hit:Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, lineColor, false, 2);

			ISGamePlayInterface* GameplayInterface = Cast<ISGamePlayInterface>(HitActor);
			if (GameplayInterface)
			{
				UE_LOG(LogTemp, Warning, TEXT("execute inteact action!"));

				APawn* MyPawn = Cast<APawn>(MyOwner);//could be null
				//quirk way to execute function
				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);

				break;
			}
			
		}
		//Debug
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitActor: %s, at game time: %f"), *GetNameSafe(HitActor), GetWorld()->TimeSeconds);

			FString CombinedString = FString::Printf(TEXT("Hit the object: %s"), *HitActor->GetName());
			DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
		}
	}

	//Debug
	//FColor lineColor = bBlocking ? FColor::Cyan : FColor::Red;
	DrawDebugLine(MyOwner->GetWorld(), Start, End, lineColor, false, 3, 0, 2);



}

