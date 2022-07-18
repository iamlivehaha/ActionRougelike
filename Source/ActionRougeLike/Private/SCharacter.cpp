// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "DrawDebugHelpers.h"
#include <SInteractionComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <SAttributeComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
//#include <Kismet/KismetMathLibrary.h>



// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	InteractComp = CreateDefaultSubobject<USInteractionComponent>("InteractComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

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

	AttackAnimDelay = 0.2f;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChange.AddDynamic(this, &ASCharacter::OnHealthChange);
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
	FVector rightVect = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(rightVect, value);
}

FTransform ASCharacter::AimAtTarget(EProjectileSpawnLocation EAttackLoc)
{
	FVector handLocation;
	FName HandSocketName;
	switch (EAttackLoc)
	{
	case EProjectileSpawnLocation::LHand:
		handLocation = GetMesh()->GetSocketLocation("Muzzle_l");
		HandSocketName = FName(TEXT("Muzzle_l"));
		break;
	case EProjectileSpawnLocation::RHand:
		handLocation = GetMesh()->GetSocketLocation("Muzzle_r");
		HandSocketName = FName(TEXT("Muzzle_r"));
		break;
	case EProjectileSpawnLocation::MagicRod:
		handLocation = GetMesh()->GetSocketLocation("Muzzle_mr");
		HandSocketName = FName(TEXT("Muzzle_mr"));
		break;
	default:
		handLocation = GetMesh()->GetSocketLocation("Muzzle_l");
		HandSocketName = FName(TEXT("Muzzle_l"));
		break;
	}

	//casting effects
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	UGameplayStatics::SpawnSoundAttached(CastingSound, GetMesh());

	#pragma region Old_AimAt
		FVector Start;
		FVector End;

		FVector EyeLocation;
		FRotator EyeRotation;

		//GetOwner():Get the owner of this Actor, used primarily for network replication.
		AActor* MyOwner = GetOwner();
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		Start = EyeLocation;
		End = EyeLocation + EyeRotation.Vector() * 3000;

		FHitResult hit;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		GetWorld()->LineTraceSingleByObjectType(hit, Start, End, ObjectQueryParams);


		AActor* HitActor = hit.GetActor();

		FVector ImpactLocation;

		if (HitActor)
		{
			ImpactLocation = hit.ImpactPoint;
		}
		else
		{
			ImpactLocation = End;
		}

		FRotator ProjRotation = UKismetMathLibrary::FindLookAtRotation(handLocation, ImpactLocation);
	#pragma endregion Old_AimAtWay

	#pragma region New_AimAtWay
		//FCollisionShape Shape;
		//Shape.SetSphere(20.f);
	
		////ignore player
		//FCollisionQueryParams Params;
		//Params.AddIgnoredActor(this);
	
	
		//FCollisionObjectQueryParams ObjColParams;
		//ObjColParams.AddObjectTypesToQuery(ECC_WorldStatic);
		//ObjColParams.AddObjectTypesToQuery(ECC_Pawn);
		//ObjColParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
		//FVector TraceStart = CameraComp->GetComponentLocation();
	
		////endpoint far into lookat direction (but not too far)
		//FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);
	
		//FHitResult hit;
	
		//if (GetWorld()->SweepSingleByObjectType(hit,TraceStart,TraceEnd,FQuat::Identity,ObjColParams,Shape,Params))
		//{
		//	//overwirte TraceEnd with hit point
		//	TraceEnd = hit.ImpactPoint;
		//}
	
		//FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - handLocation).Rotator();
	
	#pragma endregion New_AimAtWay

		////UE_LOG(LogTemp, Warning, TEXT("HitActor: %s, at game time: %f"), *GetNameSafe(HitActor), GetWorld()->TimeSeconds);
		////DrawDebugLine(MyOwner->GetWorld(), Start, End, FColor::Red, false, 3, 0, 2);

	return FTransform(ProjRotation, handLocation);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(PrimaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASCharacter::PrimaryAttack_TimeEclipse, AttackAnimDelay);
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(BlackholeAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASCharacter::BlackholeAttack_TimeEclipse, AttackAnimDelay);
}

void ASCharacter::DashAttack()
{
	PlayAnimMontage(DashAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASCharacter::Dash_TimeEclipese, AttackAnimDelay);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn, EProjectileSpawnLocation SpawnLoc)
{
	if (ensure(ClassToSpawn))
	{

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParams.Instigator = this;


		FTransform 	spawnTM;
		spawnTM = AimAtTarget(SpawnLoc);

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, spawnTM, spawnParams);
	}
}

void ASCharacter::PrimaryAttack_TimeEclipse()
{
	SpawnProjectile(projectileClass, EProjectileSpawnLocation::RHand);
}

void ASCharacter::BlackholeAttack_TimeEclipse()
{
	SpawnProjectile(BlackholeClass, EProjectileSpawnLocation::RHand);
}

void ASCharacter::Dash_TimeEclipese()
{
	SpawnProjectile(DashProjectileClass, EProjectileSpawnLocation::LHand);
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
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::DashAttack);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* Attricomp, float NewHealth, float Delta)
{
	if (NewHealth<=0.f&&Delta<0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}

}



