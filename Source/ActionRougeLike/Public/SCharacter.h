// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

//Forward declaring is the minimalist way of letting the compiler know that word is a class. 
//Including inside the header will add compile times for other classes too (it can have a cascading effect) 
//so the convention is to forward declare when possible.
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()	

	//boilerplate样板文件的宏定义

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* cameraComp;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMoveForward(float value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
