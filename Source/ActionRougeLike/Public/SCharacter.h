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
class USAttributeComponent;

enum class EProjectileSpawnLocation
{
	LHand,
	RHand,
	MagicRod
};

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

		//boilerplate样板文件的宏定义
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> projectileClass;// * TSubclassOf is a Template to allow TClassType's to be passed around with type safety 

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> BlackholeClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* PrimaryAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* BlackholeAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* DashAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackAnimDelay;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* CastingEffect;

	/* Sound Effect to play (Can be Wave or Cue) */
	UPROPERTY(EditAnywhere, Category = "Attack")
		USoundBase* CastingSound;

	FTimerHandle TimerHandle;

	void PrimaryAttack_TimeEclipse();

	void BlackholeAttack_TimeEclipse();

	void Dash_TimeEclipese();
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USInteractionComponent* InteractComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMoveForward(float value);

	void OnMoveRight(float value);

	FTransform AimAtTarget(EProjectileSpawnLocation EAttackLoc);

	void PrimaryAttack();

	void BlackholeAttack();

	void DashAttack();

	void SpawnProjectile( TSubclassOf<AActor> ClassToSpawn, EProjectileSpawnLocation SpawnLoc);

	void Jump();

	void PrimaryInteract();

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION()
		void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* Attricomp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


};
