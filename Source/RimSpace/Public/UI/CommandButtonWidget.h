// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommandButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class RIMSPACE_API UCommandButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(const FText& CommandName, AActor* TargetActor);

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* CommandButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Label;

private:
	FText Cmd;

	UPROPERTY()
	AActor* Actor;

	UFUNCTION()
	void OnButtonClicked();
	
};
