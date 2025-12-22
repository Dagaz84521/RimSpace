// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CultivateChamber.h"
#include "Component/InventoryComponent.h"


TArray<FText> ACultivateChamber::GetCommandList() const
{
	switch (CultivateType)
	{
	case ECultivateType::ECT_None:
		return { FText::FromString(TEXT("种植棉花")), FText::FromString(TEXT("种植玉米")) };
	case ECultivateType::ECT_Cotton:
	case ECultivateType::ECT_Corn:
		return { FText::FromString("取消种植") };
	}
	return {};
}




FString ACultivateChamber::GetActorInfo() const
{
	FString Info;
	Info += TEXT("=== 种植舱信息 ===\n");
	Info += FString::Printf(TEXT("培养类型: "));
	switch (CultivateType)
	{
	case ECultivateType::ECT_None:
		Info += FString::Printf(TEXT("无\n"));
		break;
	case ECultivateType::ECT_Cotton:
		Info += FString::Printf(TEXT("棉花\n"));
		break;
	case ECultivateType::ECT_Corn:
		Info += FString::Printf(TEXT("玉米\n"));
		break;
	}
	Info += FString::Printf(TEXT("培养进度: %d / %d\n"), CultivateProgress, CultivateMaxProgress);
	Info += TEXT("=== 临时存储区 ===\n");
	Info += OutputInventory->GetInventoryInfo();
	return Info;
}

void ACultivateChamber::ExecuteCommand(const FText& Command)
{
	if (Command.EqualTo(FText::FromString(TEXT("种植棉花"))))
	{
		CultivateType = ECultivateType::ECT_Cotton;
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, TEXT("开始种植棉花"));
		
	}
	else if (Command.EqualTo(FText::FromString(TEXT("种植玉米"))))
	{
		CultivateType = ECultivateType::ECT_Corn;
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, TEXT("开始种植玉米"));
		
	}
	else if (Command.EqualTo(FText::FromString("取消种植")))
	{
		CultivateType = ECultivateType::ECT_None;
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, TEXT("取消种植"));
	}
}

void ACultivateChamber::UpdateEachHour_Implementation(int32 NewHour)
{
	Super::UpdateEachHour_Implementation(NewHour);
}

void ACultivateChamber::UpdateCultivateProgress()
{
	if (CultivateType != ECultivateType::ECT_None)
	{
		CultivateProgress += 1;
		if (CultivateProgress >= CultivateMaxProgress)
		{
			// 培养完成，产出物品到临时存储区
			int32 ItemID = 0;
			switch (CultivateType)
			{
			case ECultivateType::ECT_Cotton:
				ItemID = 1; // 假设1代表棉花
				break;
			case ECultivateType::ECT_Corn:
				ItemID = 2; // 假设2代表玉米
				break;
			default:
				break;
			}
			if (ItemID != 0)
			{
				TempItemStorage.FindOrAdd(ItemID) += 1;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("培养完成，产出物品ID: %d"), ItemID));
			}
			// 重置培养状态
			CultivateType = ECultivateType::ECT_None;
			CultivateProgress = 0;
		}
	}
}

void ACultivateChamber::UpdateEachMinute_Implementation(int32 NewMinute)
{
	Super::UpdateEachMinute_Implementation(NewMinute);
	// 每分钟更新培养进度
	ProduceTimeTracker.TimeAccumulator += 1;
	if (ProduceTimeTracker.TimeAccumulator >= ProduceTimeTracker.MinutesInterval)
	{
		ProduceTimeTracker.TimeAccumulator = 0;
		UpdateCultivateProgress();
	}
}

ACultivateChamber::ACultivateChamber()
{
	OutputInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("OutputInventory"));
}

