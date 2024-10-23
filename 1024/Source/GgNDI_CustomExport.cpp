// Fill out your copyright notice in the Description page of Project Settings.

#include "GgNDI_CustomExport.h"
#include "NiagaraCompileHashVisitor.h"
#include "NiagaraTypes.h"
#include "NiagaraComputeExecutionContext.h"
#include "NiagaraGpuComputeDispatchInterface.h"
#include "NiagaraGpuReadbackManager.h"
#include "NiagaraGPUSystemTick.h"
#include "NiagaraShaderParametersBuilder.h"
#include "NiagaraSystem.h"
#include "NiagaraSystemInstance.h"
#include "NiagaraWorldManager.h"

#include "Internationalization/Internationalization.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ShaderCompilerCore.h"
#include "Misc/LargeWorldRenderPosition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GgNDI_CustomExport)

namespace GgNDI_NDIExportLocal
{
	BEGIN_SHADER_PARAMETER_STRUCT(FShaderParameters, )
		SHADER_PARAMETER(uint32,						WriteBufferSize)
		SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<uint>,	RWWriteBuffer)
	END_SHADER_PARAMETER_STRUCT()

	static const TCHAR*		TemplateShaderFile = TEXT("/Project/CustomNiagaraDataInterfaceExportTemplate.ush");

	static const FName		StoreDataName_DEPRECATED(TEXT("StoreParticleData"));
	static const FName		ExportDataName(TEXT("ExportParticleData"));

	static constexpr uint32 NumFloatsPerInstance = 8;

	static int GGPUMaxReadbackCount = 1000;
	static FAutoConsoleVariableRef CVarGPUMaxReadbackCount(
		TEXT("fx.Niagara.NDIExport.GPUMaxReadbackCount"),
		GGPUMaxReadbackCount,
		TEXT("Maximum buffer instance count for the GPU readback when in PerParticleMode, where <= 0 means ignore."),
		ECVF_Default
	);

	void EnqueueGameThreadCallback(TWeakObjectPtr<UObject> WeakCallbackHandler, TArray<FGgNDI_BasicParticleData> ParticleData, TWeakObjectPtr<UNiagaraSystem> WeakSystem, FVector3f SystemTileOffset)
	{
		FNiagaraWorldManager::EnqueueGlobalDeferredCallback(
			[WeakCallbackHandler, ParticleData=MoveTemp(ParticleData), WeakSystem, SystemTileOffset]()
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(NiagaraExportGTCallback);

				UNiagaraSystem* System = WeakSystem.Get();
				if (System == nullptr)
				{
					//UE_LOG(LogNiagara, Warning, TEXT("Invalid system handle in export data interface callback, skipping"));
					UE_LOG(LogTemp, Warning, TEXT("Invalid system handle in export data interface callback, skipping"));
					return;
				}

				UObject* CallbackHandler = WeakCallbackHandler.Get();
				if (CallbackHandler == nullptr)
				{
					//UE_LOG(LogNiagara, Warning, TEXT("Invalid CallbackHandler in export data interface callback, skipping"));
					UE_LOG(LogTemp, Warning, TEXT("Invalid CallbackHandler in export data interface callback, skipping"));
					return;
				}

				IGgNDI_NiagaraParticleCallbackHandler::Execute_CustomReceiveParticleData(CallbackHandler, ParticleData, System, FVector(SystemTileOffset) * FLargeWorldRenderScalar::GetTileSize());
			}
		);
	}
}

//////////////////////////////////////////////////////////////////////////
// Instance and Proxy Data

struct FGgNDI_NDIExportInstanceData_GameThread
{
	TWeakObjectPtr<UObject> CallbackHandler;

	/** We use a lock-free queue here because multiple threads might try to push data to it at the same time. */
	TQueue<FGgNDI_BasicParticleData, EQueueMode::Mpsc> GatheredData;

	/** A binding to the user ptr we're reading the handler from. */
	FNiagaraParameterDirectBinding<UObject*> UserParamBinding;
};

struct FGgNDI_NDIExportInstanceData_RenderThread
{
	EGgNDI_NDIExport_GPUAllocationMode	AllocationMode = EGgNDI_NDIExport_GPUAllocationMode::FixedSize;
	int									AllocationFixedSize = 0;
	float								AllocationPerParticleSize = 0.0f;

	TWeakObjectPtr<UObject>				WeakCallbackHandler;
	TWeakObjectPtr<UNiagaraSystem>		WeakSystem;

	bool								bWriteBufferUsed = false;
	uint32								WriteBufferInstanceCount = 0;

	FRDGBufferRef						RDGWriteBuffer = nullptr;
	FRDGBufferUAVRef					RDGWriteBufferUAV = nullptr;
};

struct FGgNDI_NDIExportProxy : public FNiagaraDataInterfaceProxy
{
	FGgNDI_NDIExportProxy() {}
	virtual void ConsumePerInstanceDataFromGameThread(void* PerInstanceData, const FNiagaraSystemInstanceID& Instance) override {}
	virtual int32 PerInstanceDataPassedToRenderThreadSize() const override
	{
		return 0;
	}

	virtual void PostStage(const FNDIGpuComputePostStageContext& Context) override
	{
		FGgNDI_NDIExportInstanceData_RenderThread& InstanceData = SystemInstancesToProxyData_RT.FindChecked(Context.GetSystemInstanceID());

		// If we bound the buffer as a UAV the stage may have written to it
		if (InstanceData.bWriteBufferUsed)
		{
			InstanceData.bWriteBufferUsed = false;

			FNiagaraGpuReadbackManager* ReadbackManager = Context.GetComputeDispatchInterface().GetGpuReadbackManager();
			ReadbackManager->EnqueueReadback(
				Context.GetGraphBuilder(),
				InstanceData.RDGWriteBuffer,
				[MaxInstances= InstanceData.WriteBufferInstanceCount, WeakCallbackHandler=InstanceData.WeakCallbackHandler, WeakSystem=InstanceData.WeakSystem, SystemLWCTile= Context.GetSystemLWCTile()](TConstArrayView<TPair<void*, uint32>> Buffers)
				{
					const uint32 ReadbackInstanceCount = *reinterpret_cast<uint32*>(Buffers[0].Key);
					check(ReadbackInstanceCount <= MaxInstances);
					if (ReadbackInstanceCount > 0)
					{
						// Translate float data into Export Data
						TArray<FGgNDI_BasicParticleData> ExportParticleData;
						ExportParticleData.AddUninitialized(ReadbackInstanceCount);

						const float* FloatData = reinterpret_cast<float*>(Buffers[0].Key) + 1;
						for (uint32 i = 0; i < ReadbackInstanceCount; ++i)
						{
							ExportParticleData[i].Position.X = FloatData[0];
							ExportParticleData[i].Position.Y = FloatData[1];
							ExportParticleData[i].Position.Z = FloatData[2];
							ExportParticleData[i].Size = FloatData[3];
							ExportParticleData[i].Velocity.X = FloatData[4];
							ExportParticleData[i].Velocity.Y = FloatData[5];
							ExportParticleData[i].Velocity.Z = FloatData[6];
							ExportParticleData[i].Age = FloatData[7];

							FloatData += GgNDI_NDIExportLocal::NumFloatsPerInstance;
						}

						GgNDI_NDIExportLocal::EnqueueGameThreadCallback(WeakCallbackHandler, MoveTemp(ExportParticleData), WeakSystem, SystemLWCTile);
					}
				}
			);
		}
		InstanceData.RDGWriteBuffer = nullptr;
		InstanceData.RDGWriteBufferUAV = nullptr;
	}

	TMap<FNiagaraSystemInstanceID, FGgNDI_NDIExportInstanceData_RenderThread> SystemInstancesToProxyData_RT;
};

//////////////////////////////////////////////////////////////////////////
// Data Interface

UGgNDI_CustomExport::UGgNDI_CustomExport(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Proxy.Reset(new FGgNDI_NDIExportProxy());

	FNiagaraTypeDefinition Def(UObject::StaticClass());
	CallbackHandlerParameter.Parameter.SetType(Def);
}

void UGgNDI_CustomExport::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		ENiagaraTypeRegistryFlags Flags = ENiagaraTypeRegistryFlags::AllowAnyVariable | ENiagaraTypeRegistryFlags::AllowParameter;
		FNiagaraTypeRegistry::Register(FNiagaraTypeDefinition(GetClass()), Flags);
	}
}

bool UGgNDI_CustomExport::InitPerInstanceData(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance)
{
	FGgNDI_NDIExportInstanceData_GameThread* PIData = new (PerInstanceData) FGgNDI_NDIExportInstanceData_GameThread;

	ENQUEUE_RENDER_COMMAND(FNDIExport_AddProxy)
	(
		[RT_Proxy=GetProxyAs<FGgNDI_NDIExportProxy>(), RT_InstanceID=SystemInstance->GetId(), RT_AllocationMode=GPUAllocationMode, RT_AllocationFixedSize=GPUAllocationFixedSize, RT_AllocationPerParticleSize=GPUAllocationPerParticleSize](FRHICommandListImmediate& RHICmdList)
		{
			check(!RT_Proxy->SystemInstancesToProxyData_RT.Contains(RT_InstanceID));
			FGgNDI_NDIExportInstanceData_RenderThread* InstanceData = &RT_Proxy->SystemInstancesToProxyData_RT.Add(RT_InstanceID);
			check(InstanceData);

			InstanceData->AllocationMode = RT_AllocationMode;
			InstanceData->AllocationFixedSize = RT_AllocationFixedSize;
			InstanceData->AllocationPerParticleSize = RT_AllocationPerParticleSize;
		}
	);

	return true;
}

void UGgNDI_CustomExport::DestroyPerInstanceData(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance)
{
	FGgNDI_NDIExportInstanceData_GameThread* InstData = (FGgNDI_NDIExportInstanceData_GameThread*)PerInstanceData;
	InstData->~FGgNDI_NDIExportInstanceData_GameThread();

	ENQUEUE_RENDER_COMMAND(FNDIExport_RemoveProxy)
	(
		[RT_Proxy=GetProxyAs<FGgNDI_NDIExportProxy>(), InstanceID=SystemInstance->GetId()](FRHICommandListImmediate& CmdList)
		{
			RT_Proxy->SystemInstancesToProxyData_RT.Remove(InstanceID);
		}
	);
}

int32 UGgNDI_CustomExport::PerInstanceDataSize() const
{
	return sizeof(FGgNDI_NDIExportInstanceData_GameThread);
}


bool UGgNDI_CustomExport::PerInstanceTick(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance, float DeltaSeconds)
{
	check(SystemInstance);
	FGgNDI_NDIExportInstanceData_GameThread* PIData = (FGgNDI_NDIExportInstanceData_GameThread*)PerInstanceData;
	if (!PIData)
	{
		return true;
	}

	TWeakObjectPtr<UObject> PreviousCallbackHandler = PIData->CallbackHandler;
	if (CallbackHandlerParameter.Parameter.IsValid() && SystemInstance)
	{
		//-OPT: Don't need to init this per frame, could be bound once and read per frame
		PIData->CallbackHandler = PIData->UserParamBinding.Init(SystemInstance->GetInstanceParameters(), CallbackHandlerParameter.Parameter);
	}
	else
	{
		PIData->CallbackHandler.Reset();
	}

	// If we switched modify the RT data
	if (PreviousCallbackHandler != PIData->CallbackHandler)
	{
		ENQUEUE_RENDER_COMMAND(FNDIExport_UpdateHandler)
		(
			[RT_Proxy=GetProxyAs<FGgNDI_NDIExportProxy>(), RT_InstanceID=SystemInstance->GetId(), RT_WeakHandler=PIData->CallbackHandler, RT_WeakSystem=SystemInstance->GetSystem()](FRHICommandListImmediate& RHICmdList)
			{
				FGgNDI_NDIExportInstanceData_RenderThread* InstanceData = RT_Proxy->SystemInstancesToProxyData_RT.Find(RT_InstanceID);
				if (ensure(InstanceData != nullptr))
				{
					InstanceData->WeakCallbackHandler = RT_WeakHandler;
					InstanceData->WeakSystem = RT_WeakSystem;
				}
			}
		);
	}

	return false;
}

bool UGgNDI_CustomExport::PerInstanceTickPostSimulate(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance, float DeltaSeconds)
{
	FGgNDI_NDIExportInstanceData_GameThread* PIData = static_cast<FGgNDI_NDIExportInstanceData_GameThread*>(PerInstanceData);
	if ( !PIData->GatheredData.IsEmpty() )
	{
		UObject* CallbackHandler = PIData->CallbackHandler.Get();
		if ( CallbackHandler && CallbackHandler->GetClass()->ImplementsInterface(UGgNDI_NiagaraParticleCallbackHandler::StaticClass()) )
		{
			//Drain the queue into an array here
			TArray<FGgNDI_BasicParticleData> Data;
			FGgNDI_BasicParticleData Value;
			while (PIData->GatheredData.Dequeue(Value))
			{
				Data.Add(Value);
			}

			GgNDI_NDIExportLocal::EnqueueGameThreadCallback(CallbackHandler, MoveTemp(Data), SystemInstance->GetSystem(), SystemInstance->GetLWCTile());
		}
	}
	return false;
}

bool UGgNDI_CustomExport::Equals(const UNiagaraDataInterface* Other) const
{
	if (!Super::Equals(Other))
	{
		return false;
	}

	const UGgNDI_CustomExport* OtherTyped = CastChecked<const UGgNDI_CustomExport>(Other);

	return
		OtherTyped->CallbackHandlerParameter == CallbackHandlerParameter &&
		OtherTyped->GPUAllocationMode == GPUAllocationMode &&
		OtherTyped->GPUAllocationFixedSize == GPUAllocationFixedSize &&
		OtherTyped->GPUAllocationPerParticleSize == GPUAllocationPerParticleSize;
}

#if WITH_EDITORONLY_DATA

void UGgNDI_CustomExport::GetFunctionsInternal(TArray<FNiagaraFunctionSignature>& OutFunctions) const
{
	FNiagaraFunctionSignature SigOld;
	SigOld.Name = GgNDI_NDIExportLocal::StoreDataName_DEPRECATED;
	SigOld.Description = NSLOCTEXT("Niagara", "ExportDataFunctionDescription", "This function takes the particle data and stores it to be exported to the registered callback handler after the simulation has ticked.");
	SigOld.bMemberFunction = true;
	SigOld.bRequiresContext = false;
	SigOld.bSupportsGPU = true;
	SigOld.bSoftDeprecatedFunction = true;
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("Export interface")));
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Store Data")));
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Size")));
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Velocity")));
	SigOld.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Age")));
	SigOld.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Success")));
	OutFunctions.Add(SigOld);

	FNiagaraFunctionSignature Sig;
	Sig.Name = GgNDI_NDIExportLocal::ExportDataName;
	Sig.Description = NSLOCTEXT("Niagara", "ExportDataFunctionDescription", "This function takes the particle data and stores it to be exported to the registered callback handler after the simulation has ticked.");
	Sig.bMemberFunction = true;
	Sig.bRequiresContext = false;
	Sig.bSupportsGPU = true;
	Sig.bRequiresExecPin = true;
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("Export interface")));
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Store Data")));
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Size")));
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Velocity")));
	Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Age")));
	Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("WasExported")));
	OutFunctions.Add(Sig);
}

void UGgNDI_CustomExport::GetParameterDefinitionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, FString& OutHLSL)
{
	const TMap<FString, FStringFormatArg> TemplateArgs =
	{
		{TEXT("ParameterName"),	ParamInfo.DataInterfaceHLSLSymbol},
	};
	AppendTemplateHLSL(OutHLSL, GgNDI_NDIExportLocal::TemplateShaderFile, TemplateArgs);
}

bool UGgNDI_CustomExport::GetFunctionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, const FNiagaraDataInterfaceGeneratedFunction& FunctionInfo, int FunctionInstanceIndex, FString& OutHLSL)
{
	using namespace GgNDI_NDIExportLocal;

	if ( (FunctionInfo.DefinitionName == StoreDataName_DEPRECATED) || (FunctionInfo.DefinitionName == ExportDataName) )
	{
		return true;
	}

	// Invalid function
	return false;
}

bool UGgNDI_CustomExport::AppendCompileHash(FNiagaraCompileHashVisitor* InVisitor) const
{
	bool bSuccess = Super::AppendCompileHash(InVisitor);
	bSuccess &= InVisitor->UpdateShaderFile(GgNDI_NDIExportLocal::TemplateShaderFile);
	return bSuccess;
}

#endif

void UGgNDI_CustomExport::BuildShaderParameters(FNiagaraShaderParametersBuilder& ShaderParametersBuilder) const
{
	ShaderParametersBuilder.AddNestedStruct<GgNDI_NDIExportLocal::FShaderParameters>();
}

void UGgNDI_CustomExport::SetShaderParameters(const FNiagaraDataInterfaceSetShaderParametersContext& Context) const
{
	FGgNDI_NDIExportProxy& DIProxy = Context.GetProxy<FGgNDI_NDIExportProxy>();
	FGgNDI_NDIExportInstanceData_RenderThread& InstanceData_RT = DIProxy.SystemInstancesToProxyData_RT.FindChecked(Context.GetSystemInstanceID());

	GgNDI_NDIExportLocal::FShaderParameters* Parameters = Context.GetParameterNestedStruct<GgNDI_NDIExportLocal::FShaderParameters>();
	if ( Context.IsResourceBound(&Parameters->RWWriteBuffer) )
	{
		if ( !InstanceData_RT.WeakCallbackHandler.IsExplicitlyNull() )
		{
			FRDGBuilder& GraphBuilder = Context.GetGraphBuilder();
			if (InstanceData_RT.RDGWriteBuffer == nullptr)
			{
				// Ensure our buffer if big enough to hold all the data
				const int32 AllocationRounding = 64;
				const int32 DataBufferNumInstances = Context.GetComputeInstanceData().Context->CurrentMaxInstances_RT;
				int32 NumInstances = 0;
				if (InstanceData_RT.AllocationMode == EGgNDI_NDIExport_GPUAllocationMode::PerParticle)
				{
					NumInstances = int32(float(DataBufferNumInstances) * InstanceData_RT.AllocationPerParticleSize);
					NumInstances = FMath::Max(NumInstances, 0);
					if (GgNDI_NDIExportLocal::GGPUMaxReadbackCount > 0)
					{
						NumInstances = FMath::Min(NumInstances, GgNDI_NDIExportLocal::GGPUMaxReadbackCount);
					}
				}
				else
				{
					NumInstances = InstanceData_RT.AllocationFixedSize;
				}

				InstanceData_RT.WriteBufferInstanceCount = NumInstances;

				const int32 AllocationNumInstances = FMath::DivideAndRoundUp(NumInstances, AllocationRounding) * AllocationRounding;
				const int32 RequiredElements = 1 + (AllocationNumInstances * GgNDI_NDIExportLocal::NumFloatsPerInstance);
				const int32 RequiredBytes = sizeof(float) * RequiredElements;

				FRDGBufferDesc BufferDesc = FRDGBufferDesc::CreateBufferDesc(sizeof(uint32), RequiredElements);
				BufferDesc.Usage |= BUF_SourceCopy;
				InstanceData_RT.RDGWriteBuffer = GraphBuilder.CreateBuffer(BufferDesc, TEXT("FNDIExportProxy_WriteBuffer"));
				InstanceData_RT.RDGWriteBufferUAV = GraphBuilder.CreateUAV(InstanceData_RT.RDGWriteBuffer, PF_R32_UINT);

			}

			//-OPT: We only need to reset the first 4 bytes for the count
			//-OPT: We only need to clear if we actual use the buffer
			AddClearUAVPass(GraphBuilder, InstanceData_RT.RDGWriteBufferUAV, 0);

			InstanceData_RT.bWriteBufferUsed = true;
			Parameters->WriteBufferSize = InstanceData_RT.WriteBufferInstanceCount;
			Parameters->RWWriteBuffer = InstanceData_RT.RDGWriteBufferUAV;
		}
		else
		{
			Parameters->WriteBufferSize = 0;
			Parameters->RWWriteBuffer = Context.GetComputeDispatchInterface().GetEmptyBufferUAV(Context.GetGraphBuilder(), PF_R32_UINT);
		}
	}
	else
	{
		Parameters->WriteBufferSize	= 0;
		Parameters->RWWriteBuffer = nullptr;
	}
}

DEFINE_NDI_DIRECT_FUNC_BINDER(UGgNDI_CustomExport, StoreData);
DEFINE_NDI_DIRECT_FUNC_BINDER(UGgNDI_CustomExport, ExportData);
void UGgNDI_CustomExport::GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction &OutFunc)
{
	if (BindingInfo.Name == GgNDI_NDIExportLocal::StoreDataName_DEPRECATED)
	{
		NDI_FUNC_BINDER(UGgNDI_CustomExport, StoreData)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GgNDI_NDIExportLocal::ExportDataName)
	{
		NDI_FUNC_BINDER(UGgNDI_CustomExport, ExportData)::Bind(this, OutFunc);
	}
	else
	{
		//UE_LOG(LogNiagara, Display, TEXT("Could not find data interface external function in %s. Expected Name: %s  Actual Name: %s"), *GetPathNameSafe(this), *GgNDI_NDIExportLocal::ExportDataName.ToString(), *BindingInfo.Name.ToString());
		UE_LOG(LogTemp, Display, TEXT("Could not find data interface external function in %s. Expected Name: %s  Actual Name: %s"), *GetPathNameSafe(this), *GgNDI_NDIExportLocal::ExportDataName.ToString(), *BindingInfo.Name.ToString());
	}
}

void UGgNDI_CustomExport::StoreData(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FUserPtrHandler<FGgNDI_NDIExportInstanceData_GameThread> InstData(Context);

	FNDIInputParam<FNiagaraBool> StoreDataParam(Context);
	FNDIInputParam<FVector3f> PositionParam(Context);
	FNDIInputParam<float> SizeParam(Context);
	FNDIInputParam<FVector3f> VelocityParam(Context);
	FNDIInputParam<float> AgeParam(Context);

	FNDIOutputParam<FNiagaraBool> OutSample(Context);

	checkfSlow(InstData.Get(), TEXT("Export data interface has invalid instance data. %s"), *GetPathName());
	bool ValidHandlerData = InstData->UserParamBinding.BoundVariable.IsValid() && InstData->CallbackHandler.IsValid();

	for (int32 i = 0; i < Context.GetNumInstances(); ++i)
	{
		const bool ShouldStore = StoreDataParam.GetAndAdvance();

		FGgNDI_BasicParticleData Data;
		Data.Position = (FVector)PositionParam.GetAndAdvance();
		Data.Size = SizeParam.GetAndAdvance();
		Data.Velocity = (FVector)VelocityParam.GetAndAdvance();
		Data.Age = AgeParam.GetAndAdvance();

		bool Valid = false;
		if (ValidHandlerData && ShouldStore)
		{
			Valid = InstData->GatheredData.Enqueue(Data);
		}
		OutSample.SetAndAdvance(Valid);
	}
}

void UGgNDI_CustomExport::ExportData(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FUserPtrHandler<FGgNDI_NDIExportInstanceData_GameThread> InstData(Context);

	FNDIInputParam<FNiagaraBool> StoreDataParam(Context);
	FNDIInputParam<FVector3f> PositionParam(Context);
	FNDIInputParam<float> SizeParam(Context);
	FNDIInputParam<FVector3f> VelocityParam(Context);
	FNDIInputParam<float> AgeParam(Context);

	FNDIOutputParam<FNiagaraBool> OutWasExported(Context);

	checkfSlow(InstData.Get(), TEXT("Export data interface has invalid instance data. %s"), *GetPathName());
	bool ValidHandlerData = InstData->UserParamBinding.BoundVariable.IsValid() && InstData->CallbackHandler.IsValid();

	for (int32 i = 0; i < Context.GetNumInstances(); ++i)
	{
		const bool ShouldStore = StoreDataParam.GetAndAdvance();

		FGgNDI_BasicParticleData Data;
		Data.Position = (FVector)PositionParam.GetAndAdvance();
		Data.Size = SizeParam.GetAndAdvance();
		Data.Velocity = (FVector)VelocityParam.GetAndAdvance();
		Data.Age = AgeParam.GetAndAdvance();

		bool Valid = false;
		if (ValidHandlerData && ShouldStore)
		{
			Valid = InstData->GatheredData.Enqueue(Data);
		}
		OutWasExported.SetAndAdvance(Valid);
	}
}

bool UGgNDI_CustomExport::CopyToInternal(UNiagaraDataInterface* Destination) const
{
	if (!Super::CopyToInternal(Destination))
	{
		return false;
	}

	UGgNDI_CustomExport* OtherTyped = CastChecked<UGgNDI_CustomExport>(Destination);
	OtherTyped->CallbackHandlerParameter = CallbackHandlerParameter;
	OtherTyped->GPUAllocationMode = GPUAllocationMode;
	OtherTyped->GPUAllocationFixedSize = GPUAllocationFixedSize;
	OtherTyped->GPUAllocationPerParticleSize = GPUAllocationPerParticleSize;
	return true;
}
