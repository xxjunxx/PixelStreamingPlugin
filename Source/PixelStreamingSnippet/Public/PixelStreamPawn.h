// Copyright 2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "PixelStreamingInputComponent.h"
#include "UnrealString.h"
#include "Components/StaticMeshComponent.h"
//#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PixelStreamPawn.generated.h"

UCLASS()
class PIXELSTREAMINGSNIPPET_API APixelStreamPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APixelStreamPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PressMenu();

	UFUNCTION()
	void UIInteractionProcess(const FString& Descriptor);

public:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wMainMenu;

	// Variable to hold the widget After Creating it.
	UUserWidget* MyMainMenu;

private:
	APlayerController * PlayerController;
	UPixelStreamingInputComponent * PixelStreamInputComponent;

};
