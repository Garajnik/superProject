#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "ShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShootComponent();

	void ThrowProjectile();

	void ReturnProjectile();

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf<AThrowProjectile> ProjectileWrench;

		TSubclassOf<AThrowProjectile> BlueprintWrench;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	AThrowProjectile* Wrench;

};
