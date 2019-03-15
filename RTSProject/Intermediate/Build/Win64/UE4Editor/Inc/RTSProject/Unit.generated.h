// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef RTSPROJECT_Unit_generated_h
#error "Unit.generated.h already included, missing '#pragma once' in Unit.h"
#endif
#define RTSPROJECT_Unit_generated_h

#define RTSProject_Source_RTSProject_Public_Unit_h_14_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetTarget) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->GetTarget(); \
		P_NATIVE_END; \
	}


#define RTSProject_Source_RTSProject_Public_Unit_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetTarget) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->GetTarget(); \
		P_NATIVE_END; \
	}


#define RTSProject_Source_RTSProject_Public_Unit_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAUnit(); \
	friend struct Z_Construct_UClass_AUnit_Statics; \
public: \
	DECLARE_CLASS(AUnit, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RTSProject"), NO_API) \
	DECLARE_SERIALIZER(AUnit)


#define RTSProject_Source_RTSProject_Public_Unit_h_14_INCLASS \
private: \
	static void StaticRegisterNativesAUnit(); \
	friend struct Z_Construct_UClass_AUnit_Statics; \
public: \
	DECLARE_CLASS(AUnit, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/RTSProject"), NO_API) \
	DECLARE_SERIALIZER(AUnit)


#define RTSProject_Source_RTSProject_Public_Unit_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AUnit(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AUnit) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AUnit); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AUnit); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AUnit(AUnit&&); \
	NO_API AUnit(const AUnit&); \
public:


#define RTSProject_Source_RTSProject_Public_Unit_h_14_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AUnit(AUnit&&); \
	NO_API AUnit(const AUnit&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AUnit); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AUnit); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AUnit)


#define RTSProject_Source_RTSProject_Public_Unit_h_14_PRIVATE_PROPERTY_OFFSET
#define RTSProject_Source_RTSProject_Public_Unit_h_11_PROLOG
#define RTSProject_Source_RTSProject_Public_Unit_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	RTSProject_Source_RTSProject_Public_Unit_h_14_PRIVATE_PROPERTY_OFFSET \
	RTSProject_Source_RTSProject_Public_Unit_h_14_RPC_WRAPPERS \
	RTSProject_Source_RTSProject_Public_Unit_h_14_INCLASS \
	RTSProject_Source_RTSProject_Public_Unit_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define RTSProject_Source_RTSProject_Public_Unit_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	RTSProject_Source_RTSProject_Public_Unit_h_14_PRIVATE_PROPERTY_OFFSET \
	RTSProject_Source_RTSProject_Public_Unit_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	RTSProject_Source_RTSProject_Public_Unit_h_14_INCLASS_NO_PURE_DECLS \
	RTSProject_Source_RTSProject_Public_Unit_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID RTSProject_Source_RTSProject_Public_Unit_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
