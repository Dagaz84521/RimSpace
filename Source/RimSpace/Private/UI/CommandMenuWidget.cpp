// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommandMenuWidget.h"
#include "UI/CommandButtonWidget.h"
#include "Components/VerticalBox.h"
#include "Interface/InteractionActorInterface.h"

void UCommandMenuWidget::InitializeMenu(AActor* TargetActor)
{
	CurrentActor = TargetActor;
	if (!CurrentActor || !CommandButtonClass || !CommandListBox)
	{
		return;
	}
	IInteractionActorInterface* InteractionActor = Cast<IInteractionActorInterface>(CurrentActor);
	if (!InteractionActor)
		return;
	TArray<FText> Commands = InteractionActor->GetCommandList();
	CommandListBox->ClearChildren();
	for (const FText& Command : Commands)
	{
		if (UCommandButtonWidget* CommandButton = CreateWidget<UCommandButtonWidget>(this, CommandButtonClass))
		{
			CommandButton->Setup(Command, CurrentActor);
			CommandListBox->AddChild(CommandButton);
		}
	}
}
