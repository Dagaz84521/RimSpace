#pragma once
UENUM(BlueprintType)
enum class EInteractionType: uint8
{
	EAT_None UMETA(DisplayName = "无"),
	EAT_Chamber UMETA(DisplayName = "培养仓"),
	EAT_Kitchen UMETA(DisplayName = "厨房"),
	EAT_WorkStation UMETA(DisplayName = "工作台"),
};

UENUM(BlueprintType)
enum class EShowInfoType: uint8
{
	EIT_Character UMETA(DisplayName = "角色"),
	EIT_Storage UMETA(DisplayName = "仓库"),
	EIT_Chamber UMETA(DisplayName = "培养仓"),
	EIT_Kitchen UMETA(DisplayName = "厨房"),
	EIT_WorkStation UMETA(DisplayName = "工作台"),
	EIT_Fridge UMETA(DisplayName = "冰箱"),
};

