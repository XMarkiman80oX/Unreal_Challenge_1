// Fill out your copyright notice in the Description page of Project Settings.


#include "FactoryMaterial.h"

AFactoryMaterial::AFactoryMaterial()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AFactoryMaterial::BeginPlay()
{
	Super::BeginPlay();
}

void AFactoryMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EMaterialType AFactoryMaterial::GetMaterialType()
{
	return this->Material_Type;
}

void AFactoryMaterial::SetMaterialType(EMaterialType Material_Type_Param)
{
	this->Material_Type = Material_Type_Param;
}

//AFactory_Material::AFactory_Material(FName Material_Name)
//{
//	this->Material_Name = Material_Name;
//
//	if (this->Material_Name == "Furnace") {
//		this->Production_Rate = 2.5f;
//	}
//	else if (this->Material_Name == "Sewing Machine") {
//		this->Production_Rate = 10.0f;
//	}
//}


//void UFactory_Material::SetMaterialName(FName Material_Name_Param)
//{
//	this->Material_Name = Material_Name_Param; 
//
//	if (this->Material_Name == "Steel Beam") {
//		this->Production_Rate = 2.5f;
//	}
//	else if (this->Material_Name == "Sewing Machine") {
//		this->Production_Rate = 10.0f;
//	}
//}

