--	2022 EPO-2 Smart Robot Challenge
--	Mentor group B4-2

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity controller is
	port (	clk			: in	std_logic;
		reset			: in	std_logic;

		sensor_l		: in	std_logic;
		sensor_m		: in	std_logic;
		sensor_r		: in	std_logic;

		route_straight	: in	std_logic; -- 1 to go straight
		route_corner	: in	std_logic; -- 1 to turn left, 0 to turn right
		mine_input				: in	std_logic; -- mine detected
		maneuver_complete : out std_logic;
		uart_write			: out std_logic;
		uart_read			: out std_logic;
		
		count_in		: in	std_logic_vector (19 downto 0);
		count_reset		: out	std_logic;

		motor_l_reset		: out	std_logic;
		motor_l_direction	: out	std_logic;

		motor_r_reset		: out	std_logic;
		motor_r_direction	: out	std_logic
	);
end entity controller;

architecture sys_operator of controller is
	
	type control_state is (rst, ff, fs, fr, sf, rf, turnaround);
	-- ff means forward forward, sf means stop foward etc.
	signal state, new_state: control_state;
	constant twenty_ms: unsigned := to_unsigned(1000000, 20);

	-- constant route_straight: std_logic := '1'; -- always straight
	-- constant route_corner: std_logic := '1';
	
	signal crossing: std_logic := '1'; -- 0 when a middle black-black-black (or mine) is ahead, 1 when a crossing is ahead
	signal wait_for_line: std_logic; -- 1 when a corner is being taken

begin
	-- Switch states
	reg: process(clk)

	begin
		if rising_edge(clk) then
			if reset = '1' then
				state <= rst;
			else
				state <= new_state;
			end if;
		end if;
	end process;

	-- Control outputs
	fsm: process(clk)

	begin
		if rising_edge(clk) then
			case state is
				when rst =>
					count_reset <= '1';
					motor_l_direction <= '0';
					motor_l_reset <= '1';
					motor_r_direction <= '1';
					motor_r_reset <= '1';
					maneuver_complete <= '0';
					uart_read <= '0';
					uart_write <= '1';
					wait_for_line <= '0';
					if mine_input = '1' then
						new_state <= turnaround;
					elsif sensor_l = '1' and sensor_m = '0' and sensor_r = '0' then
						new_state <= fs;
					elsif sensor_l = '1' and sensor_m = '1' and sensor_r = '0' then
						new_state <= fr;
					elsif sensor_l = '0' and sensor_m = '0' and sensor_r = '1' then
						new_state <= sf;
					elsif sensor_l = '0' and sensor_m = '1' and sensor_r = '1' then
						new_state <= rf;
					elsif sensor_l = '0' and sensor_m = '0' and sensor_r = '0' then
						-- Update the crossing signal
						if crossing = '0' then -- reached a midpoint on the line
							crossing <= '1';
							-- Send maneuver complete signal
							maneuver_complete <= '1';
							uart_write <= '1';
							uart_read <= '0';
							wait_for_line <= '0';
						else
							-- Wait to recieve signal
							maneuver_complete <= '0';
							uart_write <= '0';
							uart_read <= '1';
							-- Decide the route
							if route_straight = '1' then
								new_state <= ff;
								crossing <= '0';
								wait_for_line <= '0';
							else
								-- Left
								if route_corner = '1' then
									new_state <= rf;
									crossing <= '0';
									wait_for_line <= '1';
								-- Right
								else
									new_state <= fr;
									crossing <= '0';
									wait_for_line <= '1';
								end if;
							end if;
						end if;
					else
						new_state <= ff;
					end if;
				when fs =>
					count_reset <= '0';
					motor_l_direction <= '1';
					motor_l_reset <= '0';
					motor_r_direction <= '0';
					motor_r_reset <= '1';
					if unsigned(count_in) >= twenty_ms then
						new_state <= rst;
					end if;
				when fr =>
					count_reset <= '0';
					motor_l_direction <= '1';
					motor_l_reset <= '0';
					motor_r_direction <= '1';
					motor_r_reset <= '0';
					if wait_for_line = '0' then
						if unsigned(count_in) >= twenty_ms then
							new_state <= rst;
						end if;
					else
						if sensor_l = '1' and sensor_m = '1' and sensor_r = '1' then
							new_state <= ff;
						end if;
					end if;
				when sf =>
					count_reset <= '0';
					motor_l_direction <= '0';
					motor_l_reset <= '1';
					motor_r_direction <= '0';
					motor_r_reset <= '0';
					if unsigned(count_in) >= twenty_ms then
						new_state <= rst;
					end if;
				when rf =>
					count_reset <= '0';
					motor_l_direction <= '0';
					motor_l_reset <= '0';
					motor_r_direction <= '0';
					motor_r_reset <= '0';
					if wait_for_line = '0' then
						if unsigned(count_in) >= twenty_ms then
							new_state <= rst;
						end if;
					else
						if sensor_l = '1' and sensor_m = '1' and sensor_r = '1' then
							new_state <= ff;
						end if;
					end if;
				when ff =>
					count_reset <= '0';
					motor_l_direction <= '1';
					motor_l_reset <= '0';
					motor_r_direction <= '0';
					motor_r_reset <= '0';
					if wait_for_line = '0' then
						if unsigned(count_in) >= twenty_ms then
							new_state <= rst;
						end if;
					else
						if sensor_l = '0' or sensor_m = '0' or sensor_r = '0' then
							new_state <= rst;
						end if;
					end if;
				when turnaround => ---TODO if mine detected, send special command via uart
					count_reset <= '0';
					motor_l_direction <= '0';
					motor_l_reset <= '0';
					motor_r_direction <= '0';
					motor_r_reset <= '0';
					crossing <= '1';
					if sensor_l = '1' and sensor_m = '0' and sensor_r = '1' then
						new_state <= rst;
					end if;
			end case;
		end if;
	end process;

end architecture sys_operator;