// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericFactory.h"
#include "PreReqFactory.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_CHALLENGE_1_API APreReqFactory : public AGenericFactory
{
	GENERATED_BODY()

public:
	APreReqFactory();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere) TArray<EMaterialType> Required_Materials;

	UPROPERTY(VisibleAnywhere) TArray<AFactoryMaterial*> Received_Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int NumberOfReq1 = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int NumberOfReq2 = 0;

private:
	void ProduceMaterial();
	bool AreMaterialsComplete();

	AFactoryMaterial* InitializeMaterial();
	void SpendMaterial();

private:
	void CountInputMaterials();

public:
	UFUNCTION(BlueprintCallable)
	void ReceiveMaterials(TMap<EMaterialType, int> ReceivedMaterials);
	
	UFUNCTION(BlueprintCallable)
	TArray<EMaterialType> GetRequiredMaterials();
};
