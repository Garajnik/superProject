// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "InteractiveObjectComponent.h"
// Sets default values
AInteractiveObject::AInteractiveObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Interactive = CreateDefaultSubobject<UInteractiveObjectComponent>(TEXT("Interactive"));
	Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
}

void AInteractiveObject::DropThisObject()
{
	Mesh->SetSimulatePhysics(true);
}

void AInteractiveObject::TakeThisObject()
{
	Mesh->SetSimulatePhysics(false);
}

