

#include "Weapon/Components/ShootWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"	//для ниагары подключиил
#include "PhysicalMaterials/PhysicalMaterial.h" //для физического материала подключили
#include "Kismet/GameplayStatics.h"		//подключили для декалей
#include "Components/DecalComponent.h"	//подключили для декалей




UShootWeaponFXComponent::UShootWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

//включение ниагры и декалей
void UShootWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{

	//изменили из-за декалей auto Effect = DefaultEffect;	//значение еффекта присвоили в переменную. Будет меняться взависимости куда попали
	auto ImpactData = DefaultImpactData;	//значение еффекта присвоили в переменную. Будет меняться взависимости куда попали

	//в попадании получаем инфу о физ материале и проверяем на существование
	if (Hit.PhysMaterial.IsValid())
	{
		//если истина то получаем доступ к сырому указателю
		const auto PhysMat = Hit.PhysMaterial.Get();	//получаем указатель на физический материал т.е. возвращает нам какой то физ материал

		//проверка существует ли эффект для выбранного физ материала
		//изменили из-за декалей if (EffectsMap.Contains(PhysMat))	//Contains - проверяет существует ли ключ который передали в аргумент функции, нашего контейнера
		if (ImpactDataMap.Contains(PhysMat))	//Contains - проверяет существует ли ключ который передали в аргумент функции, нашего контейнера
		{
			//если существует, забираем соответствующее ему значение и переопределяем эффект который хотим заспаунить
			//изменили из-за декалей Effect = EffectsMap[PhysMat];	//для доступа значения по ключу [], внутри указатель на физ материал
			ImpactData = ImpactDataMap[PhysMat];	//для доступа значения по ключу [], внутри указатель на физ материал
		}
	}

	//спавн ниагару в мир. Проверки указателя ниагары и мира внутри, поэтому не нужны 
	//изменили из-за декалейUNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());	//1 где 2 указатель на ниагару систему  3 локация в мире (получили через импакт) 4 ориентация в пространстве
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());	//1 где 2 указатель на ниагару систему  3 локация в мире (получили через импакт) 4 ориентация в пространстве

	//спавн декали в мир		//SpawnDecalAttached - спавн и присоединяем к компоненту на сцене.  
	//функция спавна декали возвращает указатель на компонент декали. поэтому создали переменную DecalComponent и в нее помещаем указатель функции спавна, это надо для жизни декали
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //1 указатель на мир
		ImpactData.DecalData.Material, //2 материал декали( в нашей структуре)
		ImpactData.DecalData.Size, //3 размер декали
		Hit.ImpactPoint, //4. локация декали в мире (так же делали в ниагаре)
		Hit.ImpactNormal.Rotation());	//5 Вращение декали. Такое как при спавне ниагары системы использовали

		//если не нулевой то появляется декаль
		if(DecalComponent)
		{
			DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);	//SetFadeOut, SetFadeIn - появление декали 1 через какое время запуститься анимация 2 время анимации счезновения 3. удаление owner декали, используется если пользовались SpawnDecalAttached
		}	 
}