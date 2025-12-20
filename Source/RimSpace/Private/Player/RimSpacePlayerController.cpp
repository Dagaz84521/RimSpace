// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RimSpacePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/RimSpaceActorBase.h"
#include "UI/StatusInfoWidget.h"
#include "Interface/InteractionActorInterface.h"
#include "UI/CommandMenuWidget.h"

void ARimSpacePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(RimSpaceContext,0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	if (HoverInfoWidgetClass)
	{
		HoverInfoWidget = CreateWidget<UUserWidget>(this, HoverInfoWidgetClass);
		if (HoverInfoWidget)
		{
			HoverInfoWidget->AddToViewport();
			HoverInfoWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ARimSpacePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	UpdateHoverInfo();
	UpdateCommandMenu();
}

void ARimSpacePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARimSpacePlayerController::Move);
	EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &ARimSpacePlayerController::UpDown);
	EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &ARimSpacePlayerController::RightClick);
	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &ARimSpacePlayerController::LeftClick);
}

void ARimSpacePlayerController::Move(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector2D InputAxisVector = Value.Get<FVector2D>();
		const float DeltaTime = GetWorld()->GetDeltaSeconds();
        
		// 获取镜头控制旋转（仅Yaw生效，保持俯视）
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
        
		// 计算世界空间的前后/左右方向
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
		// 计算XY轴位移（结合输入、速度、DeltaTime）
		FVector MovementDelta = (ForwardDirection * InputAxisVector.Y + RightDirection * InputAxisVector.X) 
							  * MoveSpeed * DeltaTime;
        
		// 保持Z轴高度不变（俯视镜头只动XY）
		MovementDelta.Z = 0;

		FVector NewLocation = ControlledPawn->GetActorLocation() + MovementDelta;
		NewLocation.X = FMath::Clamp(NewLocation.X, MinMapBounds.X, MaxMapBounds.X);
		NewLocation.Y = FMath::Clamp(NewLocation.Y, MinMapBounds.Y, MaxMapBounds.Y);
		// 直接更新镜头Pawn位置
		ControlledPawn->SetActorLocation(ControlledPawn->GetActorLocation() + MovementDelta);
	}
}

void ARimSpacePlayerController::UpDown(const FInputActionValue& Value)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const float InputAxisValue = Value.Get<float>();
		const float DeltaTime = GetWorld()->GetDeltaSeconds();
        
		// 计算Z轴位移增量（UpDownSpeed直接控制速度，单位：cm/秒）
		const float ZDelta = InputAxisValue * UpDownSpeed * DeltaTime;
        
		// 获取镜头Pawn当前位置
		FVector NewLocation = ControlledPawn->GetActorLocation();
        
		// 限制镜头高度范围（根据你的需求调整最小值和最大值）
		NewLocation.Z = FMath::Clamp(NewLocation.Z - ZDelta, 500.0f, 5000.0f);
        
		// 直接设置镜头Pawn的位置
		ControlledPawn->SetActorLocation(NewLocation);
	}
}

void ARimSpacePlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, TEXT("Cursor Trace Called"));
	Cast<ARimSpaceActorBase>(CursorHit.GetActor());
	// 由于当鼠标点击不同的Actor的时候，需要将上一个Actor的高光取消，同时将现在的高光开启
	LastActor = ThisActor;
	ThisActor = Cast<ARimSpaceActorBase>(CursorHit.GetActor());
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void ARimSpacePlayerController::RightClick(const FInputActionValue& Value)
{
	if (ThisActor)
	{
		RightClickedActor = ThisActor;
		SpawnCommandMenu();
	}
	else
	{
		RightClickedActor = nullptr;
	}
}

void ARimSpacePlayerController::LeftClick(const FInputActionValue& Value)
{
	// 取消右键选中状态
	if (ThisActor != RightClickedActor)
	{
		RightClickedActor = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Left Clicked: Clear RightClickedActor"));
	}
}

void ARimSpacePlayerController::UpdateHoverInfo()
{
	if (ThisActor && HoverInfoWidget)
	{
		IInteractionActorInterface* InteractionActor = Cast<IInteractionActorInterface>(ThisActor);
		UStatusInfoWidget* StatusInfoWidget = Cast<UStatusInfoWidget>(HoverInfoWidget);
		if (InteractionActor && StatusInfoWidget)
		{
			FString ActorName = InteractionActor->GetActorName();
			FString ActorInfo = InteractionActor->GetActorInfo();
			StatusInfoWidget->UpdateInfo(ActorName, ActorInfo);
			FVector2D MousePosition;
			GetMousePosition(MousePosition.X, MousePosition.Y);
			// 设置位置
			HoverInfoWidget->SetPositionInViewport(MousePosition, true);
			HoverInfoWidget->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}
	if (HoverInfoWidget)
	{
		HoverInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ARimSpacePlayerController::SpawnCommandMenu()
{
	if (CommandMenuWidget)
	{
		CommandMenuWidget->RemoveFromParent();
		CommandMenuWidget = nullptr;
	}
	if (CommandMenuWidgetClass && RightClickedActor)
	{
		CommandMenuWidget = CreateWidget<UCommandMenuWidget>(this, CommandMenuWidgetClass);
		if (CommandMenuWidget)
		{
			CommandMenuWidget->InitializeMenu(RightClickedActor);
			CommandMenuWidget->AddToViewport();
			FVector2D MousePosition;
			GetMousePosition(MousePosition.X, MousePosition.Y);
			CommandMenuWidget->SetPositionInViewport(MousePosition, true);
		}
	}
}

void ARimSpacePlayerController::UpdateCommandMenu()
{
	if (CommandMenuWidget && !RightClickedActor)
	{
		CommandMenuWidget->RemoveFromParent();
		CommandMenuWidget = nullptr;
	}
}