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
	FString InventoryInfo = Inventory->GetInventoryInfo();
	Info += TEXT("=== 库存 ===\n");
	Info += InventoryInfo;
	return Info;
}

AStove::AStove()
{
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InputInventory"));
	ActorType = EInteractionType::EAT_Stove;
}

void AStove::TestAddAndRemoveItem(const FText& Command)
{
	if (Command.EqualTo(FText::FromString(TEXT("测试：添加原料（玉米）"))))
	{
		FItemStack ItemStack;
		ItemStack.ItemID = 1002;
		ItemStack.Count = 1;
		Inventory->AddItem(ItemStack);
	}
	else if (Command.EqualTo(FText::FromString(TEXT("测试：移除成品（玉米饼）"))))
	{
		FItemStack ItemStack;
		ItemStack.ItemID = 2003;
		ItemStack.Count = 1;
		Inventory->RemoveItem(ItemStack);
	}
}
