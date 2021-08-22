// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


AMyHUD::AMyHUD()
{
	//static ConstructorHelpers::FClassFinder<AMovement> PlayerObj(TEXT("Class'/Script/FrogDisa.Movement'"));
	//PlayerClass = PlayerObj.Class;
	//InputComponent->
}


void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}



void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	/*if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}*/
	
}
