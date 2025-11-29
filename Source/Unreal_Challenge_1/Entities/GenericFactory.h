// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FactoryMaterial.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericFactory.generated.h"

class AFactoryMaterial;
class UFactoryStats;

UENUM(BlueprintType)
enum class EFactoryType : uint8 {
	SEWING_MACHINE_FACTORY,
	FURNACE,
	IRON_MINE,
	COAL_MINE,
	LUMBERJACKS_HUT
};

UCLASS()
class UNREAL_CHALLENGE_1_API AGenericFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AGenericFactory(FName Factory_Name);
	AGenericFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//UPROPERTY(VisibleAnywhere) bool ProductionHasBegun = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) FString FactoryName;
	float StartTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) float Production_Rate = 1.0f;
	UPROPERTY(VisibleAnywhere) float Time_Elapsed = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) float Time_Elapsed_Float = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) float Time_Elapsed_Percentage = 0.0f;

	UPROPERTY(EditAnywhere) EFactoryType Factory_Type;
	UPROPERTY(EditAnywhere) USceneComponent* Mesh_Component = NULL;

	int Max_Num_Of_Materials = 3;

	FTimerHandle ProductionTimerHandle;

	UPROPERTY(VisibleAnywhere) TArray<AFactoryMaterial*> Produced_Materials;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int MaterialsCount = 0;

protected:
	virtual void ProduceMaterial();
	UFUNCTION(BlueprintCallable)
	TMap<EFactoryType, int> SendMaterials();

	void CalculateProgress();

public:
	UFUNCTION(BlueprintCallable)
	EFactoryType GetFactoryType();

	int GetProducedMaterialsCount();

	UFUNCTION(BlueprintCallable)
	FString GetFactoryName();
	
};
