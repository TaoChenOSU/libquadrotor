#pragma once

#include "math3d.h"
#include "quad_common_types.h"


// in both position and attitude of a quadrotor, the third axis is "special".
// for PID controller parameters, we generally want the same value for x and y
// and a different value for z.
struct xy_z {
	float xy;
	float z;
};

struct quad_ctrl_params
{
	struct {
		struct xy_z kp;
		struct xy_z ki;
		struct xy_z kd;
		struct vec int_bound;
	} linear;

	struct {
		struct xy_z kp;
		struct xy_z ki; // integral term is combination of omega err and attitude error
		struct xy_z kd;
		struct vec int_bound;
	} attitude;
};

struct quad_ctrl_state
{
	struct vec int_linear_err;
	struct vec int_attitude_err;
};

void quad_ctrl_default_params(struct quad_ctrl_params *params);

void quad_ctrl_init(struct quad_ctrl_state *state);

// full nonlinear position control.
// returns desired moment and normalized thrust magnitude (in m/s^2)
struct quad_accel quad_ctrl_full(
	struct quad_ctrl_state *state,
	struct quad_ctrl_params const *param,
	struct quad_state const *s, struct quad_state const *set, float dt);

// returns desired moment
struct vec quad_ctrl_attitude(
	struct quad_ctrl_state *state,
	struct quad_ctrl_params const *param,
	struct quad_state const *s, struct quad_state const *set, float dt);

//
// Attitude rate control, aka "acrobatic mode" for human pilots.
// does not use full quad_state struct since it's unnecessary.
// returns desired moment
//
struct vec quad_ctrl_attitude_rate(
	struct quad_ctrl_state *state,
	struct quad_ctrl_params const *param,
	struct vec s, struct vec set, float thrust, float dt);

// compute the force to be generated by each propeller.
// output is prop_forces in newtons (kg * m/s^2)
void quad_power_distribute(
	struct quad_accel const *acc,
	struct quad_physical_params const *params,
	float prop_forces[4]);


// manual pilot controls roll & pitch via absolute angles (right stick)
// but controls world csys yaw via rate (horiz axis of left stick).
// assuming the caller integrates the yaw rate control,
// convert these to a desired rotation matrix.
// all should be given in radians.
/*
struct mat33 quad_sticks2mat(
	float stick_roll, float stick_pitch, float stick_yaw_integral)
{
	// compute the unit vector implied by the stick roll and pitch angles
	float const y = sin(stick_roll);
	float const x = sin(stick_pitch);
	float const z = sqrtf(1.0f - x*x - y*y);

	
}
*/

