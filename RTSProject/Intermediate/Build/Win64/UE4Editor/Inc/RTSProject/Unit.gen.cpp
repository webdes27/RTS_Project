// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RTSProject/Public/Unit.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUnit() {}
// Cross Module References
	RTSPROJECT_API UClass* Z_Construct_UClass_AUnit_NoRegister();
	RTSPROJECT_API UClass* Z_Construct_UClass_AUnit();
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	UPackage* Z_Construct_UPackage__Script_RTSProject();
	RTSPROJECT_API UFunction* Z_Construct_UFunction_AUnit_GetTarget();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
// End Cross Module References
	void AUnit::StaticRegisterNativesAUnit()
	{
		UClass* Class = AUnit::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetTarget", &AUnit::execGetTarget },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AUnit_GetTarget_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AUnit_GetTarget_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Unit.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AUnit_GetTarget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AUnit, "GetTarget", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AUnit_GetTarget_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_AUnit_GetTarget_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AUnit_GetTarget()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AUnit_GetTarget_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AUnit_NoRegister()
	{
		return AUnit::StaticClass();
	}
	struct Z_Construct_UClass_AUnit_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_target_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_target;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AUnit_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_RTSProject,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AUnit_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AUnit_GetTarget, "GetTarget" }, // 3723104607
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AUnit_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Unit.h" },
		{ "ModuleRelativePath", "Public/Unit.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AUnit_Statics::NewProp_target_MetaData[] = {
		{ "Category", "Unit" },
		{ "ModuleRelativePath", "Public/Unit.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AUnit_Statics::NewProp_target = { UE4CodeGen_Private::EPropertyClass::Object, "target", RF_Public|RF_Transient|RF_MarkAsNative, (EPropertyFlags)0x0010000000020005, 1, nullptr, STRUCT_OFFSET(AUnit, target), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AUnit_Statics::NewProp_target_MetaData, ARRAY_COUNT(Z_Construct_UClass_AUnit_Statics::NewProp_target_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AUnit_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AUnit_Statics::NewProp_target,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AUnit_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUnit>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AUnit_Statics::ClassParams = {
		&AUnit::StaticClass,
		DependentSingletons, ARRAY_COUNT(DependentSingletons),
		0x009000A0u,
		FuncInfo, ARRAY_COUNT(FuncInfo),
		Z_Construct_UClass_AUnit_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UClass_AUnit_Statics::PropPointers),
		nullptr,
		&StaticCppClassTypeInfo,
		nullptr, 0,
		METADATA_PARAMS(Z_Construct_UClass_AUnit_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_AUnit_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AUnit()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AUnit_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AUnit, 597815202);
	static FCompiledInDefer Z_CompiledInDefer_UClass_AUnit(Z_Construct_UClass_AUnit, &AUnit::StaticClass, TEXT("/Script/RTSProject"), TEXT("AUnit"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AUnit);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
