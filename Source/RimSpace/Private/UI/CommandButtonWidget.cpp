// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommandButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interface/InteractionActorInterface.h"

void UCommandButtonWidget::Setup(const FText& CommandName, AActor* TargetActor)
{
	Cmd = CommandName;
	Actor = TargetActor;
	if (Label)
	{
		Label->SetText(CommandName);
	}
	CommandButton->OnClicked.AddDynamic(this, &UCommandButtonWidget::OnButtonClicked);
}

void UCommandButtonWidget::OnButtonClicked()
{
	if (Actor)
	{
		IInteractionActorInterface* InteractionActor = Cast<IInteractionActorInterface>(Actor);
		if (InteractionActor)
		{
			InteractionActor->ExecuteCommand(Cmd);
		}
	}
}