#pragma once
#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <memory>



class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(sf::Texture& t_weaponTex, MeleeWeaponType t_type);
	virtual ~MeleeWeapon();
	void update(sf::Time& _deltaTime, sf::Vector2f t_pos)override;
	virtual void render(sf::RenderWindow& t_window) override;
	void attack(sf::Vector2f t_position, sf::Vector2f t_velocity)override;

	std::shared_ptr<ProjectilePool>& getProjectilePool()override { return std::make_shared<ProjectilePool>(); }

	sf::FloatRect getMeleeWeaponAttackArea() override { return weaponSprite.getGlobalBounds() ; }


	MeleeWeaponType getMeleeWeaponType() override { return m_meleeType; }
	RangedWeaponTypes getRangedWeaponType() override { return RangedWeaponTypes::None; }

	int getAmmoCount() override { return 0; }
	void receiveAmmo(unsigned int t_ammoNum)override { }

	bool attacking()override;
	void setAttacking(bool t_value) override { attacked = t_value; }

	void rotateWeapon(float _angle, int _leftorRight);

private:

	void setPos(sf::Vector2f _pos);

	MeleeWeaponType m_meleeType{ MeleeWeaponType::Fist };
	sf::Texture m_meleeWeaponTexture;
	sf::Vector2f m_meleeAreaSize{ 0,0 };

	bool attacked = false;
	bool canAttack{ true };
	//sf::Clock timeSinceLastMelee;
	//sf::Time meleeAttackCooldown = sf::seconds(0.5);

	int attackTimer = 0;
	
	sf::RectangleShape attackRectangle;

	sf::Sprite weaponSprite;

	int swingRotation = 0;
};
