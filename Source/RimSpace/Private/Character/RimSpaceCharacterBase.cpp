// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RimSpaceCharacterBase.h"
#include "Actor/RimSpaceActorBase.h"
#include "Controller/RimSpaceAIController.h"
#include "Subsystem/RimSpaceTimeSubsystem.h"
#include "Component/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"
#include "RimSpace/RimSpace.h"

// Sets default values
ARimSpaceCharacterBase::ARimSpaceCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ARimSpaceAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

// Called when the game starts or when spawned
void ARimSpaceCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	auto TimeSubsystem = GetGameInstance()->GetSubsystem<URimSpaceTimeSubsystem>();
	TimeSubsystem->OnMinutePassed.AddDynamic(this, &ARimSpaceCharacterBase::UpdateEachMinute);
	TimeSubsystem->OnHourPassed.AddDynamic(this, &ARimSpaceCharacterBase::UpdateEachHour);
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		AICon->ReceiveMoveCompleted.AddDynamic(
			this,
			&ARimSpaceCharacterBase::OnMoveCompleted
		);
	}
}

void ARimSpaceCharacterBase::MoveToPoint(ARimSpaceActorBase* Target)
{
	if (!Target) return;

	AAIController* AICon = Cast<AAIController>(GetController());
	if (!AICon) return;

	USceneComponent* InteractionPoint = Target->GetInteractionPoint();
	if (!InteractionPoint) return;

	const FVector GoalLocation = InteractionPoint->GetComponentLocation();

	FAIMoveRequest Request;
	Request.SetGoalLocation(GoalLocation);
	Request.SetAcceptanceRadius(1.f);
	Request.SetUsePathfinding(true);

	FNavPathSharedPtr NavPath;
	const EPathFollowingRequestResult::Type Result = AICon->MoveTo(Request, &NavPath);

	if (Result == EPathFollowingRequestResult::RequestSuccessful)
	{
		CurrentPlace = nullptr;
		TargetPlace = Target;
	}
}

void ARimSpaceCharacterBase::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result != EPathFollowingResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Move Failed or Aborted"));
		return;
	}

	USceneComponent* IP = TargetPlace->GetInteractionPoint();
	if (!IP) return;

	// ① 精确站位（暂时采用瞬移）
	const FVector StandLocation = IP->GetComponentLocation();
	SetActorLocation(StandLocation);

	// ② 转向（朝向 Actor）
	const FVector LookDir =
		TargetPlace->GetActorLocation() - StandLocation;

	const FRotator TargetRot = LookDir.Rotation();
	SetActorRotation(TargetRot);

	CurrentPlace = TargetPlace;
	TargetPlace = nullptr;
	
}

// Called every frame
void ARimSpaceCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARimSpaceCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARimSpaceCharacterBase::UpdateEachMinute_Implementation(int32 NewMinute)
{
	ITimeAffectable::UpdateEachMinute_Implementation(NewMinute);

	int32 TimePassed = NewMinute - CurrentMinute;
	HungerDecreaseTimeTracker.TimeAccumulator += TimePassed;
	EnergyDecreaseTimeTracker.TimeAccumulator += TimePassed;
	if (HungerDecreaseTimeTracker.TimeAccumulator >= HungerDecreaseTimeTracker.MinutesInterval)
	{
		CharacterStats.Hunger = FMath::Clamp(CharacterStats.Hunger - HungerDecreaseAmount, 0.0f, CharacterStats.MaxHunger);
		HungerDecreaseTimeTracker.TimeAccumulator = 0;
	}
	if (EnergyDecreaseTimeTracker.TimeAccumulator >= EnergyDecreaseTimeTracker.MinutesInterval)
	{
		CharacterStats.Energy = FMath::Clamp(CharacterStats.Energy - EnergyDecreaseAmount, 0.0f, CharacterStats.MaxEnergy);
		EnergyDecreaseTimeTracker.TimeAccumulator = 0;
	}
	
}

void ARimSpaceCharacterBase::UpdateEachHour_Implementation(int32 NewHour)
{
	ITimeAffectable::UpdateEachHour_Implementation(NewHour);

}

void ARimSpaceCharacterBase::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
}

void ARimSpaceCharacterBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

FString ARimSpaceCharacterBase::GetActorName() const
{
	return TEXT("Character");
}

FString ARimSpaceCharacterBase::GetActorInfo() const
{
	return FString::Printf(
		TEXT("Hunger: %.1f / %.1f\nEnergy: %.1f / %.1f"),
		CharacterStats.Hunger,
		CharacterStats.MaxHunger,
		CharacterStats.Energy,
		CharacterStats.MaxEnergy
	);
}


