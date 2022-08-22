
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"			//подключили для декалей и ниагары
#include "ShootWeaponFXComponent.generated.h"


class UNiagaraSystem; //форвард диклорейшен
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);	//FHitResult - тут хранится инфа спауна, положение в мире, нормаль и др.

protected:

	//эффект который хотим заспаунить
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "VFX")
		//изменили из-за добавления декалей UNiagaraSystem* DefaultEffect;			//применяется к физ материалу который не можем определить
		FImpactData DefaultImpactData;				//содержать будет как декали, так и ниагару систему

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "VFX")
		//изменили из-за добавления декалей TMap<UPhysicalMaterial*, UNiagaraSystem*> EffectsMap;//TMap - хранит данные как комбинация ключа + значение, принимает типы которые хотим в нем хранить. Ключ будет = физ материал, значение = ниагара система
		TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap; //TMap - хранит данные как комбинация ключа + значение, принимает типы которые хотим в нем хранить.Ключ будет = физ материал, значение = ImpactDataMap
};
