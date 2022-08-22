// Shoot Game


#include "Puckups/ShootAmmoPickup.h"
#include "Components/ShootHealtsComponent.h"	//подключили
#include "Weapon/ShootWeaponComponent.h"		//подключили
#include "ShootUtils.h"							//подключили

//лоиг пикапа
DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

//получает боезапас для определенного типа оружия
bool AShootAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	//получаем компонент здоровья
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent<UShootHealtsComponent>(PlayerPawn);	//Эта функция в виджетах
	
	//проверяем жив ли персонаж ||(или) существует ли компонент
	if (!HealthComponent || HealthComponent->IsDead()) return false;	//сделали так как персонаж мог упасть мертвым на неё

	//получаем компонент оружия
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent<UShootWeaponComponent>(PlayerPawn);	//Эта функция в виджетах

	//проверяем существует ли компонент
	if (!WeaponComponent) return false;

	//UE_LOG(LogAmmoPickup, Display, TEXT("Pickup Ammo was taken"));
	
	//если существует вызываем 
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);	//передаем тип оружия и количество патронов
}