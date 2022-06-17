// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasicProjectile.h"

// Sets default values
ASBasicProjectile::ASBasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASBasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

