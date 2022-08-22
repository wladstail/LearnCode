

#include "Puckups/ShootHealthPickup.h"
#include "Components/ShootHealtsComponent.h"	//подключили
#include "ShootUtils.h"							//подключили

//лог пикапа
DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);



bool AShootHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	//UE_LOG(LogHealthPickup, Display, TEXT("Pickup Health was taken"));

	//получаем компонент здоровья
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent<UShootHealtsComponent>(PlayerPawn);	//Эта функция в виджетах

	//проверяем  указателя
	if (!HealthComponent) return false;

	//возвращает истину если здоровье успешно было добавлено
	return HealthComponent->TryToAddHealth(HealthAmount);
}