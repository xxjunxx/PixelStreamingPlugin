// Copyright 2019, Institute for Artificial Intelligence - University of Bremen

#include "PixelStreamPawn.h"

// Sets default values
APixelStreamPawn::APixelStreamPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent()); 
}

// Called when the game starts or when spawned
void APixelStreamPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController((UObject*)GetWorld(), 0);

	PixelStreamInputComponent = Cast<UPixelStreamingInputComponent>(PlayerController->GetComponentByClass(UPixelStreamingInputComponent::StaticClass()));

	if (PixelStreamInputComponent)
	{
		PixelStreamInputComponent->OnPixelStreamingInputEvent.AddDynamic(this, &APixelStreamPawn::UIInteractionProcess);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Fail to get pixel streaming component!"));

	}


	if (wMainMenu) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		MyMainMenu = CreateWidget<UUserWidget>(this->PlayerController, wMainMenu);
	}

}

// Called every frame
void APixelStreamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FVector(CurrentLocation.X + 0.2, CurrentLocation.Y, CurrentLocation.Z);
	SetActorLocation(NewLocation);

	FString CurrentGameTime = FString::SanitizeFloat(GetGameTimeSinceCreation());

	if (PixelStreamInputComponent)
	{
		PixelStreamInputComponent->SendPixelStreamingResponse("{\"msg\":\"currentTime\", \"time\":\"" + CurrentGameTime + " \"}");

	}

}

// Called to bind functionality to input
void APixelStreamPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &APixelStreamPawn::PressMenu);
}

void APixelStreamPawn::UIInteractionProcess(const FString& Descriptor)
{
	FString MsgToPrint;
	bool Success;

	PixelStreamInputComponent->GetJsonStringValue(Descriptor, "msgToPrint", MsgToPrint, Success);

	if (Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, MsgToPrint);

		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y + 10, CurrentLocation.Z);
		SetActorLocation(NewLocation);
	}
}

void APixelStreamPawn::PressMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Press the Menu"));

	//PixelStreamInputComponent->SendPixelStreamingResponse("{\"msg\":\"clickButton\", \"button\":\"item1\"}");

	if (MyMainMenu)
	{
		//let add it to the view port
		MyMainMenu->AddToViewport();
	}

	PlayerController->bShowMouseCursor = true;
}