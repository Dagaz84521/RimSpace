// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Stove.h"
#include "Component/InventoryComponent.h"

TArray<FText> AStove::GetCommandList() const
{
	TArray<FText> CommandList;
	if (TaskRemainCount > 0)
	{
		CommandList.Add(FText::FromString(TEXT("取消任务")));
		CommandList.Add(FText::FromString(TEXT("修改任务")));
	}
	else
	{
		CommandList.Add(FText::FromString(TEXT("添加任务")));
	}
	return CommandList;
}

void AStove::ExecuteCommand(const FText& Command)
{
	TestAddAndRemoveItem(Command);
}

FString AStove::GetActorInfo() const
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

AStove::AStove()
{
	InputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InputInventory"));
	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("OutputInventory"));
}

void AStove::TestAddAndRemoveItem(const FText& Command)
{
	if (Command.EqualTo(FText::FromString(TEXT("测试：添加原料（玉米）"))))
	{
		FItemStack ItemStack;
		ItemStack.ItemID = 1002;
		ItemStack.Count = 1;
		InputInventory->AddItem(ItemStack);
	}
	else if (Command.EqualTo(FText::FromString(TEXT("测试：移除成品（玉米饼）"))))
	{
		FItemStack ItemStack;
		ItemStack.ItemID = 2003;
		ItemStack.Count = 1;
		OutputInventory->RemoveItem(ItemStack);
	}
}
