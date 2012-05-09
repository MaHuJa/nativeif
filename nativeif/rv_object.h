#pragma once
#include "implicit_future.h"
#include "rv_util.h"

namespace rv {
struct Position;

class Object
{
	string netid;
	bool is_null_local() { return netid==""; }
	inline string selfref() {
		if (is_null_local()) throw invalid_rvobject();
		return " (getObjectFromNetID " + netid + ") ";
	}
public:
	// Will only be thrown when a function uses it unchecked
	struct invalid_rvobject : public std::logic_error {
		invalid_rvobject() : logic_error("RV Object reference is invalid.") {}
	};

	explicit Object(const string& id)  {
		if (id!="") netid = escapestring(id);
	};
	explicit Object() {};
	~Object(void) {};

	void set_damage (float damagelevel);	// sqf setdamage
	transparent_future<float> get_damage();	// sqf damage

	transparent_future<Object> commander();	// Aka "primary observer"
	transparent_future<Object> driver();	// Aka "primary observer"
	transparent_future<Object> effective_commander();

	transparent_future<int> get_ammo_in_current_magazine (string weaponname); // sqf ammo
	transparent_future<Object> get_assigned_target();	// set with dotarget, commandTarget; sqf assignedTarget
	transparent_future<Object> get_assigned_vehicle();	// sqf assignedVehicle
	transparent_future<bool> is_attack_enabled(); // sqf attackEnabled
	transparent_future<string> get_behavior();	// "CARELESS", "SAFE", "AWARE", "COMBAT" and "STEALTH"
	transparent_future<bool> can_fire();
	transparent_future<bool> can_move();
	transparent_future<bool> can_stand();
	transparent_future<bool> get_captive(); // TODO: Consider adding "flag" somewhere in the function name
	transparent_future<string> get_combat_mode();	//"BLUE", "GREEN", "WHITE, "YELLOW" and "RED"

	transparent_future<vector<Object>> crew();	// N/I
	transparent_future<int>  get_empty_positions(string);
	transparent_future<string> get_current_command();

	transparent_future<float> get_direction();	// sqf direction
	transparent_future<float> get_distance(Position);
	transparent_future<float> get_distance(Object);


	void delete_vehicle();
	transparent_future<string> get_name();
	transparent_future<bool> is_alive();



	// TODO: Remaining commands
	// transparent_future<ComplexThing> assigned_vehicle_role();
	// 3f+3f boundingBox
	// position buildingPos (int index);	// Mostly used on houses; no netid!!!

	/*

	int = object emptypositions string
	set_engine_on bool // sqf engineon
	bool get_engine_on // sqf isengineon

	*/
};

} // ns rv
