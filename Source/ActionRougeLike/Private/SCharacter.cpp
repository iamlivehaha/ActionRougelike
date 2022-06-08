// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "DrawDebugHelpers.h"
#include <SInteractionComponent.h>
//#include <Kismet/KismetMathLibrary.h>



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	InteractComp = CreateDefaultSubobject<USInteractionComponent>("InteractComp");

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);
	
	//使相机壁跟随控制旋转
	SpringArmComp->bUsePawnControlRotation = true;
	//下述两条语句的bool值互斥,一个是旋转跟随运动方向，一个是yaw旋转匹配控制器的Yaw旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::OnMoveForward(float value)
{
	//Add movement input along the given world direction vector 
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void ASCharacter::OnMoveRight(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;

	//shift+alt+F 查找API
	//way1: use api
	//FVector rightVect = UKismetMathLibrary::GetRightVector(controlRot);
	//way2: use api content
	FVector rightVect =  FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(rightVect, value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle,this,&ASCharacter::PrimaryAttack_TimeEclipse,0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ASCharacter::PrimaryAttack_TimeEclipse()
{
	FVector handLocation = GetMesh()->GetSocketLocation("Muzzle_04");

	FTransform spawnTM = FTransform(GetControlRotation(), handLocation);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(projectileClass, spawnTM, spawnParams);
}

void ASCharacter::Jump()
{
	ACharacter::Jump();
}

void ASCharacter::PrimaryInteract()
{
	if (InteractComp)
	{
		InteractComp->PrimaryInteract();
	}

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::OnMoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

