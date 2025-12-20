// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RimSpaceActorBase.h"
#include "Data/TaskInfo.h"
#include "WorkStation.generated.h"

/**
 * 
 */
UCLASS()
class RIMSPACE_API AWorkStation : public ARimSpaceActorBase
{
	GENERATED_BODY()
public:
	// UI相关接口
	virtual TArray<FText> GetCommandList() const override;
	virtual FString GetActorInfo() const override;

	AWorkStation();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<class UInventoryComponent> InputInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<class UInventoryComponent> OutputInventory;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WorkStation", meta = (AllowPrivateAccess = "true"))
	TMap<int32, int32> RawMaterialStorage; //todo: 后续可以改成TMap<FName, int32>或者使用enum

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WorkStationr", meta = (AllowPrivateAccess = "true"))
	TMap<int32, int32> ProductStorage; //todo: 后续可以改成TMap<FName, int32>或者使用enum

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WorkStation", meta = (AllowPrivateAccess = "true"))
	TMap<int32, int32> TaskList; // TaskID, 剩余任务数
};
