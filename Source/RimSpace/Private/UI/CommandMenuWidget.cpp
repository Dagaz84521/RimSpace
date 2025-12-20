// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommandMenuWidget.h"
#include "UI/CommandButtonWidget.h"
#include "Components/VerticalBox.h"
#include "Interface/CommandProvider.h"
#include "Interface/InteractionActorInterface.h"

void UCommandMenuWidget::InitializeMenu(AActor* TargetActor)
{
	CurrentActor = TargetActor;
	if (!CurrentActor || !CommandButtonClass || !CommandListBox)
	{
		return;
	}
	ICommandProvider* CommandActor = Cast<ICommandProvider>(CurrentActor);
	if (!CommandActor)
		return;
	TArray<FText> Commands = CommandActor->GetCommandList();
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
