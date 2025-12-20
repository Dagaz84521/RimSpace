// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "Data/ItemData.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TotalSpace = 50;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(const FItemStack& Item)
{
	return false;
	/*if (!Item.IsValid())
	{
		return false;
	}

	const UItemData* ItemData = 
	if (!ItemData)
	{
		return false;
	}

	const int32 NeededSpace = Item.Count * ItemData->SpaceCost;
	if (UsedSpace + NeededSpace > TotalSpace)
	{
		return false; // 空间不足
	}

	// 尝试堆叠
	for (FItemStack& Stack : Items)
	{
		if (Stack.ItemID == Item.ItemID)
		{
			Stack.Count += Item.Count;
			UsedSpace += NeededSpace;
			return true;
		}
	}

	// 没有同类，新增一个 Stack
	Items.Add(Item);
	UsedSpace += NeededSpace;
	return true;*/
}

bool UInventoryComponent::RemoveItem(const FItemStack& Item)
{
	return false;
}

int32 UInventoryComponent::GetItemCount(int32 ItemID) const
{
	return 0;
}
