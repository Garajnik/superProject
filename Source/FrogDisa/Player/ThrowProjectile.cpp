// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Movement.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AThrowProjectile::AThrowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Axe.SM_Axe'"));
	ProjectileMesh -> SetStaticMesh(mesh.Object);
	//FVector scale = FVector(0.1f, 0.1f, 0.1f);
	//ProjectileMesh->SetWorldScale3D(scale);
	ProjectileMovementComponent = CreateDefaultSubobject<UMovementComponent>(TEXT("MovementComponent"));
	//RootComponent = ProjectileCollider;
	//ProjectileMesh->SetupAttachment(RootComponent);
	projectileIsReturning = false;
}

// Called when the game starts or when spawned
void AThrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//ProjectileMesh->SetSimulatePhysics(true);
	//ProjectileMovementComponent->Velocity = GetActorForwardVector() * 10;
	//ProjectileMesh->AddImpulse(GetActorForwardVector() * 2000.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ProjectileMesh->GetFName().ToString());
	
	isLaunched = false;
	projectileIsReturning = false;
}

void AThrowProjectile::AttachToPlayerCharacter(AActor* Character)
{
	
	OwnerPlayer = Character;
	//
	ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_rSocket"));
	SetActorRelativeLocation(FVector::ZeroVector);
	//if(Owner)
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Owner->GetName());
}

void AThrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if(GetDistanceTo(Cast<AMovement>(GetWorld()->)))
	//MyTimeline.TickTimeline(DeltaTime);
}

void AThrowProjectile::ReturnToCharacter()
{
	if (projectileIsReturning == false && isLaunched == true)
	{
		//AttachToActor(OwnerPlayer,FAttachmentTransformRules::KeepWorldTransform);
		ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
		GetWorldTimerManager().ClearTimer(MoveTimer);
		AddActorWorldRotation(FRotator(0.f, 150.f, 0.f));
		StartLocation = GetActorLocation();
		EndLocation = OldStartLocation;
		projectileIsReturning = true;
		halfDistance = GetDistanceTo(OwnerPlayer) / 2;
		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
	}
}

void AThrowProjectile::ReturnStart()
{

}

void AThrowProjectile::Launch()
{
	if (isLaunched == false)
	{
		//Detach(FDetachmentTransformRules::KeepWorldTransform);
		DetachRootComponentFromParent(true);
		OldStartLocation = GetActorLocation();
		StartLocation = GetActorLocation();
		SetActorRotation(OwnerPlayer->GetActorRotation());
		EndLocation = GetActorLocation() + GetActorForwardVector() * 1000.f;
		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
		isLaunched = true;
	}
}


void AThrowProjectile::Move()
{
	if (projectileIsReturning)
	{
		currentDistance = FVector::Distance(Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), GetActorLocation());
		
		float currentAlpha;
		currentAlpha = FMath::Abs(halfDistance - currentDistance) / (halfDistance / 2);
		currentAlpha = FMath::Clamp(currentAlpha, 0.f,1.f);
		//currentAlpha /= 2;
		FVector nextRightVector = GetActorRightVector() * UKismetMathLibrary::Lerp(halfDistance, 0, currentAlpha) * -1 + Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));

		FVector nextLoc = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), nextRightVector, 0.01f, 2000.f);

		SetActorLocation(nextLoc);

		if (FVector::Distance(Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_rSocket")), GetActorLocation()) < 7)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Returned"));
			GetWorldTimerManager().ClearTimer(MoveTimer);
			projectileIsReturning = false;
			isLaunched = false;
		}
	}
	else
	{
		if (isLaunched)
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * 10);
		}
	}

}

void AThrowProjectile::Stop()
{
	isLaunched = false;
}
