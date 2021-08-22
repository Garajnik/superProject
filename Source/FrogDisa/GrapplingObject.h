// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GrapplingObjectComponent.h"
#include "GrapplingObject.generated.h"

UCLASS()
class FROGDISA_API AGrapplingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UGrapplingObjectComponent* grapplingComponent;
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* GrapplingMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* GrapplingCollider;
};
