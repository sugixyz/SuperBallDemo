#pragma once
#include"Component.h"
#include<DirectXMath.h>

using namespace DirectX;

class HealthComponent : public Component
{
public:
	HealthComponent();
	~HealthComponent() override;

	/// <summary>
	/// 初期化用関数
	/// </summary>
	/// <param name="maxHp">設定する最大HP</param>
	void Initialize(float maxHp);

	/// <summary>
	/// ダメージを受ける関数 float
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	void TakeDamage(float damage);
	/// <summary>
	/// ダメージを受ける関数 int
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	void TakeDamage(int damage);
	/// <summary>
	/// 回復する関数 float
	/// </summary>
	/// <param name="amount">回復量</param>
	void Heal(float amount);
	/// <summary>
	/// 回復する関数 int
	/// </summary>
	/// <param name="amount">回復量</param>
	void Heal(int amount);

	/// <summary>
	/// 死んでいるかどうかを返す関数
	/// </summary>
	/// <returns>死んでいる（HPが0以下）ならtrueを返す</returns>
	bool isDead();

	/// <summary>
	/// 現在のHPを取得する関数
	/// </summary>
	/// <returns>現在のHP</returns>
	float GetHp() { return currentHp_; }
	/// <summary>
	/// 最大HPを取得する関数
	/// </summary>
	/// <returns>最大HP</returns>
	float GetMaxHp() { return maxHp_; }

	/// <summary>
	/// 現在HPの割合を取得する関数
	/// </summary>
	/// <returns>HPの割合</returns>
	float GetHpRate();
private:
	float Clamp(float currentValue, float maxValue, float minValue = 0.0f);
private:
	float maxHp_;
	float currentHp_;
};