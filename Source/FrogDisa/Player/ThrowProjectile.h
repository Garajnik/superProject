// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/MovementComponent.h"
#include "ThrowProjectile.generated.h"

UCLASS()
class FROGDISA_API AThrowProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowProjectile();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ReturnToCharacter();

	void Launch();
	
	void Stop();
	void AttachToPlayerCharacter(AActor* Character);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* ProjectileCollider;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMovementComponent* ProjectileMovementComponent;
		UFUNCTION(BlueprintCallable)
			void ReturnStart();
	//AMovement* PlayerCharacter;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool projectileIsReturning;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isLaunched;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float halfDistance;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* OwnerPlayer;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
			FVector StartLocation;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector EndLocation;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
			FVector OldStartLocation;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float currentDistance;
private:
	FTimerHandle MoveTimer;

	void Move();

	
	
	
};
