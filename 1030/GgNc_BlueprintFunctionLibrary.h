
// include
#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>

// include end
#include <GgNc_BlueprintFunctionLibrary.generated.h>

// class
UCLASS()
class PROJECTC_5_4_API UGgNc_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//===== Array =====

	/* Init Array Bool */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void InitArrayBool(UPARAM(ref) TArray<bool>& Array, UPARAM(ref) const bool& Value) noexcept;

	/* Init Array Int */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void InitArrayInt(UPARAM(ref) TArray<int32>& Array, UPARAM(ref) const int32& Value) noexcept;

	/* Init Array Float */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void InitArrayFloat(UPARAM(ref) TArray<float>& Array, UPARAM(ref) const float& Value) noexcept;

	/* Init Array Vector3 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void InitArrayVector3(UPARAM(ref) TArray<FVector>& Array, UPARAM(ref) const FVector& Value) noexcept;

	/* Init Array Vector4 */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void InitArrayVector4(UPARAM(ref) TArray<FVector4>& Array, UPARAM(ref) const FVector4& Value) noexcept;
};
