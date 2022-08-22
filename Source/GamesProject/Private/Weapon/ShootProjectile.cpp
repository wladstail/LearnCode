// Shoot Game

#include "Weapon/ShootProjectile.h"
#include "Components/SphereComponent.h"                 //компонент сферы
#include "GameFramework/ProjectileMovementComponent.h"  //компонент изменения по тику
#include "DrawDebugHelpers.h"                              //для рисования сферы урона
#include "Kismet/GameplayStatics.h"                     //для нанесения ущерба
#include "Weapon/Components/ShootWeaponFXComponent.h"	//для ниагары 


AShootProjectile::AShootProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
        
    //создали сферу и колизию у снаряда
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);                                         //радиус коллизии
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);              //физического столкновения не будет, только оповещения о столкновении коллизии
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);   //реагирование коллизии на другие объекты каналов . Блокироваться на все каналы будет

    //HitResult определяется  при столкновении коллизии, поэтому включаем возвращение физического материала для HitResult
    CollisionComponent->bReturnMaterialOnMove = true;

    SetRootComponent(CollisionComponent);

    //компонент логический поэтому атачь не надо, это то что изменяется актор по тику
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;      //этим явно указываем, что скорость будет задаваться через компонент, а не проперти
    MovementComponent->ProjectileGravityScale = 0.0f;   //гравитация установленная

    //компонент ниагары создаем для гранатомета
    WeaponFXComponent = CreateDefaultSubobject<UShootWeaponFXComponent>("WeaponFXComponent");
}

void AShootProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    check(MovementComponent);   
    check(CollisionComponent);
    //проверка что существует ниагары эффект
    check(WeaponFXComponent);

    //траектория равномерного прямолинейного движения(в игре в gravity Scale можно установить силя тяжести)
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;  //Velocity - отвечает на скорость и вычисляется как = единичный вектор направления движения(ShotDirection) * значение скорости(InitialSpeed) не сделали проперти т.к. вся информация тут должна быть
   /* SetLifeSpan(5.0f);
   */

    //игнорирования акторов во время движения снаряда
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);    //1 указатель на актора(владелец персонаж) 2 игнорировать его или нет

    //удаление снаряда, что бы не было утечки памяти и столкновение снаряда
    CollisionComponent->OnComponentHit.AddDynamic(this, &AShootProjectile::OnProjectileHit);//OnComponentHit - вызывается при столкновении, динамический делегал. 1 - указатель на текущий объект, 2 ссылка на функцию.
    SetLifeSpan(LifeSeconds);
}

//логика столкновения и поведения её
void AShootProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)    //F12 сигнатура там
{
    //проверка на нуль
    if (!GetWorld()) return;

    //останавливаем движения снаряда
    MovementComponent->StopMovementImmediately();

    // наносим урон радиальный
    UGameplayStatics::ApplyRadialDamage(GetWorld(),  //мир
        DamageAmount,                                //количество урона
        GetActorLocation(),                          //цент сферы
        DamageRadius,                                //радиус сферы
        UDamageType::StaticClass(),                  //тип урона
        { GetOwner() },                              //массив актора который игнориет ущерб
        this,                                        //указатель на актора кто нанес урон
        GetController(),                             //указатель на контроллер персонажа, Нужен будет для подсчета кто кого подстрелил иначе nullptr
        DoFullDamage);                               //наносится ли урон от эпицентра постепенно или сразу


    //виртуальная сфера что визуализирует взрыв
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    //эффект столкновения ниагры
    WeaponFXComponent->PlayImpactFX(Hit);   //передали хит, который пришел нам в CallBack

    Destroy();  //уничтожения актора
}

//проверка является ли он pawn или нет, если да, то возвращает контроллер персонажа. Нужен будет для подсчета кто кого подстрелил
AController* AShootProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}