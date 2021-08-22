// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile(TEXT("Class'/Script/FrogDisa.ThrowProjectile'"));
	ProjectileWrench = projectile.Class;

	ConstructorHelpers::FClassFinder<AThrowProjectile> projectiles(TEXT("/Game/Blueprint/MyThrowProjectile"));
	BlueprintWrench = projectiles.Class;
	
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//if (BlueprintWrench)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("pzdc"));
	//}
	Wrench = GetWorld()->SpawnActor<AThrowProjectile>(ProjectileWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	//if(GetOwner() != NULL)
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, GetOwner()->GetName());
	//Wrench->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	Wrench->AttachToPlayerCharacter(GetOwner());
	//Wrench->SetActorRelativeLocation(GetOwner()->GetActorForwardVector() * 80.f);//SetActorLocation(GetActorLocation() + GetActorForwardVector() * 90.f + GetActorUpVector() * 10.f);
	// ...
	
}

void UShootComponent::ThrowProjectile()
{
	Wrench->Launch();
	
}

void UShootComponent::ReturnProjectile()
{
	Wrench->ReturnToCharacter();
}




