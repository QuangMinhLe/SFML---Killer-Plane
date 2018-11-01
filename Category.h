#pragma once

namespace Category {
	enum Type {
		None			= 0,
		Scene			= 1 << 0,		//bit shift
		PlayerAircraft	= 1 << 1,		// 00000000000
		AlliedAircraft	= 1 << 2,		// 00000000001
		EnemyAircraft	= 1 << 3,		// 00000000010
		EnemyProjectile = 1 << 4,		// 00000000100
		AlliedProjectile= 1 << 5,		// 00000001000
		AirSceneLayer	= 1 << 6,		// 00000010000
		Pickup			= 1 << 7,
		ParticleSystem  = 1 << 8,

		Aircraft		= PlayerAircraft | AlliedAircraft | EnemyAircraft,
		Projectile		= EnemyProjectile | AlliedProjectile,
	};
}