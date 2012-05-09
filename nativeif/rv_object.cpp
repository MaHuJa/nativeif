#include "stdafx.h"
#include "rv.h"
#include "rv_callback.h"
#include "nif.h"

// TODO: See what the game does in the objnull cases

namespace rv {
using namespace nif;
transparent_future<string> Object::get_name() {
	if (is_null_local()) return "";
	auto fut = sqf_cb ( "name " + selfref() );
	return move(fut);
}

void Object::set_damage(float damagelevel) {
	if (is_null_local()) return;
	ostringstream command;
	command << selfref() << " setdamage " << std::fixed << damagelevel << ";";
	sqf(command.str());
}

void Object::delete_vehicle() {
	if (is_null_local()) return;
	sqf("DeleteVehicle " + selfref());
}

transparent_future<bool> Object::is_alive() {
	if (is_null_local()) return false; 
	string query = "alive " + selfref ();
	return query_for<bool> (query);
}

transparent_future<int> Object::get_ammo_in_current_magazine (string muzzlename) {
	if (is_null_local()) return 0;
	string query = selfref() + "ammo " + rv::escapestring(muzzlename);
	return query_for<int> (query);
}

transparent_future<Object> Object::get_assigned_target() {
	if (is_null_local()) return Object(""); 
	string query = "assignedTarget " + selfref();
	return query_for<Object> (query);
}

transparent_future<Object> Object::get_assigned_vehicle() {
	if (is_null_local()) return Object(""); 
	string query = "assignedTarget " + selfref();
	return query_for<Object> (query);
}

transparent_future<bool> Object::is_attack_enabled() {
	if (is_null_local()) return false; 
	string query = "attackenabled " + selfref();
	return query_for<bool> (query);
}

transparent_future<string> Object::get_behavior() {
	if (is_null_local()) return "ERROR"; // Emulates sqf behavior
	string query = "behavior " + selfref();
	return sqf_cb(query);
}

transparent_future<bool> Object::can_fire() {
	if (is_null_local()) return false;
	return query_for<bool> ("canfire" + selfref());
}

transparent_future<bool> Object::can_move() {
	if (is_null_local()) return false;
	return query_for<bool> ("canmove" + selfref());
}

transparent_future<bool> Object::can_stand() {
	if (is_null_local()) return false;
	return query_for<bool> ("canstand" + selfref());
}


transparent_future<string>  Object::get_combat_mode() {
	if (is_null_local()) return "ERROR"; // Emulates sqf behavior
	string query = "combatmode " + selfref();
	return sqf_cb(query);
}

transparent_future<Object>  Object::commander() {
	if (is_null_local()) return Object(""); 
	string query = "commander " + selfref();
	return query_for<Object> (query);
}

//transparent_future<vector<Object>>  Object::crew() { }


transparent_future<float>  Object::get_damage() {
	if (is_null_local()) return 0; 
	return query_for<float> ("damage " + selfref());
}

transparent_future<float> Object::get_direction() {
	if (is_null_local()) return 0; 
	return query_for<float> ("direction " + selfref());
}

transparent_future<float> Object::get_distance(Position p) {
	if (is_null_local()) return 0;	// SQF behavior is distance to [0,0,0]
	return query_for<float> (selfref() + "distance " + p.sqf());
}

transparent_future<float> Object::get_distance(Object o){ 
	if (is_null_local()) return 0;
	if (o.is_null_local()) return 0;
	return query_for<float> (selfref() + "distance " + o.selfref());
}

transparent_future<Object> Object::driver() {
	if (is_null_local()) return Object(""); 
	string query = "driver " + selfref();
	return query_for<Object> (query);
}

transparent_future<Object> Object::effective_commander() {
	if (is_null_local()) return Object("");
	string query = "effectiveCommander " + selfref();
	return query_for<Object> (query);
}
/*
transparent_future<int> Object::get_empty_positions(string type) {
	if (is_null_local()) return 0;
	assert (type == "commander");
}
*/


}	// ns rv
