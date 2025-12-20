// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include  "Data/TimeTracker.h"
#include "GameFramework/Character.h"
#include "Interface/TimeAffectable.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "RimSpaceCharacterBase.generated.h"

class ARimSpaceActorBase;

USTRUCT(BlueprintType)
struct FRimSpaceCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Hunger; // 饱食度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxHunger; //	最大饱食度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Energy; //  精力值 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxEnergy; // 最大精力值
};

USTRUCT(BlueprintType)
struct FRimSpaceCharacterSkills
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	bool bCanCook; // 烹饪技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	bool bCanFarm; // 农业技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	bool bCanCraft; // 制造技能
};

UCLASS()
class RIMSPACE_API ARimSpaceCharacterBase : public ACharacter, public ITimeAffectable
{
	GENERATED_BODY()

public:
	ARimSpaceCharacterBase();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void UpdateEachMinute_Implementation(int32 NewMinute) override;
	virtual void UpdateEachHour_Implementation(int32 NewHour) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveToPoint(ARimSpaceActorBase* Target); // 移动到指定地点
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	void TakeItem(); // 从当前地点拾取物品
	void PutItem(); // 在当前地点放下携带物品
	void UseCurrent(); // 使用当前地点功能

	// 人物基本属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	FRimSpaceCharacterStats CharacterStats;

	// 人物技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	FRimSpaceCharacterSkills CharacterSkills;
	
	// 人物携带的物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	FString CarriedItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	ARimSpaceActorBase* CurrentPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	ARimSpaceActorBase* TargetPlace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	FTimeTracker HungerDecreaseTimeTracker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	FTimeTracker EnergyDecreaseTimeTracker;

	int32 CurrentMinute;
	int32 CurrentHour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character")
	float HungerDecreaseAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character")
	float EnergyDecreaseAmount = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComponent;
};
