// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    Sega Y-Board hardware

***************************************************************************/
#ifndef MAME_SEGA_SEGAYBD_H
#define MAME_SEGA_SEGAYBD_H

#pragma once

#include "cpu/m68000/m68000musashi.h"
#include "cpu/z80/z80.h"
#include "machine/mb3773.h"
#include "segaic16.h"
#include "sega16sp.h"
#include "screen.h"
#include "machine/motorsim.h"

// ======================> segaybd_state

class segaybd_state : public sega_16bit_common_base
{
public:
	// construction/destruction
	segaybd_state(const machine_config &mconfig, device_type type, const char *tag)
		: sega_16bit_common_base(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_subx(*this, "subx")
		, m_suby(*this, "suby")
		, m_soundcpu(*this, "soundcpu")
		, m_linkcpu(*this, "linkcpu")
		, m_watchdog(*this, "watchdog")
		, m_screen(*this, "screen")
		, m_bsprites(*this, "bsprites")
		, m_ysprites(*this, "ysprites")
		, m_segaic16vid(*this, "segaic16vid")
		, m_adc_ports(*this, "ADC.%u", 0)
		, m_start_lamp(*this, "start_lamp")
		, m_danger_lamp(*this, "danger_lamp")
		, m_crash_lamp(*this, "crash_lamp")
		, m_emergency_stop_lamp(*this, "emergency_stop_lamp")
		, m_bank_data_raw(*this, "bank_data_raw")
		, m_vibration_motor(*this, "vibration_motor")
		, m_bank_motor_position(*this, "bank_motor_position")
		, m_upright_wheel_motor(*this, "upright_wheel_motor")
		, m_left_start_lamp(*this, "left_start_lamp")
		, m_right_start_lamp(*this, "right_start_lamp")
		, m_gun_recoil(*this, "P%u_Gun_Recoil", 1U)
		, m_roll_pos_out(*this, "roll")
		, m_pitch_pos_out(*this, "pitch")
		, m_yaw_pos_out(*this, "yaw")
		, m_lampword_out(*this, "lamps")
		, m_turntable_motor_sim(86, 171, 5.0f, false)
		, m_bank_motor_sim(86, 171, 8.0f, false)
		, m_gloc_left_sim(86, 171, 10.0f, false)
		, m_gloc_right_sim(86, 171, 10.0f, false)

	{
	}

	void yboard_deluxe(machine_config &config);
	void yboard_link(machine_config &config);
	void yboard(machine_config &config);
	void yboard_gloc(machine_config& config);
	void yboard_generic(machine_config& config);
	void yboard_pdrift(machine_config& config);

	// game-specific driver init
	void init_generic();
	void init_pdrift();
	void init_r360();
	void init_gforce2();
	void init_rchase();
	void init_gloc();

protected:
	// device overrides
	virtual void device_resolve_objects() override ATTR_COLD;
	virtual void machine_reset() override ATTR_COLD;
	virtual void video_start() override ATTR_COLD;

private:
	// main CPU read/write handlers
	void output1_w(uint8_t data);
	void misc_output_w(uint8_t data);
	void output2_w(uint8_t data);

	// linked cabinet specific handlers
	void mb8421_intl(int state);
	void mb8421_intr(int state);
	uint16_t link_r();
	uint16_t link2_r();
	void link2_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	//uint8_t link_portc0_r();

	// input helpers
	ioport_value analog_mux();
	ioport_value adc0();
	ioport_value adc1();
	ioport_value adc2_pdrift();

	// game-specific output handlers
	void gforce2_output_cb1(uint16_t data);
	void gforce2_output_cb2(uint16_t data);
	void gloc_output_cb1(uint16_t data);
	void gloc_output_cb2(uint16_t data);
	void r360_output_cb2(uint16_t data);
	void pdrift_output_cb1(uint16_t data);
	void pdrift_output_cb2(uint16_t data);
	void rchase_output_cb2(uint16_t data);

	// video updates
	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void link_map(address_map &map) ATTR_COLD;
	void link_portmap(address_map &map) ATTR_COLD;
	void main_map(address_map &map) ATTR_COLD;
	void main_map_link(address_map &map) ATTR_COLD;
	void motor_map(address_map &map) ATTR_COLD;
	void sound_map(address_map &map) ATTR_COLD;
	void sound_portmap(address_map &map) ATTR_COLD;
	void subx_map(address_map &map) ATTR_COLD;
	void suby_map(address_map &map) ATTR_COLD;

	// internal types
	typedef delegate<void (uint16_t)> output_delegate;

	// internal helpers
	TIMER_CALLBACK_MEMBER(irq2_gen_tick);
	void update_irqs();

	// devices
	required_device<m68000msh_device> m_maincpu;
	required_device<m68000msh_device> m_subx;
	required_device<m68000msh_device> m_suby;
	required_device<z80_device> m_soundcpu;
	optional_device<z80_device> m_linkcpu;
	required_device<mb3773_device> m_watchdog;
	required_device<screen_device> m_screen;
	required_device<sega_sys16b_sprite_device> m_bsprites;
	required_device<sega_yboard_sprite_device> m_ysprites;
	required_device<segaic16_video_device> m_segaic16vid;

	// input ports
	optional_ioport_array<6> m_adc_ports;

	// outputs
	output_finder<> m_start_lamp;
	output_finder<> m_danger_lamp;
	output_finder<> m_crash_lamp;
	output_finder<> m_emergency_stop_lamp;
	output_finder<> m_bank_data_raw;
	output_finder<> m_vibration_motor;
	output_finder<> m_bank_motor_position;
	output_finder<> m_upright_wheel_motor;
	output_finder<> m_left_start_lamp;
	output_finder<> m_right_start_lamp;
	output_finder<2> m_gun_recoil;
	output_finder<> m_roll_pos_out;
	output_finder<> m_pitch_pos_out;
	output_finder<> m_yaw_pos_out;
	output_finder<> m_lampword_out;

	// configuration
	output_delegate m_output_cb1;
	output_delegate m_output_cb2;

	// internal state
	uint16_t m_pdrift_bank = 0;
	emu_timer *m_scanline_timer = nullptr;
	int m_irq2_scanline = 0;
	uint8_t m_timer_irq_state = 0;
	uint8_t m_vblank_irq_state = 0;
	uint8_t m_misc_io_data = 0;

	lamps_t m_lamps;

	//-------------------------------------------------
	//  r360_output_cb2 - output #2 handler for
	//  G-Loc R360
	//-------------------------------------------------

	motor m_turntable_motor_sim;
	motor m_bank_motor_sim;
	motor m_gloc_left_sim;
	motor m_gloc_right_sim;

	//TODO: Analyse and standardise powerdrift
	//motor pdrift_motor_sim(86, 171, 4.0f, false);

	bool m_gforce = 0; // game specific code
	int m_gforce_adc_turntable = 0x00;
	int m_gforce_adc_bank = 0x00;
	
	int m_gforce_turnspeed = 0;
	int m_gforce_bankspeed = 0;

	int m_gloc_right_motor_pos_target;
	int m_gloc_right_motor_speed;
	int m_gloc_left_motor_pos_target;
	int m_gloc_left_motor_speed;
	int m_gloc_left_pos = 0x80;
	int m_gloc_right_pos = 0x80;

};



#endif // MAME_SEGA_SEGAYBD_H
