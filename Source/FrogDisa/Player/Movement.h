#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
//#include "HealthComponent.h"
#include "ShootComponent.h"
#include "FrogDisa/InteractiveObject.h"
#include "GameFramework/HUD.h"
#include "FrogDisa/CollectiblesObject.h"
#include "GameFramework/MovementComponent.h"
#include "Movement.generated.h"

UCLASS()
class FROGDISA_API AMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMovement();
	
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Attack();

	void StopShoot();

	//void Heal();

	void Jump();

	void StopJumping();

	void UseGrapplingHook();

	void Aim(float Value);
	
	void Run(float Value);

	//UFUNCTION(Blueprint)
	void InteractionWithObject();

	void TakeCollectibles();
	//void ResetJumpState();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddControllerYawInput(float Val);

	virtual void AddControllerPitchInput(float Val);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReturnWrench();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meshComponent")
		UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* cameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* _Camera;

	//Переменная для отображения здоровья
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		UHealthComponent* Health;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
		UShootComponent* ShootComponent;

	UFUNCTION(BlueprintCallable)
		bool GetAimingState();
private:
	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle GrapplingTimer;
	bool isBearObject;
	bool isAiming;
	bool isRunning;
	bool circleMenuIsOpen;
	bool craftMenuOpen;
	int Collectibles;
	FVector End;
	FVector cameraOffsetYPlus;
	FVector cameraOffsetYMinus;
	FVector startLoc, endLoc;
	const float MinimumGrapplingDistance = 200.f;
	const float DefaultSpeed = 400.f, RunningSpeed = DefaultSpeed + 200.f;
	void LerpToGrapplingPoint(FVector StartLocation, FVector EndLocation, float time);
	void LerpTo();
	FCollisionQueryParams CollisionParams;
	AActor* InteractiveActor;
	UStaticMeshComponent* InteractiveObject;
	AActor* grapplingPoint;
};
