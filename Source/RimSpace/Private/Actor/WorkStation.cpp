// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WorkStation.h"
#include "Component/InventoryComponent.h"

TArray<FText> AWorkStation::GetCommandList() const
{
	return TArray<FText>();
}

void AWorkStation::ExecuteCommand(const FText& Command)
{
}

FString AWorkStation::GetActorInfo() const
{
	FString Info;
	FString InputInfo = InputInventory->GetInventoryInfo();
	FString OutputInfo = OutputInventory->GetInventoryInfo();
	Info += TEXT("=== 输入库存 ===\n");
	Info += InputInfo;
	Info += TEXT("=== 输出库存 ===\n");
	Info += OutputInfo;
	return Info;
}

AWorkStation::AWorkStation()
{
	InputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InputInventory"));
	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("OutputInventory"));
}
