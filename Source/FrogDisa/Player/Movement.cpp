// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"
#include "FrogDisa/InteractiveObject.h"
#include "FrogDisa/Collectibles.h"
#include "FrogDisa/GrapplingObjectComponent.h"
#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/MyHUD.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Styling/SlateWidgetStyleContainerInterface.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"


//#include <Runtime/Engine/Private/Pawn.cpp>
// Sets default values
AMovement::AMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	//GetCapsuleComponent()->SetCapsuleRadius(20.f);
	//MovementComponent-
	cameraComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("camera"));
	cameraComponent->bUsePawnControlRotation = true;
	cameraComponent->TargetArmLength = 400.f;
	cameraComponent->SetupAttachment(RootComponent);
	cameraOffsetYPlus = FVector(0.f, 80.f, 0.f);
	cameraOffsetYMinus = FVector(0.f, -110.f, 0.f);
	//cameraComponent-> AddRelativeLocation(cameraOffsetYPlus);

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("_Camera"));
	_Camera->SetupAttachment(cameraComponent);
	_Camera->bUsePawnControlRotation = false;
	_Camera->SetRelativeLocation(FVector(0.f, 110.f, 80.f));

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	Mesh->SetSkeletalMesh(mesh.Object);
	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	Mesh->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);

	//Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("Shoot"));

	isBearObject = false;
	isAiming = false;
	isRunning = false;
	craftMenuOpen = false;
	Collectibles = 0;
}

// Called when the game starts or when spawned
void AMovement::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MyTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void AMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed,this, &AMovement::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMovement::StopShoot);
	PlayerInputComponent->BindAction("ReturnWrench", IE_Pressed, this, &AMovement::ReturnWrench);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMovement::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMovement::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMovement::AddControllerPitchInput);

	//PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AMovement::Heal);
	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, this, &AMovement::InteractionWithObject);
	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, this, &AMovement::UseGrapplingHook);
	PlayerInputComponent->BindAction("TakeCollectibles", IE_Pressed, this, &AMovement::TakeCollectibles);
	//PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMovement::Aim);
	PlayerInputComponent->BindAxis("Aim", this, &AMovement::Aim);
	PlayerInputComponent->BindAxis("Run", this, &AMovement::Run);
}

void AMovement::MoveForward(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{

			AddMovementInput(GetActorForwardVector(), Value);

	}
}

void AMovement::MoveRight(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMovement::Run(float Value)
{
	if (Value != 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		isRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}
void AMovement::Attack()
{
	if (isBearObject == false)
	{
		if (isAiming == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot"));
			ShootComponent->ThrowProjectile();
		}
		else
		{
			//MeleeAttack
		}
	}
	else
	{
		isBearObject = false;
		InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InteractiveObject->SetSimulatePhysics(true);
		InteractiveObject->AddImpulse(_Camera->GetForwardVector() * 1500.f * InteractiveObject->GetMass());
		InteractiveObject = NULL;
	}
}

void AMovement::StopShoot()
{
	//UE_LOG(LogTemp, Warning, TEXT("Stop shoot"));
}

//void AMovement::Heal()
//{
//
//	Health->UpdateHealth(40.f);
//
//
//}

void AMovement::UseGrapplingHook()
{
	FHitResult hitPoint;
	startLoc = GetActorLocation();
	
	if (isBearObject == false)
	{
		FVector Start = _Camera->GetComponentLocation();
		End = _Camera->GetForwardVector() * 3000.f + Start;
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, CollisionParams) == true)
		{
			if (hitPoint.Actor->IsA(AGrapplingObject::StaticClass()) && GetDistanceTo(hitPoint.Actor.Get()) > MinimumGrapplingDistance)
			{
				grapplingPoint = hitPoint.Actor.Get();
				endLoc = hitPoint.Location;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, hitPoint.Actor->GetName());
				GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::LerpTo, 0.01f, true, 0.f);
				
			}
		}

	}
}

void AMovement::LerpTo()
{
	if (GetDistanceTo(grapplingPoint) <= 70.f)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(GetDistanceTo(grapplingPoint)));
		GetWorldTimerManager().ClearTimer(GrapplingTimer);
		GetMovementComponent()->Velocity = FVector::ZeroVector;
		GetMovementComponent()->Velocity.Z = 500.f;
		grapplingPoint = NULL;
	}
	else
	{
		GetMovementComponent()->Velocity = (grapplingPoint->GetActorLocation() - GetActorLocation()) * 5;
	}
}
void AMovement::LerpToGrapplingPoint(FVector StartLocation, FVector EndLocation, float time)
{
	FVector nextLoc = UKismetMathLibrary::VLerp(StartLocation,
		EndLocation, time);
	nextLoc.Z += 0.1f;
	SetActorLocation(nextLoc);
}
void AMovement::InteractionWithObject()
{

	FHitResult hitPoint;
	
	if (isBearObject == true)
	{
		isBearObject = false;
		
		InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InteractiveObject->SetSimulatePhysics(true);
		InteractiveObject = NULL;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drop this"));
	}
	else
	{
		End = GetActorForwardVector() * 500.f + GetActorLocation();
		DrawDebugLine(GetWorld(), GetActorLocation(), End, FColor::Blue, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), End, ECC_Visibility, CollisionParams) == true)
		{
			if (hitPoint.Actor->IsA(AInteractiveObject::StaticClass()))
			{
				isBearObject = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Take this"));

				InteractiveActor = hitPoint.Actor.Get();
				InteractiveActor->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 90.f + GetActorUpVector() * 10.f);
				InteractiveActor->SetActorRotation(GetActorRotation());
				InteractiveObject = InteractiveActor->FindComponentByClass<UStaticMeshComponent>();
				InteractiveObject->SetSimulatePhysics(false);//отключаем потому что с физикой объект не двигается

				InteractiveActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			}
		}
		
	}
}

void AMovement::AddControllerYawInput(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController() && craftMenuOpen == false)
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Val);
	}
}

void AMovement::AddControllerPitchInput(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController() && craftMenuOpen == false)
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Val);
	}
}

void AMovement::Jump()
{
	if (CharacterMovement->IsFalling() == false/* && Health->Stamina - 10.f >= 0*/)
	{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	
		//Health->UpdateStamina(-10.f);
	}
}

void AMovement::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}

void AMovement::Aim(float Value)
{
	if (Value != 0 && craftMenuOpen == false)
	{
		isAiming = true;
		cameraComponent->TargetArmLength = 100.f;
	}
	else
	{
		isAiming = false;
		cameraComponent->TargetArmLength = 400.f;
	}
}

bool AMovement::GetAimingState()
{
	return isAiming;
}

void AMovement::TakeCollectibles()
{
	FHitResult hitPoint;

	if (isBearObject == false)
	{
		FVector Start = _Camera -> GetComponentLocation();
		End = _Camera -> GetForwardVector() * 1000.f + Start;
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, CollisionParams) == true)
		{
			if (hitPoint.Actor->IsA(ACollectiblesObject::StaticClass()))
			{
				Collectibles++;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Collectibles));
				hitPoint.Actor->Destroy();
			}
		}
		
	}
}

void AMovement::ReturnWrench()
{
	ShootComponent->ReturnProjectile();
}