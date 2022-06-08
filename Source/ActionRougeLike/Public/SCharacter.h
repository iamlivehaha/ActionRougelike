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
class USInteractionComponent;

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()	

	//boilerplate样板文件的宏定义
protected:
	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<AActor> projectileClass;// * TSubclassOf is a Template to allow TClassType's to be passed around with type safety 

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle;

	void PrimaryAttack_TimeEclipse();
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMoveForward(float value);

	void OnMoveRight(float value);

	void PrimaryAttack();

	void Jump();

	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	
};
