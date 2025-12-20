// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WorkStation.h"
#include "Component/InventoryComponent.h"

TArray<FText> AWorkStation::GetCommandList() const
{
	return Super::GetCommandList();
}

FString AWorkStation::GetActorInfo() const
{
	return Super::GetActorInfo();
}

AWorkStation::AWorkStation()
{
	InputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InputInventory"));
	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("OutputInventory"));
}
