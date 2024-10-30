
// include
#include <GgNc_BlueprintFunctionLibrary.h>

// class
void UGgNc_BlueprintFunctionLibrary::InitArrayBool(TArray<bool>& Array, const bool& Value) noexcept
{
	for (auto& Element : Array)
	{
		Element = Value;
	}
}

void UGgNc_BlueprintFunctionLibrary::InitArrayInt(TArray<int32>& Array, const int32& Value) noexcept
{
	for (auto& Element : Array)
	{
		Element = Value;
	}
}

void UGgNc_BlueprintFunctionLibrary::InitArrayFloat(TArray<float>& Array, const float& Value) noexcept
{
	for (auto& Element : Array)
	{
		Element = Value;
	}
}

void UGgNc_BlueprintFunctionLibrary::InitArrayVector3(TArray<FVector>& Array, const FVector& Value) noexcept
{
	for (auto& Element : Array)
	{
		Element = Value;
	}
}

void UGgNc_BlueprintFunctionLibrary::InitArrayVector4(TArray<FVector4>& Array, const FVector4& Value) noexcept
{
	for (auto& Element : Array)
	{
		Element = Value;
	}
}
