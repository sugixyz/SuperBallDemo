#include "HealthComponent.h"

HealthComponent::HealthComponent()
	:maxHp_(100.0f),
	currentHp_(100.0f)
{}

HealthComponent::~HealthComponent()
{}

void HealthComponent::Initialize(float maxHp)
{
	maxHp_ = maxHp;
	currentHp_ = maxHp_;
}

void HealthComponent::TakeDamage(float damage)
{
	//死亡していたら無視
	if (isDead())return;

	//ダメージを受ける
	currentHp_ -= damage;
	currentHp_ = Clamp(currentHp_, maxHp_);
}

void HealthComponent::TakeDamage(int damage)
{
	TakeDamage(static_cast<float>(damage));
}

void HealthComponent::Heal(float amount)
{
	//死亡していたら無視
	if (isDead())return;

	//回復する
	currentHp_ += amount;
	currentHp_ = Clamp(currentHp_, maxHp_);
}

void HealthComponent::Heal(int amount)
{
	Heal(static_cast<float>(amount));
}

bool HealthComponent::isDead()
{
	return (currentHp_ <= 0.0f);
}

float HealthComponent::GetHpRate()
{
	return currentHp_ / maxHp_;
}

float HealthComponent::Clamp(float currentValue, float maxValue, float minValue)
{
	if (currentValue > maxValue)currentValue = maxValue;
	if (currentValue < minValue)currentValue = minValue;

	return currentValue;
}
