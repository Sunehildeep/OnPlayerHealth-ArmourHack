/*
	Health And Armour Detection
	- Sunehildeep
	V1.2
*/

#if defined ACHEALTH_Included
	#endinput
#endif

#define ACHEALTH_Included

#include <a_samp>

#if !defined FILTERSCRIPT //It's a gamemode
/*============================================
*** DEFINES
==============================================*/
#define IsPlayerPaused(%1) GetTickCount() > (hcInfo[%1][pTick]+2000)

/*============================================
***	INCLUDE FUNCTIONS
==============================================
1.) OnPlayerHealthHack(playerid, Float:original, Float:anticheat);
2.) OnPlayerArmourHack(playerid, Float:original, Float:anticheat);
*/

/*============================================
***	VARIABLES
==============================================*/
enum healthHack
{
	Float:acHealth,
	bool:hImmune,
	bool:aImmune,
	Float:acArmour,
	hcTimer,
	pTick,
	bool:isSpawned
};

new hcInfo[MAX_PLAYERS][healthHack];

// Thanks to Emmet for the Vending Machines locations.
static
	Float:s_aVendingMachines[][] = {

	    {2326.0273, -1645.9480, 14.8270},

	    {2060.1038, -1898.4542, 13.5538},

	    {1729.8389, -1943.8608, 13.5687},

	    {1929.5480, -1772.3799, 13.5469},

	    {1788.3971, -1369.2784, 15.7578},

	    {2352.9934, -1357.0977, 24.3984},

	    {1153.9095, -1460.9143, 15.7969},

	    {200.2008, -107.6411, 1.5513},

	    {1278.5946, 372.2171, 19.5547},

	    {2320.0242, 2532.0378, 10.8203},

	    {2086.5876, 2071.3574, 11.0579},

	    {1519.3347, 1055.2349, 10.8203},

	    {2503.1370, 1244.5089, 10.8203},

	    {373.8246, -178.9297, 1000.6328},

	    {2575.9084, -1284.4164, 1060.9844},

	    {495.9450, -23.5309, 1000.6797},

	    {501.8352, -2.2189, 1000.6797},

	    {2224.4141, -1153.4738, 1025.7969},

	    {2156.6990, 1606.7858, 999.9718},

	    {2209.1143, 1607.1713, 999.9691},

	    {2222.9924, 1606.8201, 999.9758},

	    {-14.7329, 1176.1732, 19.5634},

	    {-862.8334, 1537.4231, 22.5870},

	    {-252.9273, 2597.9426, 62.8582},

	    {-2419.5483, 985.9162, 45.2969},

	    {-2419.5837, 984.5789, 45.2969},

	    {-1349.3037, 492.3072, 11.1953},

	    {-1981.6035, 142.6650, 27.6875},

	    {-2005.6317, -490.8682, 35.5313},

	    {-2034.4124, -490.8690, 35.5313},

	    {-2063.2695, -490.8681, 35.5313},

	    {-2092.0920, -490.8683, 35.5313},

	    {-2119.7566, -423.4371, 35.5313},

	    {-2119.4058, -422.2168, 35.5313},

	    {-2097.2969, -397.5224, 35.5313},

	    {-2068.5955, -397.5209, 35.5313},

	    {-2039.8862, -397.5221, 35.5313},

	    {-2011.1857, -397.5218, 35.5313},

	    {-32.5050, -185.9060, 1003.5469},

	    {-16.1550, -90.8514, 1003.5469},

	    {-15.1533, -139.4374, 1003.5469},

	    {-35.7115, -139.4262, 1003.5469},

        {-19.0527, -57.0465, 1003.5469},

        {-75.2803, 1227.9558, 19.7359},

	    {-252.9929, 2599.7952, 62.8582},

	    {-1455.1244, 2592.4136, 55.8359},

	    {-1349.3669, 493.8326, 11.1953},

	    {-2229.1858, 287.1634, 35.3203},

	    {-33.9020, -186.0259, 1003.5469},

	    {-17.5500, -90.9689, 1003.5469},

	    {-16.4742, -139.5535, 1003.5469},

	    {-36.1270, -57.1352, 1003.5469},

		{1634.0918, -2238.2810, 13.5078},

	    {2480.8667, -1958.5234, 13.5830},

	    {2140.2676, -1161.4753, 23.9922},

	    {2153.8462, -1015.7134, 62.8837},

	    {662.4010, -551.4146, 16.3359},

	    {2271.7383, -77.2105, 26.5812},

	    {2845.7246, 1294.2957, 11.3906},

	    {1398.8699, 2223.3665, 11.0234},

	    {1659.4425, 1722.1097, 10.8281},

	    {2647.6428, 1128.9147, 11.1797},

	    {374.8246, 188.2364, 1008.3893},

	    {361.5366, 159.3573, 1008.3828},

	    {351.6461, 206.0936, 1008.3828},

	    {371.5602, 177.7127, 1019.9844},

	    {331.9078, 177.7589, 1019.9912},

	    {330.6430, 177.7591, 1019.9912},

	    {378.2992, -178.8477, 1000.6328},

	    {316.2407, -140.3572, 999.6016},

	    {500.6049, -2.1070, 1000.6797},

	    {2156.5854, 1607.8103, 999.9692},

	    {2202.4285, 1617.7483, 999.9766},

	    {2209.2668, 1620.4711, 999.9827},

	    {2223.1074, 1602.5835, 999.9734}

};



/*============================================
***	Forward Hooks
==============================================*/
#if defined OnPlayerExitVehicleHealth
forward OnPlayerExitVehicleHealth(playerid, vehicleid);
#endif

#if defined OnPlayerUpdateHealth
forward OnPlayerUpdateHealth(playerid);
#endif

#if defined OnPlayerSpawnHealth
forward OnPlayerSpawnHealth(playerid);
#endif

#if defined OnPlayerDeathHealth
forward OnPlayerDeathHealth(playerid, killerid, reason);
#endif

#if defined OnPlayerDisconnectHealth
forward OnPlayerDisconnectHealth(playerid, reason);
#endif

#if defined OnPlayerConnectHealth
forward OnPlayerConnectHealth(playerid);
#endif

#if defined OnPlayerTakeDamageHealth
forward OnPlayerTakeDamageHealth(playerid, issuerid, Float: amount, weaponid, bodypart);
#endif

#if defined OnPlayerHealthHack
forward OnPlayerHealthHack(playerid, Float:original, Float:anticheat);
#endif

#if defined OnPlayerArmourHack
forward OnPlayerArmourHack(playerid, Float:original, Float:anticheat);
#endif

#if defined OnPlayerGiveDamageHealth
forward OnPlayerGiveDamageHealth(playerid, damagedid, Float: amount, weaponid, bodypart);
#endif

/*============================================
*** Timers
==============================================*/
forward health_ResetImmune(playerid);
public health_ResetImmune(playerid)
{
	hcInfo[playerid][hImmune] = false;
	return 1;
}

forward sprunk_ResetImmune(playerid);
public sprunk_ResetImmune(playerid)
{
	hcInfo[playerid][hImmune] = false;
	new Float:health;
	GetPlayerHealth(playerid, health);
	if(health > 99) SetPlayerHealth(playerid, 99);
	hcInfo[playerid][acHealth] = health;
	return 1;
}

forward armour_ResetImmune(playerid);
public armour_ResetImmune(playerid)
{
	hcInfo[playerid][aImmune] = false;
	return 1;
}

forward checkHealthLoss(playerid);
public checkHealthLoss(playerid)
{
    new Float:health;
	GetPlayerHealth(playerid, health);
	
	new currentHealth;
	currentHealth = floatround(health, floatround_round);
	
	new oldHealth;
	oldHealth = floatround(hcInfo[playerid][acHealth], floatround_round);
	
	if(oldHealth > currentHealth) hcInfo[playerid][acHealth] -= 10;
	
	return 1;
}

forward CheckHealth(playerid);
public CheckHealth(playerid)
{
	if(!hcInfo[playerid][isSpawned] || IsPlayerPaused(playerid)) return 1;
	
	// Thanks to Emmet for OnPlayerUseVending
	for(new i = 0; i < sizeof(s_aVendingMachines); i ++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, s_aVendingMachines[i][0], s_aVendingMachines[i][1], s_aVendingMachines[i][2]))
        {
			if(GetPlayerAnimationIndex(playerid) == 1660)
			{
				OnPlayerUseVending(playerid);
				return 1;
			}
		}
	}

	if(!hcInfo[playerid][hImmune])
	{
	    if(hcInfo[playerid][acHealth] < 0) hcInfo[playerid][acHealth] = 0.0;
	    new Float:health;
	    GetPlayerHealth(playerid, health);

	    new healthOrg;
	    healthOrg = floatround(health, floatround_round);

	    new healthAc;
	    healthAc = floatround(hcInfo[playerid][acHealth], floatround_round);

	    if(healthOrg > healthAc)
	    {
	        #if defined OnPlayerHealthHack
	        OnPlayerHealthHack(playerid, healthOrg, healthAc);
	        #endif
	    }
	}
	if(!hcInfo[playerid][aImmune])
	{
	    if(hcInfo[playerid][acArmour] < 0) hcInfo[playerid][acArmour] = 0.0;

	    new Float:armour;
	    GetPlayerArmour(playerid, armour);

	    new armourOrg;
	    armourOrg = floatround(armour, floatround_round);

	    new armourAc;
	    armourAc = floatround(hcInfo[playerid][acArmour], floatround_round);

	    if(armourOrg > armourAc)
	    {
	        #if defined OnPlayerArmourHack
	        OnPlayerArmourHack(playerid, armourOrg, armourAc);
	        #endif
	    }
	}
	return 1;
}

/*============================================
***	Hooked Callbacks
==============================================*/
public OnPlayerDeath(playerid, killerid, reason)
{
	hcInfo[playerid][isSpawned] = false;
	
    hcInfo[playerid][acArmour] = 0.0;
    hcInfo[playerid][acHealth] = 0.0;
    
    #if defined OnPlayerDeathHealth
        return OnPlayerDeathHealth(playerid, killerid, reason);
	#else
	    return 1;
	#endif
}

public OnPlayerUpdate(playerid)
{
	hcInfo[playerid][pTick] = GetTickCount();
	#if defined OnPlayerUpdateHealth
        return OnPlayerUpdateHealth(playerid);
	#else
	    return 1;
	#endif
}

public OnPlayerDisconnect(playerid, reason)
{
	KillTimer(hcInfo[playerid][hcTimer]);
	
    #if defined OnPlayerDisconnectHealth
        return OnPlayerDisconnectHealth(playerid, reason);
	#else
	    return 1;
	#endif
}

public OnPlayerConnect(playerid)
{
    hcInfo[playerid][isSpawned] = false;
    hcInfo[playerid][aImmune] = false;
    hcInfo[playerid][hImmune] = false;

    hcInfo[playerid][acArmour] = 0.0;
    hcInfo[playerid][acHealth] = 99.0;

    hcInfo[playerid][hcTimer] = SetTimerEx("CheckHealth", 1000, true, "i", playerid);
    
    #if defined OnPlayerConnectHealth
        return OnPlayerConnectHealth(playerid);
    #else
    	return 1;
    #endif
}

public OnPlayerSpawn(playerid)
{
	new health;
	health = floatround(hcInfo[playerid][acHealth], floatround_round);
    if(health == 0) {
		SetPlayerHealth(playerid, 99);
	}
	hcInfo[playerid][isSpawned] = true;
    #if defined OnPlayerSpawnHealth
        return OnPlayerSpawnHealth(playerid);
    #else
    	return 1;
    #endif
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	if(hcGetPlayerSpeed(playerid) >= 20) SetTimerEx("checkHealthLoss", 500, false, "i", playerid);
	
	#if defined OnPlayerExitVehicleHealth
        return OnPlayerExitVehicleHealth(playerid, vehicleid);
    #else
    	return 1;
    #endif
}

public OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart)
{
	if(issuerid == INVALID_PLAYER_ID) hcInfo[playerid][acHealth] -= amount;
	
	#if defined OnPlayerTakeDamageHealth
        return OnPlayerTakeDamageHealth(playerid, issuerid, Float: amount, weaponid, bodypart);
    #else
    	return 1;
    #endif
}

public OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart)
{
	if(damagedid != INVALID_PLAYER_ID)
	{
	    new Float:armour;
	    GetPlayerArmour(damagedid, armour);

	    if(armour > 0.0) hcInfo[damagedid][acArmour] -= amount;
	    else hcInfo[damagedid][acHealth] -= amount;
	}
	#if defined OnPlayerGiveDamageHealth
        return OnPlayerGiveDamageHealth(playerid, damagedid, Float: amount, weaponid, bodypart)
    #else
    	return 1;
    #endif
}
/*============================================
***	Hooked Functions
==============================================*/
stock OnPlayerUseVending(playerid)
{
    if(!hcInfo[playerid][hImmune]) SetTimerEx("sprunk_ResetImmune", 2500, false, "i", playerid);
    hcInfo[playerid][hImmune] = true;
	return 1;
}

stock acSetPlayerHealth(playerid, Float:amount)
{
	if(!hcInfo[playerid][hImmune]) SetTimerEx("health_ResetImmune", 3000, false, "i", playerid);
	printf("Old %f", amount);
	if(amount > 99.0) amount = 99.0;
	if(amount < 0.0) amount = 0.0;
	printf("New %f", amount);
	hcInfo[playerid][hImmune] = true;
	hcInfo[playerid][acHealth] = amount;
	
	return SetPlayerHealth(playerid, amount);
}

stock acSetPlayerArmour(playerid, Float:amount)
{
	if(!hcInfo[playerid][aImmune]) SetTimerEx("armour_ResetImmune", 3000, false, "i", playerid);
	if(amount > 99.0) amount = 99.0;
	if(amount < 0.0) amount = 0.0;

	hcInfo[playerid][aImmune] = true;
	hcInfo[playerid][acArmour] = amount;
	
	return SetPlayerArmour(playerid, amount);
}

stock hcGetPlayerSpeed(playerid)
{
    if(playerid != INVALID_PLAYER_ID)
    {
        new Float:Pos[3],Float:PS;
        GetPlayerVelocity(playerid, Pos[0], Pos[1], Pos[2]);
        
        PS = floatsqroot(Pos[0]*Pos[0] + Pos[1]*Pos[1] + Pos[2]*Pos[2])*200;
        return floatround(PS,floatround_round);
    }
    return INVALID_PLAYER_ID;
}

/*============================================
***	Hookings
==============================================*/
#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif

#define SetPlayerArmour acSetPlayerArmour

#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif

#define SetPlayerHealth acSetPlayerHealth

#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif

#define OnPlayerGiveDamage OnPlayerGiveDamageHealth

#if defined _ALS_OnPlayerTakeDamage
  #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif

#define OnPlayerTakeDamage OnPlayerTakeDamageHealth

#if defined _ALS_OnPlayerExitVehicle
  #undef OnPlayerExitVehicle
#else
    #define _ALS_OnPlayerExitVehicle
#endif

#define OnPlayerExitVehicle OnPlayerExitVehicleHealth

#if defined _ALS_OnPlayerConnect
  #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect OnPlayerConnectHealth

#if defined _ALS_OnPlayerDeath
  #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath OnPlayerDeathHealth

#if defined _ALS_OnPlayerDisconnect
  #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect OnPlayerDisconnectHealth

#if defined _ALS_OnPlayerUpdate
  #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate OnPlayerUpdateHealth

#if defined _ALS_OnPlayerSpawn
  #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn OnPlayerSpawnHealth

#else // It's a filterscript

stock acSetPlayerHealthFS(playerid, Float:amount)
	return CallLocalFunction("acSetPlayerHealth", "if", playerid, amount);

stock acSetPlayerArmourFS(playerid, Float:amount)
	return CallLocalFunction("acSetPlayerArmour", "if", playerid, amount);

#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif

#define SetPlayerArmour acSetPlayerArmourFS

#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif

#define SetPlayerHealth acSetPlayerHealthFS

#endif
